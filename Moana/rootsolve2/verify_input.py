import os
import cmath

outs = [f for f in os.listdir("data/") if f.endswith('.in')]


for fn in outs:
    print(fn)
    content = open("data/"+fn).read()
    lines = content.strip().split('\n')
    assert len(lines) == 3
    assert 'e' not in lines[1]
    assert 'e' not in lines[2]
    N = int(lines[0])
    assert 1 <= N <= 1000
    a = list(map(float, lines[1].split()))
    assert len(a) == N+1
    b = list(map(float, lines[2].split()))
    assert len(b) == N+1
    assert b[0] == 0
    assert not (a[N] == 0 and b[N] == 0)
