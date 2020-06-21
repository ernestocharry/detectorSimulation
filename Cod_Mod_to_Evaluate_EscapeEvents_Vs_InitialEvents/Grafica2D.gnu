set terminal postscript eps size 5.3,3 enhanced color
set out 'Grafica_Porcentaje_Eventos_Escapados.eps'

#set xtics (1,2,3,4)


set grid
set key  b font ",18"

set log x
set format x "10^{%T}"

set xrange [1:1000000]
set yrange [0:25]


set ylabel "Porcentaje de eventos que se escaparon" font ",18"
set xlabel "No. de eventos generados aleatoriamente" font ", 18"

set style fill transparent solid 0.2 noborder

plot 'NoEventGenerados_NoEventsEscape_Probabilidad.txt' using 1:($2-$3):($2+$3) with filledcurves title '68% confidence value', 'NoEventGenerados_NoEventsEscape_Probabilidad.txt' u 1:2  pt 7 ps 1 lw 3 title 'Average value (50 repetitions)'

