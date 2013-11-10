#!/usr/bin/env python
import sqlite3,sys,csv,os

if(len(sys.argv) != 2):
    print "Usage: output_result_space inputfile.db3"
    sys.exit(1)

CONFIGS = sys.argv[1]
PERCENT = 0.10
OUTPUT = "pareto.dat"


conn = sqlite3.connect(CONFIGS)

# Get absolute maximum and minimum
query = "SELECT MAX(quality) FROM results;"
cursor = conn.execute(query)
MAX = cursor.fetchone()[0]

query = "SELECT MIN(quality) FROM results WHERE (quality > 0);"
cursor = conn.execute(query)
MIN = cursor.fetchone()[0]
if(MIN == 0):
    MIN = PERCENT

#print MIN,MAX
# Just one value first

results = []
q = MIN
while q < MAX:
    q_max = q * (1+PERCENT)
    q_min = q * (1-PERCENT)
    PARETO_STMT = "SELECT energy,quality FROM results WHERE (quality <= %.2f) AND (quality >= %.2f) ORDER BY energy;" %(q_max,q_min)
    #print PARETO_STMT
    cursor = conn.execute(PARETO_STMT)
    (p_e, p_q) = cursor.fetchone()
    #print q,p_e,p_q
    results.append((p_q,p_e))
    q += PERCENT

results_set = set(results)
results = sorted(list(results_set))
pareto = []

last_q = 0
for i in range(len(results)):
    if results[i][0] != last_q:
        pareto.append(results[i])
        last_q = results[i][0]

writer = csv.writer(open(OUTPUT,'w'),delimiter='\t')
for p in pareto:
    print p
    writer.writerow(p)

# Line fit



