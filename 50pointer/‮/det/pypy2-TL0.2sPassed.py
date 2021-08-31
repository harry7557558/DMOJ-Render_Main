import sys
N = int(sys.stdin.readline())
M = [[int(t) for t in sys.stdin.readline().split()] for i in range(N)]
_ = 1000000007
d = 1
for i in range(N):
    P=M[i]
    if P[i]==0:
        for j in range(i+1,N):
            if M[j][i]!=0:
                for k in range(i,N):
                    t = M[i][k]
                    M[i][k]=M[j][k]
                    M[j][k]=t
                d=-d
                break
    v = -pow(P[i],_-2,_)
    for j in range(i+1,N):
        Q=M[j]
        if Q[i]:
            m=(Q[i]*v)%_
            for k in range(i+1,N):
                Q[k]=(P[k]*m+Q[k])%_
    d=(P[i]*d)%_
print((d+_)%_)