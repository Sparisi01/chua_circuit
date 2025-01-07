set terminal png size 1600,4600 enhanced font ',30' lw 2
set output './png/multiplot.png'
set title font ',40'

set multiplot layout 6,1
set monochrome

set view 63, 30

unset xtics
unset ytics
unset ztics

set xlabel 'x'
set ylabel 'y'
set zlabel 'z'

set title "β = 50"
splot "./data/time_evolution_data_50.dat" every ::20000 using 2:3:4 w l lw 1 notitle

unset xlabel
unset ylabel
unset zlabel

set title "β = 35"
splot "./data/time_evolution_data_35.dat" every ::20000 using 2:3:4 w l lw 1 notitle
set title "β = 33.8"
splot "./data/time_evolution_data_33_8.dat" every ::20000 using 2:3:4 w l lw 1 notitle
set title "β = 32"
splot "./data/time_evolution_data_32.dat" every ::20000 using 2:3:4 w l lw 1 notitle
set title "β = 30"
splot "./data/time_evolution_data_30.dat" every ::20000 using 2:3:4 w l lw 1 notitle
set title "β = 25"
splot "./data/time_evolution_data_25.dat" every ::20000 using 2:3:4 w l lw 1 notitle