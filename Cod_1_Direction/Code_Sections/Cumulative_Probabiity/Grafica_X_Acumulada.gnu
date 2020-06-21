set terminal postscript eps size 5.3,3 enhanced color
set out 'Grafica_X_Acumulada.eps'

set grid
set key  b font ",18"

#set log y
#set format x "10^{%T}"

set xrange [0:15]
#set yrange [-0.1:1.1]

set xtics (5, 5.5,  8.25, 10.5, 15)

set label 1 at 2, 0.55
set label 1 "Muestra" font ",18"

set label 2 at 5.25, 0.45 rotate by 90
set label 2 "Contenedor" font ",18"

set label 3 at 6, 0.55
set label 3 "Aire" font ",18"

set label 4 at 8, 0.47 rotate by 90
set label 4 "Aluminio" font ",18"

set label 5 at 9, 0.55
set label 5 "Aire" font ",18"

set label 6 at 12, 0.55
set label 6 "Cristal de Ge" font ",18"

set arrow from 7.75,0 to 7.75,1 nohead dt 0


set ylabel "Probabilidad acumulada" font ",18"
set xlabel "Distancia en 1D (mm)" font ", 18"

plot 'X_AbsorptionProbability.txt' u 1:2 w l lw 3.5 notitle
