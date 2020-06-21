rm(list = ls())
PuntoAleatorios = read.table("/Users/F.E.CharryPastrana/Google Drive/3_Master_UNAM/3_Introducción_Física_Computacional/Mercurial/introcomputationalphysics2/0-Proyecto_MonteCarlo/Cod_1_Direction/Code_Sections/Distancia_mm_emision_Y_captura.txt")

hist(PuntoAleatorios [,3], breaks = 7e3, freq = TRUE,  col="lightcyan", main="Histograma de variable aleatoria. No. de puntos: 1e6",xlab="Distancia donde se captura rayo, mm",ylab="Frecuencia", xlim = c(0,33.5))
