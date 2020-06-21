set terminal epslatex
set output 'Grafica_Barras_Procesos.tex'

set style line 2 lc rgb 'black' lt 1 lw 1
set grid y
set style data histograms
set style histogram rowstacked
set boxwidth 0.5
#set style fill pattern border -1
set style fill solid
set ytics 10 nomirror
set yrange [0:100]
set ylabel "Porcentaje de diferentes procesos"
set xlabel "Energía (keV)" offset character 0, -0.5, 0
set ytics 10

set key bottom right outside horizontal
unset title

set xtics  border rotate by 90  offset character +0, -1.7, 0

plot \
'Resultados_Resumen.txt' using 14 t "Muestra" lt rgb '#a6bddb', \
'' using 16 t "Aluminio" lt rgb '#525252', \
'' using 18 t "Vial" ls 2 lt rgb '#fc9272', \
'' using 20 t "Aire" ls 2 lt rgb '#3182bd', \
'' using 2 t "Fotoeléctrico en Ge" ls 2 lt rgb '#00441b', \
'' using 4:xtic(1) t "Compton en Ge" ls 2 lt rgb '#41ab5d', \

set output
