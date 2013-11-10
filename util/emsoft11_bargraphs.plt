set style fill solid
set style histogram clustered
set style data histograms
set key autotitle columnhead
set key outside
set key bottom center
set key horizontal
unset key

set terminal png

# Sensitivities
## For each Success and Energy

## Energy limit
set ylabel "Success (%)"
set output "/home/fmaker/workspace/emsoft11/img/englimit_sensitivity_success.png"
plot [][0:100]"/home/fmaker/workspace/LazySimulator/results/energy_limited/summary/ratio/success.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9

set key autotitle columnhead
set key outside
set key bottom center
set key horizontal
set output "/home/fmaker/workspace/emsoft11/img/key.png"
plot "" u 2:xtic(1) lt 1 t "Design of Experiments","" u 3 lt 2 t "Hill Climbing", "" u 4 lt 3 t "HillClimbing + DoE","" u 5 lt 4 t "Hill Climbing + DoE w/Only New","" u 6 lt 5 t "Hill Climbing w/Only New","" u 7 lt 6 t "Random Greedy","" u 8 lt 7 t "Random Greedy + DoE","" u 9 lt 8 t "Random Greedy + DoE w/Only New","" u 10 lt 9 t "Random Greedy w/Only New"
unset key

set output "/home/fmaker/workspace/emsoft11/img/durlimit_sensitivity_success.png"
plot [][0:100]"/home/fmaker/workspace/LazySimulator/results/duration_limited/summary/ratio/success.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9

set output "/home/fmaker/workspace/emsoft11/img/memlimit_sensitivity_success.png"
plot [][0:100]"/home/fmaker/workspace/LazySimulator/results/memory_limited/summary/ratio/success.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9

set xtic rotate by -45
set output "/home/fmaker/workspace/emsoft11/img/hw_sensitivity_success.png"
plot [][0:100]"/home/fmaker/workspace/LazySimulator/results/paper_hardwares/summary/ratio/success.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9
set xtic rotate by 0

set output "/home/fmaker/workspace/emsoft11/img/numtargets_sensitivity_success.png"
plot [][0:100]"/home/fmaker/workspace/LazySimulator/results/targetnum_sensitivity/summary/ratio/success.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9

set output "/home/fmaker/workspace/emsoft11/img/percent_sensitivity_success.png"
plot [][0:100]"/home/fmaker/workspace/LazySimulator/results/percent_sensitivity/summary/ratio/success.dat"   u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9


set ylabel "Energy (J)"
set output "/home/fmaker/workspace/emsoft11/img/englimit_sensitivity_energy.png"
plot "/home/fmaker/workspace/LazySimulator/results/energy_limited/summary/ratio/total_energy.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9

set output "/home/fmaker/workspace/emsoft11/img/durlimit_sensitivity_energy.png"
plot "/home/fmaker/workspace/LazySimulator/results/duration_limited/summary/ratio/total_energy.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9

set output "/home/fmaker/workspace/emsoft11/img/memlimit_sensitivity_energy.png"
plot "/home/fmaker/workspace/LazySimulator/results/memory_limited/summary/ratio/total_energy.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9

set xtic rotate by -45
set output "/home/fmaker/workspace/emsoft11/img/hw_sensitivity_energy.png"
plot "/home/fmaker/workspace/LazySimulator/results/paper_hardwares/summary/ratio/total_energy.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9
set xtic rotate by 0

set output "/home/fmaker/workspace/emsoft11/img/numtargets_sensitivity_energy.png"
plot "/home/fmaker/workspace/LazySimulator/results/targetnum_sensitivity/summary/ratio/total_energy.dat" u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9

set output "/home/fmaker/workspace/emsoft11/img/percent_sensitivity_energy.png"
plot "/home/fmaker/workspace/LazySimulator/results/percent_sensitivity/summary/ratio/total_energy.dat"   u 2:xtic(1) lt 1,"" u 3 lt 2, "" u 4 lt 3,"" u 5 lt 4,"" u 6 lt 5,"" u 7 lt 6,"" u 8 lt 7,"" u 9 lt 8,"" u 10 lt 9


