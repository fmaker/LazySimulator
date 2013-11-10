set ylabel "Quality (PSNR)"
set xlabel "Energy (J)"
plot [0:35]"db/all_cam_configs.dat" u 2:3 with points pointsize 1 pointtype 7 title "Configurations",x<25?x:1/0 t "Ratio = 1.0" lw 6,2*x<40?2*x:1/0 t "Ratio = 2.0" lw 6,3*x<40?3*x:1/0 t "Ratio = 3.0" lw 6,4*x<40?4*x:1/0 t "Ratio = 4.0" lw 6,5*x<40?5*x:1/0 t "Ratio = 5.0" lw 6 lc 7
