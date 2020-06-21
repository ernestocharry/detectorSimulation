rm(list = ls())
PuntoAleatorios = read.table("/Users/F.E.CharryPastrana/Google Drive/3_Master_UNAM/3_Introducción_Física_Computacional/Mercurial/introcomputationalphysics2/0-Proyecto_MonteCarlo/Cod_1_Direction/Code_Sections/Distancia_mm_emision_Y_captura.txt")

hist(PuntoAleatorios [,3], breaks = 2e2, freq = TRUE,  col="lightcyan", main="No. de puntos: 6e5",xlab="Distancia absoluta de captura rayo gamma de 50 keV, mm",ylab="Frecuencia", xlim = c(0,15))
