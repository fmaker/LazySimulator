#!/usr/bin/env python
import csv
import os
import sys
import shutil

if(len(sys.argv) < 2):
    print "Usage: cmd <results_dir>"
    exit(1)

#BASE = "/home/fmaker/workspace/LazySimulator/util/result"
#SUMMARY_BASE = "/home/fmaker/workspace/LazySimulator/util/results_summary"
BASE = os.path.abspath(sys.argv[1])
SUMMARY_BASE = sys.argv[1]+"/summary"
TARGETS = ["energy","ratio","quality"]


# For each target
for target in TARGETS:
    filename = target+".dat"
    # For each column
    #(4,"duration","avg"),
    #,(7,"energy_used","avg")
    for (colNum,colName,format) in [(4,"total_duration","sum"),(7,"total_energy","sum"),(8,"success","percent"),(6,"tablesize","last")]:
        dir = SUMMARY_BASE+"/"+target+"/"
        try:
            os.makedirs(dir)
        except OSError: # Already exists
            pass
        
	FILE = dir+colName+".dat"
        writer = csv.writer(open(FILE, 'w'),delimiter=' ')

        # Each row is the device name
        header = False
        devs = os.listdir(BASE)
	try:
        	devs.remove("summary")
	except:
		pass	
#        devs.remove("post_proc.py")
        devs.remove("config.xml")
#	devs.remove("each_run_result.py")
#	devs.remove("run_results")
        devs.sort()
        for d in devs:
            fileRow = []
            fileRow.append(d)
            # Each column is the algorithm
            algs = os.listdir(BASE+"/"+d)
            algs.sort()
            
            if(not header):
                writer.writerow(["Device"]+algs)
                header = True
            
            for a in algs:
                runs = os.listdir(BASE+"/"+d+"/"+a)
                
                # Average runs
                results = []
                for r in runs:
                    value = 0
                    numTargets = 0
		    RUN_FILE = BASE+"/"+d+"/"+a+"/"+r+"/"+filename
		    print RUN_FILE
                    reader = csv.reader(open(RUN_FILE, "rb"),delimiter=' ')
                    reader.next() # skip header
                    for row in reader:
			if format == "percent" and int(row[0]) == 0:
				#print "Skipping DoE: ",row
				pass
			else:
                        	value += float(row[colNum])
                        	numTargets+=1
                    
                    if(format == "percent"):
                        results.append((value / float(numTargets)) * 100)   
                    elif(format == "last"):
                        results.append(float(row[colNum]))
                    elif(format == "avg"):
                        results.append(value / numTargets)
                    elif(format == "sum"):
                        results.append(value)
                
                result = sum(results) / len(results)  # Average out over all runs
                fileRow.append(result)
            
            writer.writerow(fileRow)

# Copy GNUplot slideshow into summary folder
#shutil.copyfile(sys.path[0]+"/../../util/summary_slideshow",BASE+"/summary/summary_slideshow");

