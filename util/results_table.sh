for i in 2 3 4;do for r in *.dat;do more +4 total_energy.dat | head -n1 | cut -d' ' -f8|sed -e s/r/ /g;done; echo ; done
