import os
import cmath

outs = [f for f in os.listdir("data/") if f.endswith('.out')]


for out in outs:
    print(out)
    content = open("data/"+out).read()
    xs = []
    for line in content.strip().split('\n'):
        re, im = map(float, line.split())
        assert abs(im) < 2
        x = cmath.exp(1j*(re + im * 1j))
        assert 1e-1 < abs(x) < 1e1
        xs.append(x)
    mindif = 100
    for i in range(len(xs)):
        for j in range(len(xs)):
            if i == j:
                continue
            dif = abs(xs[j]-xs[i])
            mindif = min(mindif, dif)
    assert mindif > 1e-4
