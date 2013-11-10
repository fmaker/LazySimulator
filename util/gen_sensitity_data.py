#!/usr/bin/python
import sys,csv


FILE = sys.argv[1]

reader = csv.reader(open(FILE,'r'),delimiter=' ')
header = reader.next()

# def translate(colname):
#     print "Translating: ",colname
#     colname = colname.replace("random_greedy","Random Greedy")
#     print "Translated:  ",colname,"\n"
#     return colname

# Tuple format
# (algorithm, limit)
cols = []

for h in header[1:]: # Skip "Device"
    col = h.split('=')
    col[0] = col[0].split("+limit")[0]
    cols.append(col)
print cols
print

# Tuple format
# (value, (algorithm, limit))
data = []
for row in reader:
    i = 0
    for value in row[1:]:
        if (row == 0):
            continue  # device name "all_cam_configs"
        data.append((value,cols[i]))

        i+=1

print data
