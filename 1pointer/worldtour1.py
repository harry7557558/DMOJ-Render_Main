N = int(input())

for i in range(N):
    s = input()
    if s[0] in 'Bb': print('m'+s)
    elif s[0] in 'Cc': print('g'+s)
    elif s[0] in 'Ff': print('bh'+s)
    elif s[0] in 'Pp': print('b'+s)
    elif s[0] in 'Tt': print('d'+s)
    elif s[0] in 'AEIOUÁÉÍÓÚGgDd': print('n'+s)
    elif s[0] in 'aeiouáéíóú': print('n-'+s)
    else: print(s)
