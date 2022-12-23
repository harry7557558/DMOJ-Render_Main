"""
Input format:
N M V  # num_joints, num_wires, battery_voltage
ai, bi, Ri  # joint_1, joint_2, resistance; M lines
1 is battery +, N is battery -
"""
N, M, V = map(int, input().split())
W = []
R = []
for i in range(M):
    a, b, r = map(int, input().split())
    W.append((a-1, b-1))
    R.append(r)

"""
Solve a linear system.
Unknowns:
 - Current in each wire (M)
 - Electric potential at each joint (N)
Equations:
 - V = I R (M)
 - I[in] = I[out] (N)
"""
import numpy as np
import scipy.sparse
import scipy.sparse.linalg
import scipy.optimize

Ai, Aj, Av = [], [], []
for i in range(M):
    Ai += [i, i, i]
    Aj += [M+W[i][0], M+W[i][1], i]
    Av += [1.0, -1.0, -R[i]]
for i in range(M):
    Ai += [M+W[i][0], M+W[i][1]]
    Aj += [i, i]
    Av += [1.0, -1.0]
Ai.append(M+N-1)
Aj.append(M+N-1)
Av.append(1.0)
A = scipy.sparse.csr_matrix((Av, (Ai, Aj)), shape=((M+N, M+N)))
if M+N > 4 and False:
    eigs = scipy.sparse.linalg.eigs(A, k=2, which='SM', return_eigenvectors=False)
    assert sorted(abs(eigs))[1] > 1e-10

b = [0.0]*(M+N)
b[M], b[M+N-1] = 1.0, -1.0  # we don't know the current at this time

# matrix is singular, electric potentials are shifted by a constant

def err(x):
    return np.amax(abs(A*x-b))/(A.multiply(A).sum()**0.5/(M+N))

e = 1e-20
np.random.seed(0)
try:
    x = scipy.sparse.linalg.spsolve(A, b)
    #print(err(x))
    assert err(x) < 1e-8
except AssertionError:
    try:
        x0 = np.random.normal(1.0, 10.0, size=M+N)
        x = scipy.sparse.linalg.cg(A, b, x0=x0, tol=e)[0]
        #print(err(x))
        assert err(x) < 1e-8
    except AssertionError:
        try:
            x = scipy.sparse.linalg.minres(A, b, tol=e)[0]
            #print(err(x))
            assert err(x) < 1e-8
        except AssertionError:
            x = scipy.sparse.linalg.lgmres(A, b, tol=e, atol=e)[0]
            #print(err(x))
            assert err(x) < 1e-8

I_overall = V / (x[M]-x[M+N-1])
sol = I_overall * x[:M]
print('\n'.join(map(str, sol)))


if False:
    import matplotlib.pyplot as plt
    precond = np.diag([1/r**0.5 for r in R]+[5e4]*N)
    A = A.todense()
    A = precond*A*precond
    invA = np.linalg.inv(A)
    fig, (ax1, ax2) = plt.subplots(1, 2)
    im1 = ax1.matshow((np.maximum(abs(A),1e-16)))
    #im2 = ax2.matshow((np.maximum(abs(invA[:M,:M]),1e-16)))
    im2 = ax2.matshow(np.log10(np.maximum(abs(invA),1e-16)))
    fig.colorbar(im2)
    plt.show()
