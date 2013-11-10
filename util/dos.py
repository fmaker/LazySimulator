import os
import sys
import math
import csv

# Grab file name
FILE = sys.argv[1]

# Read in list
reader = csv.reader(open(FILE,"rb"),delimiter=' ')
header = reader.next()
percent = float(header[1])
values = []
for row in reader:
    values.append(float(row[0]));

# Get min and max
MIN = min(values)
MAX = max(values)


# Generate target bins
BINS = [(MIN*(1-percent),MIN*(1+percent))] # First bin is min +/- percent
binTop = None
binBottom = None
while binTop < MAX:
    binBottom = BINS[-1][1] # Last max is bottom of next bin
    nextX = binBottom / (1-percent) # 
    binTop = nextX * (1+percent)  # Calculate top of next bin
    BINS.append((binBottom,binTop))

# Count hits in each bin
BIN_COUNTS = [0] * len(BINS)
for v in values:
    for i in range(len(BINS)):
        (min,max) = BINS[i]
        if(v >= min and v <max):
            BIN_COUNTS[i] += 1
            break
        
# Make sure each value is accounted for
assert(sum(BIN_COUNTS) == len(values))

# Plot bin value versus frequency of hits
writer = csv.writer(open("/tmp/dos.dat","w"),delimiter=' ')
for i in range(len(BINS)):
    value = sum(BINS[i])/len(BINS[i])
    writer.writerow([value,BIN_COUNTS[i]])
    
