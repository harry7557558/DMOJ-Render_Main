"""
Input format:
N M V  # num_joints, num_wires, battery_voltage
ai, bi, Ri  # joint_1, joint_2, resistance; M lines
1 is battery +, N is battery -
"""
N, M, V0 = map(int, input().split())
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
Matrix formulation:
 - Matrices: Diagonal matrix R, connection C, reference voltage Z;
 - Solution vectors: currents I, voltages V
Equations (matrix form):
 - RI + CV = 0   =>   I = -invR C V
 - C'I + ZV = I0   =>   (C' invR C - Z) V = -I0
 - The sign of I0 doesn't matter because we need to
   multiply by reference current;
   Also drop the - sign in the first equation;
 - Make Z rank-1 and negative so the matrix in the
   second equation is positive definite.
Steps:
 - Solve (C' invR C - Z) V = I0
 - Evaluate I = invR C V
"""

import numpy as np
import scipy.sparse
import scipy.sparse.linalg
import scipy.optimize

invR = scipy.sparse.coo_matrix(
    ([1.0/r for r in R], (list(range(M)), list(range(M)))),
    shape=(M, M))
C = scipy.sparse.coo_matrix(
    ([-1.0]*M+[1.0]*M, (
        list(range(M))*2,
        [w[0] for w in W] + [w[1] for w in W]
    )),
    shape=(M, N))
Z = scipy.sparse.coo_matrix(([1.0], ([0], [0])), shape=(N, N))

A = C.T * invR * C + Z

b = [1.0] + [0.0]*(N-2) + [-1.0]  # assume the current is 1

def err(x):
    return np.amax(abs(A*x-b))/(A.multiply(A).sum()**0.5/(M+N))

# the solution should be unique if this doesn't give an error
V = scipy.sparse.linalg.spsolve(A, b)

I_overall = V0 / (V[N-1]-V[0])
I = invR * C * (I_overall*V)
print('\n'.join(map(str, I)))


if False:
    import matplotlib.pyplot as plt
    precond = scipy.sparse.csr_matrix(
        (A.diagonal()**-0.5, (list(range(N)), list(range(N)))),
        shape=(N, N))
    A = precond * A * precond
    A = A.todense()
    invA = np.linalg.inv(A)
    fig, (ax1, ax2) = plt.subplots(1, 2)
    im1 = ax1.matshow((np.maximum(abs(A),1e-16)))
    im2 = ax2.matshow(np.log10(np.maximum(abs(invA),1e-16)))
    fig.colorbar(im2)
    plt.show()
