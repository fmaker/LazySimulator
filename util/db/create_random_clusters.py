#!/usr/bin/env python

import sys
import sqlite3
import shutil
import random

NUM_PER_AXIS=10

MAX = 100
MIN = 0

MAX_X = 100
MIN_X = 1
MAX_Y = 100
MIN_Y = 1
XY_DELTA = 0.1

def main():
    db = sys.argv[1]
    
    conn = sqlite3.connect(db)
    
    table = "results"
        
    cursor = conn.execute("SELECT config_num FROM %s;" %(table))
        
    for row in cursor:
        config_num = row[0]
        
        # Random coordinate
        x = random.randrange(MIN_X,MAX_X/NUM_PER_AXIS)*NUM_PER_AXIS
        y = random.randrange(MIN_Y,MAX_Y/NUM_PER_AXIS)*NUM_PER_AXIS
        
        # Random value around that coordinate
        energy = random.uniform(x-XY_DELTA,x+XY_DELTA)
        quality = random.uniform(y-XY_DELTA,y+XY_DELTA)
        
        conn.execute("UPDATE %s SET energy='%f' WHERE config_num='%d' " %(table,energy,config_num))
        conn.execute("UPDATE %s SET quality='%f' WHERE config_num='%d' " %(table,quality,config_num))
        
        # These don't exist for fake configs
        conn.execute("UPDATE %s SET duration='%d' WHERE config_num='%d' " %(table,0,config_num))
        conn.execute("UPDATE %s SET power='%d' WHERE config_num='%d' " %(table,0,config_num))
    
    conn.commit()
    conn.close()

if __name__ == "__main__":
        main()

