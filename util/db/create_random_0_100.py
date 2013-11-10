#!/usr/bin/env python

import sys
import sqlite3
import shutil
import random

MAX = 100
MIN = 0

def main():
    db = sys.argv[1]
    
    conn = sqlite3.connect(db)
    
    table = "results"
        
    cursor = conn.execute("SELECT config_num FROM %s;" %(table))
        
    for row in cursor:
        config_num = row[0]
        energy = random.uniform(MIN,MAX)
        quality = random.uniform(MIN,MAX)
        
        conn.execute("UPDATE %s SET energy='%f' WHERE config_num='%d' " %(table,energy,config_num))
        conn.execute("UPDATE %s SET quality='%f' WHERE config_num='%d' " %(table,quality,config_num))
        
        # These don't exist for fake configs
        conn.execute("UPDATE %s SET duration='%d' WHERE config_num='%d' " %(table,0,config_num))
        conn.execute("UPDATE %s SET power='%d' WHERE config_num='%d' " %(table,0,config_num))
    
    conn.commit()
    conn.close()

if __name__ == "__main__":
        main()

