set terminal pngcairo mono
set output "doe_coverage.png"
set ylabel "Quality (PSNR)"
set xlabel "Energy (J)"
plot [0:35]"all_cam_configs.dat" u 2:3 with points pointsize 1 pointtype 7 title "Configurations", "doe.dat" u 1:2:($1*0.1):($2*0.1) w boxxy t "Design of Experiments"

