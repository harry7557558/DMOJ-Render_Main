import numpy as np
import matplotlib.pyplot as plt

from solver_1 import format_float, solve_truss
import trusses


def colormap(t):
    """-1 < t < 1"""
    t = max(0.0, min(0.5+0.5*t, 1.0))
    w = np.array([
        (132.23,.39,-142.83),
        (-245.97,-1.4,270.69),
        (755.63,1.32,891.31),
        (.3275,2.39,.3053),
        (-1.7461,-1.84,1.4092)
    ])
    c = w[0]+w[1]*t+w[2]*np.cos(w[3]*t+w[4])
    return '#' + ''.join(["{:02x}".format(max(0, min(255, round(255*x)))) for x in c])


def print_solution_info(joints, fixed, members, EA, fa, fm, u):
    """See if the solution makes sense"""
    fixed = [i-1 for i in fixed]
    nonfixed = list(set(range(len(joints))).difference(set(fixed)))
    print("{} joints ({} fixed, {} nonfixed)".format(
        len(joints), len(fixed), len(nonfixed)))
    print(len(members), "members")
    # cross section properties
    # fitting: https://www.desmos.com/calculator/0srvstbp74
    E = 2e5
    A = EA / E
    I = 0.099 * A ** 2.24
    r = (I/A)**0.5
    W = 0.077 * A
    # geometry / safety
    joints = np.array(joints)
    fm = np.array(fm)
    fa = np.array(fa)
    L = np.array([np.linalg.norm(joints[b-1]-joints[a-1]) for a, b in members])
    member_weights = W * L / 1000
    joint_weights = np.zeros(len(joints))
    for ((a, b), w) in zip(members, member_weights):
        joint_weights[a-1] += 0.5*w
        joint_weights[b-1] += 0.5*w
    maxw, minw = np.amax(joint_weights), np.amin(joint_weights)
    maxfa, minfa = np.amax(-fa[nonfixed][:, 2]), np.amin(-fa[nonfixed][:, 2])
    maxfr = np.amax(np.linalg.norm(fa[fixed], axis=1))
    maxfm, minfm = np.amax(fm), np.amin(fm)
    maxu = np.amax(np.linalg.norm(u, axis=1))
    maxlr = np.amax(L/r * (fm < 0))
    min_buckle_fos = 1.0 / np.amax(-fm / (np.pi**2*E*I/L**2) * (fm < 0))
    # print
    print("Dead load range:", format_float(minw), format_float(maxw))
    print("Applied load range:", format_float(minfa), format_float(maxfa))
    print("Max reaction:", format_float(maxfr))
    print("Member force range:", format_float(maxfm), format_float(minfm))
    print("Stress range:", format_float(maxfm/A), format_float(minfm/A))
    print("Max slenderness:", format_float(maxlr))
    print("Min buckle FoS:", format_float(min_buckle_fos))
    print("Max deflection:", format_float(maxu))


def plot_truss(joints, fixed, members, fa, fm):
    joints = np.array(joints)
    fa = np.array(fa)
    fixed = [i-1 for i in fixed]
    nonfixed = list(set(range(len(joints))).difference(set(fixed)))

    ax = plt.axes(projection="3d")

    # fixed joints
    ps = joints[fixed]
    ax.plot(*ps.T, 'ko', label="fixed joints")

    # members
    maxfm = max(abs(fm))
    for (f, (a, b)) in zip(fm, members):
        ps = joints[[a-1, b-1]]
        c = colormap(f/maxfm)
        ax.plot(*ps.T, c=c)

    # applied forces
    fl = max(np.linalg.norm(fa, axis=1))
    ml = np.amax(np.amax(joints, axis=0) - np.amin(joints, axis=0))
    ax.quiver(*joints[fixed].T, *fa[fixed].T, length=0.5*ml/fl,
              color='m', label="reaction forces")
    ax.quiver(*joints[nonfixed].T, *fa[nonfixed].T, length=0.5*ml/fl,
              color='k', label="applied forces")

    # equal aspect ratio - https://stackoverflow.com/a/63625222/16318343
    limits = np.array([
        ax.get_xlim3d(),
        ax.get_ylim3d(),
        ax.get_zlim3d(),
    ])
    center = np.mean(limits, axis=1)
    radius = 0.55 * np.max(np.abs(limits[:, 1] - limits[:, 0]))
    ax.set_xlim3d([center[0] - radius, center[0] + radius])
    ax.set_ylim3d([center[1] - radius, center[1] + radius])
    ax.set_zlim3d([center[2] - 0.8*radius, center[2] + 0.8*radius])

    ax.legend()
    plt.show()


if __name__ == "__main__":

    joints, weights, fixed, members, EA = trusses.sample_1()
    fa, fm, u = solve_truss(joints, weights, fixed, members, EA, verbose='c')
    print_solution_info(joints, fixed, members, EA, fa, fm, u)
    plot_truss(joints, fixed, members, fa, fm)


