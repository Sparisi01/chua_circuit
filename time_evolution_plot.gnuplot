set terminal png size 1600,1200 enhanced font ',30' lw 2
set output './png/attractor_test.png'
set title font ',36'

set title "Time evolution"

set view 63, 45

set ylabel 'y'
set xlabel 'x'
set zlabel 'z'

unset xtics
unset ytics
unset ztics

splot "./time_evolution_data.dat" using 2:3:4 w l lw 1 notitle
 