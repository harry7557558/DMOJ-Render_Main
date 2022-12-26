import numpy as np
import scipy.sparse
import matplotlib.pyplot as plt


def cross_product_matrix(r):
    return np.array([
        [0, -r[2], r[1]],
        [r[2], 0, -r[0]],
        [-r[1], r[0], 0]
    ])


def format_float(x, s=3):
    """CIV102 convension"""
    try:
        return '[' + ' '.join([format_float(xi) for xi in iter(x)]) + ']'
    except TypeError:
        if not np.isfinite(x):
            return str(x)
        if abs(x) <= 1e-10:
            return '0'
        if np.log10(abs(x)) >= s:
            return str(round(x))
        sigfig = s+1 if np.log10(abs(x))%1.0 < np.log10(2) else s
        return f"{{:.{sigfig}g}}".format(x)


def disjoint_components(N, members):
    """N=len(joints), 0-indexed members"""
    # disjoint set
    parent = [-1] * N
    rank = [0] * N
    def find_rep(i):
        if parent[i] < 0:
            return i
        ans = find_rep(parent[i])
        parent[i] = ans
        return ans
    # union
    for i, j in members:
        ir, jr = find_rep(i), find_rep(j)
        if ir == jr:
            continue
        if rank[ir] < rank[jr]:
            parent[ir] = parent[i] = jr
        elif rank[ir] > rank[jr]:
            parent[jr] = parent[j] = ir
        else:
            parent[jr] = parent[j] = ir
            rank[ir] += 1
    # find disjoint ones
    res = {}
    for i, j in members:
        ir, jr = find_rep(i), find_rep(j)
        assert ir == jr
        if ir not in res:
            res[ir] = set()
        res[ir].add(i)
        res[ir].add(j)
    return [list(dj) for dj in res.values()]


def solve_truss(joints, weights, fixed, members, EA, verbose=''):
    """Returns (applied_reaction_forces, member_forces, deflections)
        1-index for `fixed` and `members`"""
    N = len(joints)
    M = len(members)
    joints = [np.array(x, dtype=np.float64) for x in joints]
    forces = [np.array([0, 0, -w], dtype=np.float64) for w in weights]
    fixed = [f-1 for f in fixed]
    members = [(a-1, b-1) for (a, b) in members]
    ks = [EA/np.linalg.norm(joints[b]-joints[a]) for (a, b) in members]

    # Get stiffness matrix
    Cv, Ci, Cj = [], [], []
    for i in range(len(members)):
        a, b = members[i]
        dp = joints[a]-joints[b]
        dp /= np.linalg.norm(dp)
        Cv += [dp[0], dp[1], dp[2], -dp[0], -dp[1], -dp[2]]
        Ci += [3*a, 3*a+1, 3*a+2, 3*b, 3*b+1, 3*b+2]
        Cj += [i]*6
    C = scipy.sparse.csr_matrix((Cv, (Ci, Cj)), shape=(3*N, M))
    K = scipy.sparse.csr_matrix(
        (ks, (range(M), range(M))),
        shape=(M, M))
    A = C @ K @ C.T

    #plt.matshow(abs(C.todense()))
    #plt.matshow(abs((C.T@C).todense()))
    #plt.matshow(abs(A.todense()))
    #plt.show()

    # Rearrange
    nonfixed = list(set(range(N)).difference(set(fixed)))
    fixed_d = dict(zip(fixed, range(len(fixed))))
    nonfixed_d = dict(zip(nonfixed, range(len(nonfixed))))
    coo = scipy.sparse.coo_matrix(A)
    mats = [[[], []], [[], []]]
    for i, j, v in zip(coo.row, coo.col, coo.data):
        ii, ic = i//3, i%3
        ji, jc = j//3, j%3
        ifx, jfx = (ii in fixed_d), (ji in fixed_d)
        ii = (nonfixed_d, fixed_d)[ifx][ii]
        ji = (nonfixed_d, fixed_d)[jfx][ji]
        mats[ifx][jfx].append((3*ii+ic, 3*ji+jc, v))
    for i in range(2):
        for j in range(2):
            m = mats[i][j]
            s = [3*len(nonfixed), 3*len(fixed)]
            mats[i][j] = scipy.sparse.csr_matrix(
                ([e[2] for e in m], ([e[0] for e in m], [e[1] for e in m])),
                shape=(s[i], s[j]))

    # Check for singularity
    if 'c' in verbose:
        n = 3*len(nonfixed)
        if n < 3000:
            eigvals = np.linalg.eigvalsh(mats[0][0].todense())
            #print(sorted(abs(eigvals)))
            print("cond =", abs(max(eigvals)/min(eigvals)))
        else:
            M = scipy.sparse.csr_matrix(
                (1.0/mats[0][0].diagonal(), (list(range(n)), list(range(n)))),
                shape=(n, n))
            X = np.random.normal(size=(n, 1))
            mineig, _ = scipy.sparse.linalg.lobpcg(
                mats[0][0], X, M=M, largest=False, maxiter=10000)
            maxeig, _ = scipy.sparse.linalg.lobpcg(
                mats[0][0], X, M=M, largest=True, maxiter=10000)
            print("cond =", abs(maxeig[0]/mineig[0]))
        pass

    # Solve
    f = np.array([forces[i] for i in nonfixed]).reshape(3*len(nonfixed))
    u = scipy.sparse.linalg.spsolve(mats[0][0], f)  # deflection
    fr = mats[1][0] * u  # reaction force
    fr = fr.reshape((len(fixed), 3))
    u = u.reshape((len(nonfixed), 3))
    fa = [np.array(f) for f in forces]
    for i, j in zip(range(len(fixed)), fixed):
        fa[j] = fr[i]
    u0 = np.zeros(3*N)
    for i, j in zip(range(len(nonfixed)), nonfixed):
        u0[3*j:3*j+3] = u[i]
    fm = K * C.T * u0  # member force
    u0 = u0.reshape(N, 3)

    # Correctness check
    if 'a' in verbose:
        fa1 = np.array(fa)
        for l in disjoint_components(len(joints), members):
            net_f = sum(fa1[l]) / len(l)
            c = np.mean(np.array(joints)[l], axis=0)
            net_m = sum([cross_product_matrix(joints[i]-c)@fa1[i] for i in l]) / len(l)
            #print(np.linalg.norm(net_f), np.linalg.norm(net_m))
            assert np.linalg.norm(net_f) < 1e-6
            assert np.linalg.norm(net_m) < 1e-3
        fa_ = (C * fm).reshape(N, 3)
        #print(np.linalg.norm(fa-fa_)/len(fm))
        assert np.linalg.norm(fa-fa_)/len(fm) < 1e-8
        diff = ((fa-fa_).T[0]**2 + (fa-fa_).T[1]**2 + (fa-fa_).T[2]**2)**.5
        #print(np.amax(diff))
        assert np.amax(diff) < 1e-5

    # Print result
    if 'p' in verbose:
        print("Reaction")
        for i, fi in zip(fixed, fr):
            print(i+1, format_float(fi))
        print("Member")
        for i, fi in zip(range(M), fm):
            print(tuple([a+1 for a in members[i]]), format_float(fi))
        print("Deflection")
        for i, ui in zip(nonfixed, u):
            print(i+1, format_float(ui))

    return (fa, fm, u0)


if __name__ == "__main__":
    import trusses
    solve_truss(*trusses.roof(), verbose='cap')
