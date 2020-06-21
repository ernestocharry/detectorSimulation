set terminal epslatex
set output 'Grafica_eficiencia_agua_Lin.tex'

set grid

unset title
set xlabel "Energía (keV)"
set ylabel 'Eficiencia, $\epsilon$'

plot 'Eficiencia_Agua.txt' u 1:2 w l lc rgb '#2171b5' lw 5 notitle, 'Eficiencia_Agua.txt' u 1:2 w p lc rgb '#084594' pt 7 ps 1.5 title 'Valores experimentales', 'Resultados_Resumen.txt' u ($1*1000):($22/100):($23/100) w yerrorbars lc rgb '#00441b' pt 7 ps 1.5 title 'Simulacion'

set output
