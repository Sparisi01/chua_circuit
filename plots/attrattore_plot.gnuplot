set terminal png size 1600,1200 enhanced font ',30' lw 2
set output './png/bacino_attrazione.png'
set title font ',36'

set xlabel 'x'
set ylabel 'y'
set title '2D Plot con colore in base alla terza colonna'

set palette rgbformulae 13, 22, -31  # Definisce una palette di colori                      # Mostra la legenda dei colori
unset key                         # Rimuove la legenda dei punti
unset colorbox

plot './convergenza.dat' using 1:2:4 with points pointtype 9 pointsize 0.3 palette

set output
