import numpy as np
import scipy.sparse
import matplotlib.pyplot as plt


def cross_product_matrix(r):
    return np.array([
        [0, -r[2], r[1]],
        [r[2], 0, -r[0]],
        [-r[1], r[0], 0]
    ])


def solve_reactions_check(joints, forces, fixed):
    """Check the attempt to solve for all reaction forces,
        seems like the rank of the matrix is
        at most 5 for two pins and at most 6 for >=3 pins
    Intuition:
        Can rotate about an axis for 2 pins;
        3N variables and 6 equations for N pins."""

    # solve for reaction forces
    # net force = 0, net moment = 0
    A = np.zeros((3*len(fixed)+3, 3*len(fixed)))
    b = np.zeros(3*len(fixed)+3)

    # moment equations
    for j0 in range(len(fixed)):
        xj0 = joints[fixed[j0]]#*0
        for j in range(len(fixed)):
            xj = joints[fixed[j]]
            r = cross_product_matrix(xj - xj0)
            A[3*j0:3*j0+3, 3*j:3*j+3] = r
        for i in range(len(forces)):
            xi = joints[i]
            r = cross_product_matrix(xi - xj0)
            b[3*j0:3*j0+3] -= r @ forces[i]

    # equilibrium equations
    for j in range(len(fixed)):
        A[3*len(fixed):, 3*j:3*j+3] = np.eye(3)
    for i in range(len(forces)):
        b[3*len(fixed):] -= forces[i]

    # balance
    for i in range(0, len(A), 3):
        A[i:i+3, :] /= np.mean(A[i:i+3, :]**2)**0.5

    fig, (ax1, ax2) = plt.subplots(1, 2)
    ax1.matshow(abs(A))

    # solve Ax=b
    A, b = A.T@A, A.T@b
    print(sorted(np.linalg.eig(A)[0].real))

    ax2.matshow(abs(A))
    plt.show()



def solve_truss(joints, forces, fixed, members, ks, verbose=False):
    #solve_reactions_check(joints, forces, fixed)
    N = len(joints)
    M = len(members)

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
    print(min(np.linalg.eig(mats[0][0].todense())[0].real))
    #print(min(np.linalg.eig(mats[1][1].todense())[0].real))

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

    # get the result
    if verbose:

        net_f = sum(fa)
        assert np.linalg.norm(net_f) < 1e-8
        net_m = sum([cross_product_matrix(joints[i])@fa[i] for i in range(N)])
        assert np.linalg.norm(net_m) < 1e-4
        fa_ = (C * fm).reshape(N, 3)
        assert np.linalg.norm(fa-fa_) < 1e-6

        print("Reaction")
        for i, fi in zip(fixed, fr):
            print(i+1, fi)
        print("Member")
        for i, fi in zip(range(M), fm):
            print(tuple([a+1 for a in members[i]]), fi)
        print("Deflection")
        for i, ui in zip(nonfixed, u):
            print(i+1, ui)

    return (fa, fm, u0)


def truss_1():
    joints = [
        (0, 0, 0),
        (2000, 0, 0),
        (4000, 0, 0),
        (0, 2000, 0),
        (2000, 2000, 0),
        (4000, 2000, 0),
        (500, 1000, -1000),
        (3500, 1000, -1000)
    ]
    weights = [0, 0, 0, 0, 0, 0, 5000, 1000]
    fixed = [1, 2, 3, 5]
    members = [
        (1, 2), (2, 3), (1, 4), (2, 5), (3, 6), (4, 5), (5, 6),
        (1, 7), (2, 7), (4, 7), (5, 7), (2, 8), (3, 8), (5, 8), (6, 8), (7, 8),
        #(1, 8), (4, 8), (3, 7), (6, 7)
    ]
    EA = 2e5 * 100

    joints = [np.array(x, dtype=np.float64) for x in joints]
    weights = [np.array([0, 0, -w], dtype=np.float64) for w in weights]
    fixed = [f-1 for f in fixed]
    members = [(a-1, b-1) for (a, b) in members]
    ks = [EA/np.linalg.norm(joints[b]-joints[a]) for (a, b) in members]

    solve_truss(joints, weights, fixed, members, ks, verbose=True)


if __name__ == "__main__":
    truss_1()
