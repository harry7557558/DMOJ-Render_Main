# Reference solution
# This problem can be fun for optimization and code-golf

N = int(input())

Sum = 0
for i in range(1,N+1):
    Sum += i**6

print(Sum % 10**9)


