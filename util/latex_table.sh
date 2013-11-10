#!/bin/bash

if [ $# -ne 1 ]
then
    echo "Usage: $0 DIRECTORY"
    exit -1
fi

cat energy/success.dat  | head -n 1 | sed "s/ /\n/g" > /tmp/header
cat energy/duration.dat  | tail -n 1 | sed "s/ /\n/g" > /tmp/duration
join /tmp/header /tmp/duration > /tmp/output
cat energy/success.dat  | tail -n 1 | sed "s/ /\n/g" > /tmp/success
join /tmp/output /tmp/success > /tmp/output
cat energy/tablesize.dat  | tail -n 1 | sed "s/ /\n/g" > /tmp/tablesize
join /tmp/output /tmp/tablesize > /tmp/output
cat energy/total_energy.dat  | tail -n 1 | sed "s/ /\n/g" > /tmp/total_energy
join /tmp/output /tmp/total_energy > /tmp/output



