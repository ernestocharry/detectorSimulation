set terminal postscript eps size 4,4 enhanced color
set out 'Grafica_Detector_Y_Z_XFijo.eps'

set xrange [-40:40]
set yrange [0:70]

set key  b font ",18"
#set grid back ls 12 lt 1 lw 2

set xlabel 'y (mm)' font ",18"
set ylabel 'z (mm)' font ",18"

set key off
set view map
set size ratio 1

splot 'Y_Z_Xfijo.txt' using 1:2:3 with points pointtype 5 pointsize 0.08 palette linewidth 30, 'PuntosAleatorios1.txt' u 5:6:12 with points pointtype 5 pointsize 0.8 palette linewidth 30, 'PuntosAleatoriosTrayectorias1.txt' u 2:3:4 with points pointtype 5 pointsize 0.1 palette linewidth 30
