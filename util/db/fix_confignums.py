#!/usr/bin/env python
# Merge bt_power and wifi_power tables

import sys
import sqlite3
import shutil

powerTables = ["bt_power","wifi_power"]

def main():
    db = sys.argv[1]
    
    conn = sqlite3.connect(db)
    
    table = "results"
        
    cursor = conn.execute("SELECT config_num FROM %s;" %(table))
        
    i = 1
    for row in cursor:
        config_num = row[0]
            
        # Need to fix config
        if(config_num != i):
            conn.execute("UPDATE %s SET config_num='%d' WHERE config_num='%d' " %(table,i,config_num))
        i+=1
    
    conn.commit()
    conn.close()
        
        
    
    

if __name__ == "__main__":
        main()

