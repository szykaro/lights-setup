ref = [33, 0]
add = [0, 1]
lamps = [[2, 0], [4, 0], [8, 0], [16, 0], [0, 2], [0, 4], [0, 8], [0, 32]]
null = [0, 0]
seq = []
for i, s1 in enumerate(lamps):
    seq.append(ref)
    seq.append([s1[0] + add[0], s1[1] + add[1]])
    seq.append(null)
    for j, s2 in enumerate(lamps):
        if i > j:
            seq.append(ref)
            seq.append([s1[0] + s2[0] + add[0], s1[1] + s2[1] + add[1]])
            seq.append(null)
        for k, s3 in enumerate(lamps):
            if (i > j) and (j > k):
                seq.append(ref)
                seq.append([s1[0] + s2[0] + s3[0] + add[0], s1[1] + s2[1] + s3[1] + add[1]])
                seq.append(null)

print(len(seq))
c_seq = str(seq).replace('[', '{').replace(']', '}')
print(c_seq)