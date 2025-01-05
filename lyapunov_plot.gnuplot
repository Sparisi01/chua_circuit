set terminal png size 1600,1200 enhanced font ',30' lw 2
set output './png/lyap_exp_30.png'
set title font ',36'

set monochrome

set title "Lyapunov exponent convergence on evolution time"

set ylabel 'λ'
set xlabel 'Propagation time'

set xrange [1:200]


plot "./lyapunov_30.dat" using 1:2 w l lw 1.5 notitle, \
"./lyapunov_30.dat" using 1:3 w l lw 1.5 notitle, \
"./lyapunov_30.dat" using 1:4 w l lw 1.5 notitle