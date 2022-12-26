import numpy as np
import solver_1 as solver


def _return(joints, weights, fixed, members, EA):
    """Force integer and 1-indexedness when return data.
        Should I shuffle the order for CP purpose here? :thinking:
    """
    shift = 1 if min([min(m) for m in members]) == 0 else 0
    return (
        [(round(p[0]), round(p[1]), round(p[2])) for p in joints],
        [round(w) for w in weights],
        sorted(set([i+shift for i in fixed])),
        sorted(set([(a+shift, b+shift) for (a, b) in members])),
        round(EA)
    )


## Help functions


def stlread(filename):
    from struct import unpack
    fp = open(filename, 'rb')
    fp.seek(80)
    fn = unpack("<I", fp.read(4))[0]  # number of faces
    vsmap = {}
    es = {}
    for i in range(fn):
        buffer = unpack("<ffffffffffff", fp.read(48))
        vs = [buffer[3:6], buffer[6:9], buffer[9:12]]
        f = []
        for v in vs:
            if v not in vsmap:
                vsmap[v] = len(vsmap)
            f.append(vsmap[v])
        f.append(f[0])
        for i in range(3):
            t = tuple(sorted(f[i:i+2]))
            if t not in es:
                es[t] = 0
            es[t] += 1
        fp.read(2)
    fixes = set({})
    for edge, count in es.items():
        if count < 2:
            fixes.add(edge[0])
            fixes.add(edge[1])
    fixes = sorted(fixes)
    vs = sorted(vsmap.items(), key=lambda _: _[1])
    vs = np.array([v[0] for v in vs])
    es = sorted(es.keys())
    weights = np.zeros(len(vs))
    for a, b in es:
        dl = np.linalg.norm(vs[b]-vs[a])
        weights[a] += 0.5*dl
        weights[b] += 0.5*dl
    return vs, fixes, es, weights


def connect_members(joints, L):
    """Connect all pairs of joints with distance less than L"""
    # generate lookup table
    sp = {}
    joints_i = [
        (round(p[0]/(2*L)), round(p[1]/(2*L)), round(p[2]/(2*L)))
        for p in joints]
    for i, pi in zip(range(len(joints)), joints_i):
        if pi in sp:
            sp[pi].append(i)
        else:
            sp[pi] = [i]
    # get members
    members = []
    for i in range(len(joints)):
        p1, p1i = joints[i], joints_i[i]
        for _x in [-1, 0, 1]:
            for _y in [-1, 0, 1]:
                for _z in [-1, 0, 1]:
                    p1j = (p1i[0]+_x, p1i[1]+_y, p1i[2]+_z)
                    if p1j not in sp:
                        continue
                    for j in sp[p1j]:
                        if j > i and np.linalg.norm(joints[j]-joints[i]) <= L:
                            members.append((i, j))
    return sorted(members)


def clean_planar_0(joints, members):
    dirs = [[] for _ in range(len(joints))]
    for a, b in members:
        d = joints[b] - joints[a]
        d /= np.linalg.norm(d)
        dirs[a].append(d)
        dirs[b].append(d)
    maps = []
    good_count = 0
    for i in range(len(joints)):
        dirs[i] = np.array(dirs[i])
        eigs = np.linalg.eigvalsh(dirs[i].T@dirs[i])
        if np.amin(eigs)/np.amax(eigs) < 1e-6:  # coplanar
            maps.append(-1)
        else:
            maps.append(good_count)
            good_count += 1
    joints = [joints[i] for i in range(len(joints)) if maps[i] != -1]
    members = [(maps[a], maps[b]) for a, b in members \
                   if maps[a] != -1 and maps[b] != -1]
    return np.array(joints), members, maps


def clean_planar(joints, members):
    """Get rid of joints with only planar connections
        members must be 0-indexed"""
    joints1, members1, maps = clean_planar_0(joints, members)
    while len(joints1) != len(joints):
        joints, members = joints1, members1
        joints1, members1, maps = clean_planar_0(joints, members)
    return joints1, members1
        


def quad_base(fun, x0, x1, y0, y1, z0, z1):
    """Square-based pyramid lattice with equal member lengths
        x -> x + 1/2 z
        y -> y + 1/2 z
        z -> sqrt(2)/2 z
    """
    from math import floor, ceil
    joints = []
    r22 = 2**.5/2
    for zi in range(int(floor(z0/r22))+1, int(ceil(z1/r22))):
        z = zi * r22
        for xi in range(int(floor(x0-zi/2))+1, int(ceil(x1-zi/2))):
            x = xi + zi/2
            for yi in range(int(floor(y0-zi/2))+1, int(ceil(y1-zi/2))):
                y = yi + zi/2
                if fun(x, y, z) < 0:
                    joints.append((x, y, z))
    return np.array(joints)


def tri_base(fun, x0, x1, y0, y1, z0, z1):
    """Triangle-based pyramid lattice with equal member lengths
        x -> x + 1/2 y + 1/2 z
        y -> sqrt(3)/2 y + sqrt(3)/6 z
        z -> sqrt(6)/3 z
    """
    from math import floor, ceil
    joints = []
    r32, r36, r63 = 3**.5/2, 3**.5/6, 6**.5/3
    for zi in range(int(floor(z0/r63))+1, int(ceil(z1/r63))):
        z = zi * r63
        for yi in range(int(floor((y0-r36*zi)/r32))+1, int(ceil((y1-r36*zi)/r32))):
            y = r32 * yi + r36 * zi
            for xi in range(int(floor(x0-(yi+zi)/2))+1, int(ceil(x1-(yi+zi)/2))):
                x = xi + yi/2 + zi/2
                if fun(x, y, z) < 0:
                    joints.append((x, y, z))
    return np.array(joints)


## Trusses

def sample_1():
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
    fixed = [1, 2, 3, 6]
    members = [
        (1, 2), (2, 3), (1, 4), (2, 5), (3, 6), (4, 5), (5, 6),
        (1, 7), (2, 7), (4, 7), (5, 7), (2, 8), (3, 8), (5, 8), (6, 8), (7, 8),
        #(1, 8), (4, 8), (3, 7), (6, 7)
    ]
    EA = 2e5 * 300
    return _return(joints, weights, fixed, members, EA)


def roof():
    """#1
        cond = 25.081783770379758
        16 joints (10 fixed, 6 nonfixed)
        39 members
        Dead load range: 520 2311
        Applied load range: 6000 12000
        Max reaction: 11340
        Member force range: 0 -9679
        Stress range: 0 -6.45
        Max slenderness: 355
        Min buckle FoS: 7.25
        Max deflection: 0.321
    """
    N, L0, L1, W, H = 2, 12000, 3000, 3000, 2000
    joints = []
    weights = []
    fixed = []
    members = []
    for i in range(0, N):
        y0 = i*L0
        y1 = (i-(N-1)/2)*(L0+L1)/2+(N-1)/2*L0
        y2 = (i-(N-1)/2)*L1+(N-1)/2*L0
        joints += [
            (-2*W, y0, 0),
            (-W, y0, 0),
            (0, y0, 0),
            (W, y0, 0),
            (2*W, y0, 0),
            (-W, y1, H),
            (W, y1, H),
            (0, y2, 2*H)
        ]
        weights += [0, 0, 0, 0, 0, 6000, 6000, 12000]
        i0 = 8*i
        fixed += [i0, i0+1, i0+2, i0+3, i0+4]
        members += [
            (i0, i0+1), (i0+1, i0+2), (i0+2, i0+3), (i0+3, i0+4),
            (i0, i0+5), (i0+5, i0+7), (i0+7, i0+6), (i0+6, i0+4),
            (i0+1, i0+5), (i0+2, i0+7), (i0+3, i0+6),
            (i0+2, i0+5), (i0+2, i0+6),
        ] + (i != N-1) * ([
            (i0, i0+8), (i0+4, i0+12),
            (i0+5, i0+13), (i0+6, i0+14), (i0+7, i0+15),
            (i0+4, i0+14), (i0+6, i0+15), (i0, i0+13), (i0+5, i0+15),
            (i0+6, i0+12), (i0+7, i0+14), (i0+5, i0+8), (i0+7, i0+13)
        ])
    EA = 2e5 * 1500
    return _return(joints, weights, fixed, members, EA)


def beam():
    """#2
        cond = 812.0758601627853
        44 joints (13 fixed, 31 nonfixed)
        128 members
        Dead load range: 919 1991
        Applied load range: 3000 34000
        Max reaction: 173297
        Member force range: 101989 -133514
        Stress range: 40.8 -53.4
        Max slenderness: 89.6
        Min buckle FoS: 5.7
        Max deflection: 5.06
    """
    N, W, Bb, Bt, H = 11, 2000, 1500, 1000, 2500
    joints = []
    weights = []
    fixed = []
    members = []
    for i in range(N):
        joints += [
            (W*i, Bb, 0),
            (W*i, Bt, H),
            (W*i, -Bt, H),
            (W*i, -Bb, 0)
        ]
        weights += [20000+4000*(i-N/2), 3000, 3000, 20000-4000*(i-N/2)]
        fixed += [4*i, 4*i+3] * (i < 2 or i >= N-2) + \
                 [4*i+1, 4*i+2] * (i < 1 or i >= N-1) + \
                 [4*i] * (i == N//2+1)
        members += [
            (4*i, 4*i+1),
            (4*i+1, 4*i+2),
            (4*i+2, 4*i+3),
            (4*i+3, 4*i)
        ] + (i != N-1) * ([
            (4*i, 4*i+4),
            (4*i+1, 4*i+5),
            (4*i+2, 4*i+6),
            (4*i+3, 4*i+7),
        ] + (i < (N-1)/2) * [
            (4*i, 4*i+5), (4*i+3, 4*i+6)
        ] + (i >= (N-1)/2) * [
            (4*i+1, 4*i+4), (4*i+2, 4*i+7)
        ] + (i % 2 == 0) * [
            (4*i+1, 4*i+6), (4*i+3, 4*i+4)
        ] + (i % 2 == 1) * [
            (4*i+2, 4*i+5), (4*i, 4*i+7)
        ]) + (i == 0 or i == N-1) * [
            (4*i, 4*i+2), (4*i+1, 4*i+3)
        ]
    EA = 2e5 * 2500
    return _return(joints, weights, fixed, members, EA)


def cantilever():
    """#3
        cond = 36091.12536523044
        25 joints (3 fixed, 22 nonfixed)
        90 members
        Dead load range: 31.6 405
        Applied load range: 500 20000
        Max reaction: 120196
        Member force range: 57976 -116100
        Stress range: 58 -116.1
        Max slenderness: 84.4
        Min buckle FoS: 6.33
        Max deflection: 21.1
    """
    N, W, H, L = 8, 600, 1500, 6000
    joints = []
    weights = []
    fixed = [0, 1, 2]
    members = []
    for i in range(N):
        w = W*((N-i)/N)**1.2
        l = L*(1-((N-i)/N)**1.5)
        h = H*(1-(i/N)**0.5)
        joints += [
            (w, l, 0),
            (-w, l, 0),
            (0, l, -h)
        ]
        weights += [500, 500, 500]
        members += [
            (3*i, 3*i+1), (3*i+1, 3*i+2), (3*i+2, 3*i)
        ] + (i < N-1) * [
            (3*i, 3*i+3), (3*i+1, 3*i+4), (3*i+2, 3*i+5),
            (3*i, 3*i+4), (3*i+1, 3*i+3),
            (3*i+1, 3*i+5), (3*i+2, 3*i+4),
            (3*i, 3*i+5), (3*i+2, 3*i+3)
        ] + (i == N-1) * [
            (3*i, 3*N), (3*i+1, 3*N), (3*i+2, 3*N)
        ]
    joints += [(0, L, 0)]
    weights += [20000]
    EA = 2e5 * 1000
    return _return(joints, weights, fixed, members, EA)


def polyhedra():
    """#4
        cond = 55.59049164475541
        50 joints (18 fixed, 32 nonfixed)
        165 members
        Dead load range: 23.1 83.2
        Applied load range: 1000 1000
        Max reaction: 6748
        Member force range: 2905 -2907
        Stress range: 14.52 -14.54
        Max slenderness: 168.3
        Min buckle FoS: 5.37
        Max deflection: 0.299
    """
    r22, r32, r33, r36, r63 = 2**.5/2, 3**.5/2, 3**.5/3, 3**.5/6, 6**.5/3
    tetra = np.array([
        (0, 0, 0),
        (1, 0, 0),
        (0.5, r32, 0),
        (0.5, r36, r63)
    ]) + (-0.5, -r36, 0)
    cube = np.array([
        (0, 0, 0), (1, 0, 0), (0, 1, 0), (0, 0, 1),
        (0, 1, 1), (1, 0, 1), (1, 1, 0), (1, 1, 1)
    ]) + (-0.5, -0.5, 0) + (3, 0, 0)
    octa = np.array([
        (0, 0, 0), (1, 0, 0), (0.5, r32, 0),
        (0, r33, r63), (1, r33, r63), (0.5, -r36, r63)
    ]) + (-0.5, -r36, 0) + (0, 3, 0)
    phi = np.sin(0.3*np.pi)
    icosa = np.array([
        (0, 0.5, phi), (0, 0.5, -phi), (0, -0.5, phi), (0, -0.5, -phi),
        (0.5, phi, 0), (0.5, -phi, 0), (-0.5, phi, 0), (-0.5, -phi, 0),
        (phi, 0, 0.5), (-phi, 0, 0.5), (phi, 0, -0.5), (-phi, 0, -0.5)
    ]) + (0, 0, phi) + (3, 3, 0)
    phi, psi = (5**.5+1)/2, (5**.5-1)/2
    dodeca = (np.array([
        (1, 1, 1), (1, 1, -1), (1, -1, 1), (1, -1, -1),
        (-1, 1, 1), (-1, 1, -1), (-1, -1, 1), (-1, -1, -1),
        (0, phi, psi), (0, phi, -psi), (0, -phi, psi), (0, -phi, -psi),
        (psi, 0, phi), (psi, 0, -phi), (-psi, 0, phi), (-psi, 0, -phi),
        (phi, psi, 0), (phi, -psi, 0), (-phi, psi, 0), (-phi, -psi, 0)
    ]) + (0, 0, phi)) * ((5**.5+3)/8)**.5 * 0.85 + (6.5, 1.5, 0)
    joints = np.concatenate([tetra, cube, octa, icosa, dodeca])
    members = connect_members(joints, 1.42)
    fixed = [0, 1, 2, 4, 5, 6, 10, 12, 13, 14, 22, 23, 24, 25,
             30, 31, 32, 33]
    weights = [1000]*len(joints)
    joints *= 1000
    EA = 2e5 * 200
    return _return(joints, weights, fixed, members, EA)


def quad_shell():
    """#5
        cond = 879.69496780939
        137 joints (4 fixed, 133 nonfixed)
        480 members
        Dead load range: 897 2547
        Applied load range: 210 3790
        Max reaction: 117926
        Member force range: 25069 -59444
        Stress range: 12.53 -29.7
        Max slenderness: 138.6
        Min buckle FoS: 4.32
        Max deflection: 3.94
    """
    joints = quad_base(
        lambda x, y, z: max(abs(x)-5.1, abs(y)-3.1, abs(z+0.4)-0.5),
        -5.6, 5.6, -3.6, 3.6, -1.0, 0.1)
    members = connect_members(joints, 1.01)
    joints[:, 2] -= 0.05*joints[:, 0]**2 + 0.03*joints[:, 1]**2
    joints[:, 2] -= np.amin(joints[:, 2])
    weights = [2000+10*p[0]**3+20*p[1]**3 for p in joints]
    joints *= 4000
    fixed = np.where(joints[:, 2] < np.amin(joints[:, 2])+1)[0]
    EA = 2e5 * 2000
    return _return(joints, weights, fixed, members, EA)


def inverted_double_pyramid():
    """#6
        cond = 5115.636634797697
        186 joints (8 fixed, 178 nonfixed)
        778 members
        Dead load range: 231 924
        Applied load range: 1003 19198
        Max reaction: 428409
        Member force range: 78998 -224731
        Stress range: 39.5 -112.4
        Max slenderness: 28.6
        Min buckle FoS: 21.6
        Max deflection: 7.07
    """
    joints = quad_base(
        lambda x, y, z: max(max(abs(abs(x)-2), abs(y)) - .5**.5 * z, \
                            z-0.2*x-0.1*y-4),
        -6.1, 6.1, -4.1, 4.1, -0.1, 5.)
    members = connect_members(joints, 1.01)
    joints, members = clean_planar(joints, members)
    weights = [1000+0.25*p[2]**7 for p in joints]
    joints *= 1000
    fixed = np.where(joints[:, 2] < np.amin(joints[:, 2])+1)[0]
    EA = 2e5 * 2000
    return _return(joints, weights, fixed, members, EA)


def suzanne():
    """#7
        cond = 48393111.615607224
        505 joints (45 fixed, 460 nonfixed)
        1473 members
        Dead load range: 1.616 24.9
        Applied load range: -2217 10029
        Max reaction: 5458
        Member force range: 9552 -14524
        Stress range: 47.8 -72.6
        Max slenderness: 79.7
        Min buckle FoS: 11.7
        Max deflection: 70.6
    """
    joints, fixed, members, weights = stlread("models/blender_suzanne.stl")
    joints *= 1000
    weights *= 20
    fixed += np.where(abs(joints[:, 0]) > np.amax(joints[:, 0])-1)[0].tolist()
    fixed += np.where(joints[:, 1] > np.amax(joints[:, 1])-1)[0].tolist()
    bottoms = np.where(joints[:, 2] < np.amin(joints[:, 2])+40)[0]
    for w in bottoms:
        weights[w] -= 20000/len(bottoms)
    weights[np.where(joints[:, 2] > np.amax(joints[:, 2])-1)[0][0]] += 10000
    EA = 2e5 * 200
    return _return(joints, weights, fixed, members, EA)


def double_tower():
    """#8
        cond = 85880.96256113035
        1258 joints (36 fixed, 1222 nonfixed)
        5327 members
        Dead load range: 22470 107104
        Applied load range: 24529 98755
        Max reaction: 3021030
        Member force range: 2150336 -2302609
        Stress range: 107.5 -115.1
        Max slenderness: 88.8
        Min buckle FoS: 3
        Max deflection: 218
    """
    def fun(x, y, z):
        tower = max(abs(abs(x)-9.5)-1.6,
                    abs(max(abs(y)-2.25, 1.0*(z-0.15*x+0.3*y-8)))-1.3)
        road = max(abs(y)-2, abs(z)-1)
        return min(tower, road)
    joints = quad_base(fun, -19.1, 17.1, -4.1, 4.1, -5., 12.)
    members = connect_members(joints, 1.01)
    joints, members = clean_planar(joints, members)
    weights = [50000+4000*p[2]+1000*p[1]+500*p[0] for p in joints]
    joints[:, 1] /= 1 + 0.03*joints[:, 2] - 0.01*joints[:, 0] + 0.02*joints[:, 1]
    joints *= 10000
    fixed = np.where(joints[:, 2] < np.amin(joints[:, 2])+1)[0]
    EA = 2e5 * 20000
    return _return(joints, weights, fixed, members, EA)


def effiel_tower():
    """#9
        cond = 457583.98835728545
        2047 joints (100 fixed, 1947 nonfixed)
        9832 members
        Dead load range: 5775 23101
        Applied load range: 20000 4000000
        Max reaction: 1783054
        Member force range: 746342 -1202983
        Stress range: 74.6 -120.3
        Max slenderness: 69.8
        Min buckle FoS: 4.95
        Max deflection: 71.1
    """
    def fun(x, y, z):
        x, y = abs(x), abs(y)
        r22 = .5**.5
        base = max(abs(x+z*r22-8.), abs(y+z*r22-8.))-2.01
        body = max(x-3.5+0.1*(z-9*r22), y-3.5+0.1*(z-9*r22), 8.99*r22-z-.1, z-45.01*r22)
        return min(base, body)
    joints = quad_base(fun, -10.1, 10.1, -10.1, 10.1, -0.1, 50)
    ph = joints[np.where(joints[:, 2] > np.amax(joints[:, 2])-0.0001)[0]]
    more_joints = []
    for i in range(1, 8):
        s = 1-i/7
        for p in ph:
            more_joints.append((p[0]*s, p[1]*s, p[2]+0.9*i**1.1))
            if s == 0.0:
                break
    joints = np.concatenate([joints, more_joints])
    weights = [20000 for p in joints]
    weights[-1] = 4000000
    members = connect_members(joints, 1.4)
    joints *= 5000
    fixed = np.where(joints[:, 2] < np.amin(joints[:, 2])+1)[0]
    EA = 2e5 * 10000
    return _return(joints, weights, fixed, members, EA)


def torch_tower():
    """#10
        cond = 37643672.57528001
        2261 joints (6 fixed, 2255 nonfixed)
        9722 members
        Dead load range: 1155 4620
        Applied load range: 1000 1000
        Max reaction: 1494711
        Member force range: 2584909 -2001101
        Stress range: 258 -200
        Max slenderness: 10.53
        Min buckle FoS: 89.1
        Max deflection: 359
    """
    from math import sqrt, sin, cos, asin, atan2, pi
    def fun(x, y, z):
        r63 = 6**.5/3
        x, y = x-0.5, y-3**.5/6
        r = sqrt(x**2+y**2)
        stem = r - 1.5
        a0 = atan2(y, x)
        a = asin(sin(3*a0))/3
        x, y = r*cos(a), r*sin(a)
        branch = max(abs(y)-0.8, abs(x)-12, abs(z-20.5*r63)-0.51*r63)
        ring = max(abs(r-12+2*sin(3*a0)+a0)-1,
                   abs(z-20.*r63+sin(a0))-2.01*r63-3*cos(a0)**2)
        ball = sqrt(r*r+(z-25)**2)-5
        return min(stem, branch, ring, ball)
    joints = tri_base(fun, -15, 15, -15, 15, -0.1, 30)
    members = connect_members(joints, 1.01)
    joints, members = clean_planar(joints, members)
    weights = [1000 for p in joints]
    joints *= 1000
    fixed = np.where(joints[:, 2] < np.amin(joints[:, 2])+1)[0]
    EA = 2e5 * 10000
    return _return(joints, weights, fixed, members, EA)


def storeys():
    """#11
        cond = 299328.32191458234
        4102 joints (32 fixed, 4070 nonfixed)
        17662 members
        Dead load range: 277 1109
        Applied load range: 1092 5015
        Max reaction: 555513
        Member force range: 147863 -295470
        Stress range: 37 -73.9
        Max slenderness: 11.15
        Min buckle FoS: 215
        Max deflection: 5.54
    """
    def fun(x, y, z):
        r36, r63 = 3**.5/6, 6**.5/3
        p1 = ((abs(x)-10.5)**2 + (abs(y)-r36)**2)**.5 - 1.5
        p2 = (x**2 + (abs(y)-(4*3**.5+r36))**2)**.5 - 1.5
        sb = abs(x)**1.5+(1.3*abs(y))**1.5-15**1.5
        f1 = max(min(abs(z-8.5*r63), abs(z-16.5*r63), abs(z-24.5*r63))-0.5, sb)
        return min(p1, p2, f1)
    joints = tri_base(fun, -15, 15, -15, 15, -0.1, 25)
    members = connect_members(joints, 1.01)
    joints, members = clean_planar(joints, members)
    weights = [1000 + 2000*np.exp(-(p[0]**2+p[1]**2)/5**2) + 100*p[2] for p in joints]
    joints *= 600
    fixed = np.where(joints[:, 2] < np.amin(joints[:, 2])+1)[0]
    EA = 2e5 * 4000
    return _return(joints, weights, fixed, members, EA)


def lantern():
    """#12
        cond = 89993701.58396149
        1200 joints (48 fixed, 1152 nonfixed)
        4656 members
        Dead load range: 1.394 3.78
        Applied load range: 10 81
        Max reaction: 1519
        Member force range: 2953 -3277
        Stress range: 29.5 -32.8
        Max slenderness: 26.9
        Min buckle FoS: 185.6
        Max deflection: 48.9
    """
    from math import sin, cos, pi
    N, M = 24, 48
    joints = []
    weights = []
    members = []
    for i in range(N+1):
        for j in range(M):
            v, u = 0.8+1.8*i/N, 2*pi*j/M
            joints.append((1000*cos(u)*sin(v), 800*sin(u)*sin(v), 900*cos(v)))
            members += [
                (i*M+j, i*M+(j+1)%M),
            ] + (i != N) * ([
                (i*M+j, i*M+j+M),
                (i*M+j, i*M+(j+1)%M+M),
                (i*M+(j+1)%M, i*M+j+M)
            ])
            weights.append(10 + (1000/M*(sin(2*u)+cos(2*u)+2)) * (i == N))
    fixed = list(range(M))
    EA = 2e5 * 100
    return _return(joints, weights, fixed, members, EA)
