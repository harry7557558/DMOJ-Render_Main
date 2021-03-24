# calculate the maximum possible points of a single User
# also print a point distribution statistics


# get data

import requests
import json

problems = []

for page_index in range(1, 100):
    url = f'https://dmoj.ca/api/v2/problems?page={page_index}'
    print("Request", url)
    r = requests.get(url)
    parsed = json.loads(r.content.decode())
    problems += parsed['data']['objects']
    if not parsed['data']['has_more']:
        break

print()


# print problem point distribution

points_list = {}

for problem in problems:
    point = problem['points']
    if point in points_list:
        points_list[point] += 1
    else:
        points_list[point] = 1

print("point\tcount\tgraph")
for point,count in sorted(points_list.items()):
    print(int(point), end='\t\t')
    print(count, end='\t\t')
    print('*'*((count+4)//8))
print()


# calculate maximum possible points

S = []

for problem in problems:
    S.append(problem['points'])

S.sort(reverse=True)

P = sum([0.95**i * S[i] for i in range(100)])
B = 150 * (1-0.997**len(S))

print("Maximum possible point for a single user:")
print("P + B = {:.2f} + {:.2f} = {:.2f}".format(P, B, P+B))

