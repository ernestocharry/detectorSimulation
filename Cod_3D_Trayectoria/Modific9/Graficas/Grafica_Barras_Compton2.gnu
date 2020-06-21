set terminal epslatex
set output 'Grafica_Barras_Compton2.tex'

set style line 2 lc rgb 'black' lt 1 lw 1
set grid y
set style data histograms
set style histogram rowstacked
set boxwidth 0.5
#set style fill pattern border -1
set style fill solid
set ytics 10 nomirror
set yrange [0:100]
set ylabel "Porcentaje de procesos después de Evento Compton"
set xlabel "Energía (keV)" offset character 0, -0.5, 0
set ytics 10

set key bottom right outside horizontal
unset title

set xtics  border rotate by 90  offset character +0, -1.7, 0

plot \
'Resultados_Resumen.txt' using 6 t "Fotoeléctrico 2" lt rgb '#00441b', \
'' using 8 t "Compton 2" lt rgb '#41ab5d', \
'' using 10 t "Sale del Ge" ls 2 lt rgb '#fa9fb5', \
'' using 12:xtic(1) t "Hubo problemas" ls 2 lt rgb '#ce1256'

set output
