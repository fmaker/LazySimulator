#!/bin/bash
echo "Generating graphs..."
gnuplot emsoft11_bargraphs.plt
echo "Cropping key..."
./crop_key.sh /home/fmaker/workspace/emsoft11/img/key.png
