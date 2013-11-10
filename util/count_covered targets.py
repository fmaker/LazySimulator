#!/usr/bin/env python
import sqlite3,sys,csv,os

if(len(sys.argv) != 2):
    print "Usage: output_result_space inputfile.db3"
    sys.exit(1)

CONFIGS = sys.argv[1]
#ALL_CONFIGS = "/home/fmaker/workspace/LazySimulator/util/db/all_cam_configs.db3"
#DOE_CONFIGS = "/home/fmaker/workspace/LazySimulator/util/db/doe_configs.db3"
#OUTPUT_DIR = "/home/fmaker/workspace/LazySimulator/util/db/"
PERCENT = 0.10

TARGETS = [("energy","energy"),("quality","quality"),("ratio","quality/energy")]

DB_FILENAME = os.path.split(sys.argv[1])[1].split('.')[0]

for (target,sqlcol) in TARGETS:
    OUTPUT = DB_FILENAME+"_"+target+"_counts.dat"
    
    writer = csv.writer(open(OUTPUT,'w'),delimiter=' ')
    writer.writerow([target,"Count"])
    if __name__ == '__main__':
        # Get absolute maximum and minimum
        query = "SELECT MAX("+sqlcol+") FROM results;"
        conn = sqlite3.connect(CONFIGS)
        cursor = conn.execute(query)
        MAX = cursor.fetchone()[0]
        
        query = "SELECT MIN("+sqlcol+") FROM results;"
        conn = sqlite3.connect(CONFIGS)
        cursor = conn.execute(query)
        MIN = cursor.fetchone()[0]
        if(MIN == 0):
            MIN = PERCENT
        
        i = PERCENT/2
        conn = sqlite3.connect(CONFIGS)
        data = []
        maxCount = 0
        while (i < MAX):
            numValidConfigs = "SELECT COUNT(*) FROM results WHERE (%s >= %f) AND (%s <= %f)" %(sqlcol,i*(1-PERCENT),sqlcol,i*(1+PERCENT))
            cursor = conn.execute(numValidConfigs)
            count = cursor.fetchone()[0]
            data.append((i,count))
            if count > maxCount:
                maxCount = count;
            i = i + PERCENT/2
        conn.close()
        
        for (i,count) in data:
            writer.writerow([i,count/float(maxCount)])
    
        writer.writerow([i+PERCENT/2,0])
