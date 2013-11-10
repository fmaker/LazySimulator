#!/usr/bin/env bash
#if [ $# -ne 1 ]; then
#	echo "Usage: $0 <filename.dat>" 
#	exit
#fi	

FILES="duration energy_used success tablesize total_energy"

DIRS="energy quality ratio"
if [ ! -d "all" ]; then
	mkdir all
fi

if [ -f all/$1.dat ]; then
	rm all/$1.dat
fi

for f in $FILES
do
	header=true
	for d in $DIRS
	do
		if $header; then
			head -n 1 $d/$f.dat >> all/$f.dat
			header=false
		fi	
		tail -n 1 $d/$f.dat | sed "s/all_cam_configs/$d/g" >> all/$f.dat
	done
done
