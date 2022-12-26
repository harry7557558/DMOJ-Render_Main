import trusses
from solver_1 import format_float, solve_truss

cases = [
    trusses.sample_1,
    trusses.roof,
    trusses.beam,
    trusses.cantilever,
    trusses.polyhedra,
    trusses.quad_shell,
    trusses.inverted_double_pyramid,
    trusses.suzanne,
    trusses.double_tower,
    trusses.effiel_tower,
    trusses.torch_tower,
    trusses.storeys,
    trusses.lantern
]

for ci, case in zip(range(len(cases)), cases):
    ci = "sample" if ci == 0 else "{:02d}".format(ci)
    print(ci)
    joints, weights, fixed, members, EA = case()

    fp = open(f"data/{ci}.in", 'w')
    print("N, S, M, EA:", len(joints), len(fixed), len(members), EA)
    print(len(joints), len(fixed), len(members), EA, file=fp)
    maxb, minw, maxw = 0, float('inf'), float('-inf')
    for (x, y, z), w in zip(joints, weights):
        print(x, y, z, w, file=fp)
        maxb = max(maxb, abs(x), abs(y), abs(z))
        minw, maxw = min(minw, w), max(maxw, w)
    print("maxb, minw, maxw:", maxb, minw, maxw)
    print(' '.join(map(str, fixed)), file=fp)
    print(' '.join(map(str, [a for a, b in members])), file=fp)
    print(' '.join(map(str, [b for a, b in members])), file=fp)
    fp.close()

    fa, fm, u = solve_truss(joints, weights, fixed, members, EA, 'a')
    fp = open(f"data/{ci}.out", 'w')
    print(' '.join([format_float(f, s=6) for f in fm]), file=fp)
    maxfm = format_float(max(abs(fm)), s=6)
    print("maxfm:", maxfm)
    fp.close()


import zipfile
import os
with zipfile.ZipFile("data.zip", 'w', zipfile.ZIP_DEFLATED) as zipf:
    for file in os.listdir("data/"):
        content = open('data/'+file, 'r').read()
        content = content.replace('\r\n', '\n')
        content = content.strip() + '\n'
        open('data/'+file, 'wb').write(bytearray(content, 'ascii'))
        zipf.write("data/"+file, file)
