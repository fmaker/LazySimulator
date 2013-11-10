#!/usr/bin/env python
import os,csv,string,locale
locale.setlocale(locale.LC_ALL, ('en_US', 'UTF8'))

targets = ["energy","quality","ratio"]
results = ["total_duration","success","tablesize","total_energy"]


def transpose(table):
    new_table = []
    for c in range(len(table[0])):
        new_row = []
        for r in range(len(table)):
            #print r,c,table[r][c]
            new_row.append(table[r][c])
        new_table.append(new_row)

    return new_table

for t in targets:
     raw_table = []
     have_header = False
     for r in results:
         reader = csv.reader(open("%s/%s.dat" %(t,r)), delimiter=' ')
         if have_header:
             reader.next() # Skip header
         else:
             row = reader.next()
             raw_table.append(row[1:])
             have_header = True

         for row in reader:
             raw = raw_table.append(map(int,map(float,row[1:])))


     
     #print raw_table
     #print "-----"
     
     table = transpose(raw_table)
     
     latex = ""
     for r in table:
         for c in r:
             #latex += str(c)
             if type(c) == type(0):
                 latex += locale.format("%d",c,grouping=True)
             else:
                 latex += c
             if c != r[-1]:
                 latex += " & "
         latex += "\\\\\n"

     print t
     print "--------------------------------------------------"
     print latex
     print ""
