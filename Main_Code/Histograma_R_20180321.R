rm(list = ls())
PuntoAleatorios = read.table("/Users/F.E.CharryPastrana/Google\ Drive/3_Master_UNAM/3_Introducción_Física_Computacional/Mercurial/introcomputationalphysics2/0-Proyecto_MonteCarlo/Main_Code/PuntosAleatorios1.txt")
hist(PuntoAleatorios [,9], breaks = 1e3, freq = FALSE,  col="lightcyan", main="Histograma de variable aleatoria. No. de puntos: 1e7",xlab="Valor variable aleatoria Theta Min",ylab="Frecuencia")
