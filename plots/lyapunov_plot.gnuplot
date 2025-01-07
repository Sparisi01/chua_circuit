set terminal png size 1600,1200 enhanced font ',30' lw 2
set output './png/lyap_exp_25.png'
set title font ',40'

set monochrome

set ylabel 'λᵢ' font ',40'
set xlabel 'T' font ',40'

set xrange [1:200]


plot "./data/lyapunov_25.dat" using 1:2 w l lw 1.5 notitle, \
"./data/lyapunov_25.dat" using 1:3 w l lw 1.5 notitle, \
"./data/lyapunov_25.dat" using 1:4 w l lw 1.5 notitle