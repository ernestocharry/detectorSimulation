set terminal postscript eps size 3,3 enhanced color
set out 'Graph_Trayectoria.eps'

#set xtics (1,2,3,4)


set grid
set key  b font ",18"

set size square

set xrange [-40:40]
set yrange [-40:40]


set ylabel "X (mm)" font ",18"
set xlabel "Y (mm)" font ", 18"

plot 'Trayectoria.txt' using 3:4 with p ps 0.5 notitle '68% confidence value'
