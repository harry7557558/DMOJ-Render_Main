import sys
import subprocess
import math

_SEED = 0
RAND_MAX = 2**24
def srand(seed):
    global _SEED
    seed = str(seed)
    h, p = 1013904223, 1
    for c in seed:  # rolling hash
        h = (h + ord(c)) * p % RAND_MAX
        p = p * 31 % RAND_MAX
    _SEED = h
def randu():  # uniform integer less than RAND_MAX
    global _SEED
    _SEED = (_SEED * 1664525 + 1013904223) % (2**32)
    return _SEED >> 8
def randf():  # uniform float from 0 to 1
    return randu()/RAND_MAX
def randn():  # standard normal distribution
    return math.sqrt(-2.0*math.log(1.0-randf()))*math.cos(2.0*math.pi*randf())
def shuffle(x):
    for i in reversed(range(1, len(x))):
        j = randu() % (i+1)
        x[i], x[j] = x[j], x[i]


def rect_grid(W, H, MAXR, more_wire):
    N = W*H
    rs = []
    for i in range(W-1):
        for j in range(H):
            r = randu() % MAXR + 1
            rs.append((i*H+j, (i+1)*H+j, r))
    for i in range(W):
        for j in range(H-1):
            r = randu() % MAXR + 1
            rs.append((i*H+j, i*H+j+1, r))
    assert len(rs) == (W-1)*H + (H-1)*W
    for i in range(max(2, int(more_wire*len(rs)))):
        a = randu() % N
        b = randu() % N
        while a == b:
            b = randu() % N
        rs.append((a, b, randu() % MAXR + 1))
    print(N, len(rs), int(MAXR*(W*H)**0.5*(1+0.2*randf())))
    mp = list(range(1, N+1))
    shuffle(mp)
    shuffle(rs)
    for a, b, r in rs:
        print(mp[a], mp[b], r)

def torus_grid(W, H, MAXR, more_wire):
    N = W*H
    rs = []
    for i in range(W):
        for j in range(H):
            r = randu() % MAXR + 1
            rs.append((i*H+j, (i+1)%W*H+j, r))
    for i in range(W):
        for j in range(H):
            r = randu() % MAXR + 1
            rs.append((i*H+j, i*H+(j+1)%H, r))
    assert len(rs) == 2*W*H
    for i in range(max(2, int(more_wire*len(rs)))):
        a = randu() % N
        b = randu() % N
        while a == b:
            b = randu() % N
        rs.append((a, b, randu() % MAXR + 1))
    print(N, len(rs), int(MAXR*(W*H)**0.5*(1+0.2*randf())))
    mp = list(range(1, N+1))
    shuffle(mp)
    shuffle(rs)
    for a, b, r in rs:
        print(mp[a], mp[b], r)

def torus_grid_3d(D1, D2, D3, MAXR, more_wire):
    N  = D1*D2*D3
    rs = []
    for i in range(D1):
        for j in range(D2):
            for k in range(D3):
                r = randu() % MAXR + 1
                rs.append(((i*D2+j)*D3+k, ((i+1)%D1*D2+j)*D3+k, r))
                r = randu() % MAXR + 1
                rs.append(((i*D2+j)*D3+k, (i*D2+(j+1)%D2)*D3+k, r))
                r = randu() % MAXR + 1
                rs.append(((i*D2+j)*D3+k, (i*D2+j)*D3+(k+1)%D3, r))
    for i in range(max(2, int(more_wire*len(rs)))):
        a = randu() % N
        b = randu() % N
        while a == b:
            b = randu() % N
        rs.append((a, b, randu() % MAXR + 1))
    print(N, len(rs), int(MAXR*len(rs)**0.5*(1+1*randf())))
    mp = list(range(1, N+1))
    shuffle(mp)
    shuffle(rs)
    for a, b, r in rs:
        print(mp[a], mp[b], r)

def dense_neural(layers, MAXR, more_wire):
    N = sum(layers)
    rs = []
    for li in range(len(layers)-1):
        D1, D2 = layers[li], layers[li+1]
        s1, s2 = sum(layers[:li]), sum(layers[:li+1])
        for a in range(D1):
            for b in range(D2):
                r = randu() % MAXR + 1
                rs.append((s1+a, s2+b, r))
    for i in range(int(more_wire*len(rs))):
        a = randu() % N
        b = randu() % N
        while a == b:
            b = randu() % N
        rs.append((a, b, randu() % MAXR + 1))
    print(N, len(rs), int(MAXR*len(rs)**0.5*(1+1*randf())))
    mp = list(range(1, N+1))
    shuffle(mp)
    shuffle(rs)
    for a, b, r in rs:
        print(mp[a], mp[b], r)


def strand(N, MAXR, variance, more_wire):
    assert N > 10
    rs = []
    for l in range(N-1):
        r = randu() % MAXR + 1
        rs.append((l, l+1, r))
    for i in range(int(more_wire*len(rs))):
        a = randu() % N
        b = a
        while a == b or b < 0 or b >= N:
            b = round(a+variance*randn())
        rs.append((a, b, randu() % MAXR + 1))
    print(N, len(rs), int(MAXR*len(rs)**0.5*(1+1*randf())))
    mp = list(range(2, N))
    shuffle(mp)
    mp = [1] + mp + [N]
    shuffle(rs)
    for a, b, r in rs:
        print(mp[a], mp[b], r)
        

def run_case(case_id, fun):
    print("Case", case_id)
    old_stdout = sys.stdout
    fp = open('data/'+case_id+'.in', 'w')
    sys.stdout = fp
    fun()
    fp.close()
    sys.stdout = old_stdout
    proc = subprocess.Popen(
        ['python', 'refsol.py'],
        stdin=open('data/'+case_id+'.in', 'r'),
        stdout=subprocess.PIPE
    )
    srand(case_id)
    output = proc.communicate()[0]
    open('data/'+case_id+'.out', 'wb').write(output)

if __name__ == "__main__":
    run_case('01', lambda: rect_grid(2, 4, 1000, 0.05))
    run_case('02', lambda: torus_grid(2, 7, 2000, 0.2))
    run_case('03', lambda: torus_grid(5, 5, 2000, 0.3))
    run_case('04', lambda: rect_grid(4, 9, 100000, 1.1))
    run_case('05', lambda: strand(110, 100000, 10.0, 1.1))
    run_case('06', lambda: torus_grid_3d(2, 3, 2, 100000, 6.0))
    run_case('07', lambda: torus_grid_3d(2, 3, 4, 100000, 4.0))
    run_case('08', lambda: dense_neural([2, 20, 20, 3], 100000, 0.0))
    run_case('09', lambda: rect_grid(7, 9, 100000, 5.5))
    run_case('10', lambda: torus_grid(9, 11, 100000, 5.5))
    run_case('11', lambda: dense_neural([30, 35, 32], 100000, 0.577))
    run_case('12', lambda: dense_neural([45, 32, 43], 100000, 0.871))
    run_case('13', lambda: torus_grid_3d(3, 3, 3, 100000, 67.3))
    run_case('14', lambda: torus_grid_3d(3, 4, 5, 100000, 53.1))
    run_case('15', lambda: torus_grid_3d(5, 5, 5, 100000, 53.3))
    run_case('16', lambda: dense_neural([20, 40, 60, 50, 20, 140], 100000, 2.0))
    run_case('17', lambda: rect_grid(56, 47, 1000, 0.05))
    run_case('18', lambda: torus_grid(45, 54, 10000, 0.05))
    run_case('19', lambda: strand(4356, 100000, 50.0, 1.1))
    run_case('20', lambda: strand(7125, 100000, 10.0, 2.33))
