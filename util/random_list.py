#!/usr/bin/env python
# Generate random list for targets.h

import sys
import random

def random_list(n):
    list = []
    for i in range(n):
        list.append(random.randint(0,65535))
    return list

runs = int(sys.argv[1])
targets = int(sys.argv[2])
output = sys.argv[3]

f = open(output,'w')

f.write("#ifndef TARGETS_H_\n")
f.write("#define TARGETS_H_\n\n")
f.write("const unsigned int int_constants[]["+str(targets)+"] = {\n")
RUNS = range(runs)
for r in RUNS:
    f.write("\t{")
    TARGETS = random_list(targets)
    for t in TARGETS:
        if t == TARGETS[-1]:
            f.write(str(t))
        else:
            f.write(str(t)+",")
    
    if r == RUNS[-1]:
        f.write("}")
    else:
        f.write("},\n")
f.write("\n};\n\n#endif TARGETS_H_\n");

f.close()


