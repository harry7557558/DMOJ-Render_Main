# example solution

N = int(input())

S = 0
for i in range(1,N+1):
    S += i**6

print(S % 10**9)


