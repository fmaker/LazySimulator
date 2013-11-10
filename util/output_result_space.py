#!/usr/bin/env python
import sqlite3,sys,csv

if(len(sys.argv) != 2):
    print "Usage: output_result_space inputfile.db3"
    sys.exit(1)

DATABASE = sys.argv[1]
OUTPUT = DATABASE.split(".")[0]+".dat"

writer = csv.writer(open(OUTPUT,'w'),delimiter=' ')
writer.writerow(["Number","Energy","Quality","Ratio"])
if __name__ == '__main__':
    i = 1
    query = "SELECT energy,quality,quality/energy FROM results;"
    conn = sqlite3.connect(DATABASE)
    cursor = conn.execute(query)
    rows = cursor.fetchall()
    for row in rows:
        writer.writerow([i,row[0],row[1],row[2]]);
        i += 1
    conn.close()
