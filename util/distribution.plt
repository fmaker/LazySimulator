set terminal png
set output "count_distribution.png"
set multiplot layout 3,1 rowsfirst
set xlabel 'Energy (J)'
set ylabel '# Configs'
plot "./all_cam_configs_energy_counts.dat" u 1:2 w filledcurve t "Exhaustive","./doe_configs_energy_counts.dat" u 1:2 w filledcurve t "DoE"
unset title
set xlabel 'Quality (PSNR)'
plot "./all_cam_configs_quality_counts.dat" u 1:2 w filledcurve t "Exhaustive","./doe_configs_quality_counts.dat" u 1:2 w filledcurve t "DoE"
set xlabel 'Ratio (Quality/Energy)'
plot "./all_cam_configs_ratio_counts.dat" u 1:2 w filledcurve t "Exhaustive","./doe_configs_ratio_counts.dat" u 1:2 w filledcurve t "DoE"
unset multiplot
