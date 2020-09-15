# example solution

s = open(False).read()
counts = []

for i in range(26):
    uppercase_count = s.count(chr(i+65))
    lowercase_count = s.count(chr(i+97))
    count = uppercase_count + lowercase_count
    counts.append(str(count))

print(' '.join(counts))


