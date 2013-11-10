#/usr/bin/env python
# Merge bt_power and wifi_power tables

import sys
import sqlite3
import shutil

def main():
    db = sys.argv[1]
    newDb = db + ".new"
    print "Creating: " + newDb
    
    # Create a copy of the original database
    shutil.copy(db,newDb)
    db = None
    
    conn = sqlite3.connect(newDb)
    
    # See if there is a wifi_power table
    tables = conn.execute("SELECT * FROM (SELECT tbl_name FROM sqlite_master)").fetchall()
    hasWifiPower = False
    if((u'wifi_power',) in tables):
        hasWifiPower = True
    
    # Rename bt_power to results
    conn.execute("ALTER TABLE bt_power RENAME TO results;")
    conn.commit()
    
    # Add radio columnhasWifiPower
    conn.execute("ALTER TABLE results ADD COLUMN radio TEXT;")
    if(hasWifiPower):
        conn.execute("ALTER TABLE wifi_power ADD COLUMN radio TEXT;")
    conn.commit()
    
    # Set radio for all entries since they came from bt_power
    conn.execute("UPDATE results SET radio='bluetooth';")
    if(hasWifiPower):
        conn.execute("UPDATE wifi_power SET radio='wifi';")
    conn.commit()            
    
    # Create duration column and copy over values (results)
    conn.execute("ALTER TABLE results ADD COLUMN duration NUMERIC;")
    results = conn.execute("SELECT config_num FROM results;")
    for r in results:
        config_num = r[0]
        duration = conn.execute("SELECT duration FROM bt_power_duration WHERE config_num=%d;"%(config_num)).fetchone()[0]
        conn.execute("UPDATE results SET duration='%f' WHERE config_num='%d'"%(duration,config_num))
    conn.execute("DROP TABLE bt_power_duration;")
    conn.commit()            
    
    # Create duration column and copy over values (wifi_power)
    if(hasWifiPower):
        conn.execute("ALTER TABLE wifi_power ADD COLUMN duration NUMERIC;")
        results = conn.execute("SELECT config_num FROM wifi_power;")
        for r in results:
            config_num = r[0]
            duration = conn.execute("SELECT duration FROM wifi_power_duration WHERE config_num=%d;"%(config_num)).fetchone()[0]
            conn.execute("UPDATE wifi_power SET duration='%f' WHERE config_num='%d'"%(duration,config_num))
        conn.execute("DROP TABLE wifi_power_duration;")
        conn.commit()             
    
    # Create quality column and copy over values from psnr
    tables = ["results"]
    if(hasWifiPower):
        tables.append("wifi_power")
    for table in tables:
        conn.execute("ALTER TABLE %s ADD COLUMN quality NUMERIC;"%(table))
        results = conn.execute("SELECT config_num FROM %s;"%(table))
        for r in results:
            config_num = r[0]
            quality = conn.execute("SELECT psnr FROM psnr WHERE config_num=%d;"%(config_num)).fetchone()[0]
            conn.execute("UPDATE %s SET quality='%f' WHERE config_num='%d'"%(table,quality,config_num))
    conn.execute("DROP TABLE psnr;")
    conn.commit()
    
    # Copy result from wifi_power to results and add offset
    if(hasWifiPower):
        offset = conn.execute("SELECT MAX(config_num) FROM results;").fetchone()[0]
        cmd = "UPDATE wifi_power SET config_num = config_num + %d;" %(offset)
        conn.execute(cmd)
        conn.commit()
    
        # Insert wifi_power into results and drop table
        conn.execute("INSERT INTO results SELECT * FROM wifi_power;")
        conn.execute("DROP TABLE wifi_power;")
        conn.commit();
        
        
    
    

if __name__ == "__main__":
        main()

