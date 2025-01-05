set terminal png size 1600,1200 enhanced font ',30' lw 2
set output './png/attractor_30.png'
set title font ',36'

set title "Time evolution"

set monochrome

set view 63, 60



set ylabel 'y'
set xlabel 'x'
set zlabel 'z'

unset xtics
unset ytics
unset ztics

splot "./time_evolution_data_30.dat" using 2:3:4 w l lw 1 notitle