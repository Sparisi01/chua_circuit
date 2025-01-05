set terminal png size 1600,5400 enhanced font ',30' lw 2
set output './png/multiplot.png'
set title font ',36'



set multiplot layout 5,1
set monochrome

set view 63, 60


set ylabel 'y'
set xlabel 'x'
set zlabel 'z'

unset xtics
unset ytics
unset ztics

set title "β = 50"
splot "./data/time_evolution_data_50.dat" using 2:3:4 w l lw 1 notitle
set title "β = 40"
splot "./data/time_evolution_data_40.dat" using 2:3:4 w l lw 1 notitle
set title "β = 32"
splot "./data/time_evolution_data_32.dat" using 2:3:4 w l lw 1 notitle
set title "β = 30"
splot "./data/time_evolution_data_30.dat" using 2:3:4 w l lw 1 notitle
set title "β = 25"
splot "./data/time_evolution_data_25.dat" using 2:3:4 w l lw 1 notitle