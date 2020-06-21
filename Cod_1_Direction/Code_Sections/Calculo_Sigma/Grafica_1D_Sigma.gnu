set terminal postscript eps size 5.3,3 enhanced color
set out 'Grafica_Porcentaje_EventosDetectados.eps'

set grid
set key  b font ",18"

set log x
set format x "10^{%T}"

set xrange [90:1000000]
set yrange [65:95]


set ylabel "Eventos detectados en el cristal de Ge" font ",18"
set xlabel "No. de eventos generados aleatoriamente" font ", 18"

#set style fill transparent solid 0.2 noborder

plot 'Eventos_ProbabilidadEscape_Sigma.txt' using 1:2:3 with yerrorbars title '68% confidence value' lw 3, 'Eventos_ProbabilidadEscape_Sigma.txt' u 1:2  pt 7 ps 1 lt rgb "black" title 'Average value (20 repetitions)'

