/******************************************************************************
Universidad Nacional Autonoma de Mexico
Maestria en Ciencias Fisicas
Proyecto Final

Asignatura: Introduccion a Fisica Computacional
Profesor: Santiago Caballero
Estudiante: F. E. Charry-Pastrana

Generando datos de vista de techo del detector
*******************************************************************************/
#include <iomanip>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>  
#include <math.h>
#include <time.h>
#include <random>

using namespace std;

//***************************************************************************************
//***************************************************************************************
int main()
{
    int i, j, k;				// Contadoeres
    
    int nX = 400;
    int nY = 400;
    
    double X0 = -40;
    double X1 = +40;
    double Y0 = -40;
    double Y1 = +40;
    
    double deltaX = (X1 - X0) / nX;
    double deltaY = (Y1 - Y0) / nY;
    
    double Matrix[nX][nY];
    
    double PhiAle, r, X, Y, pendiente, b, m;
    int nXAux, nYAux;
    
    int nXMuestraMin = (int)(0.49*nX);
    int nXMuestraMax = (int)(0.51*nX);
    int nYMuestraMin = (int)(0.49*nY);
    int nYMuestraMax = (int)(0.51*nY);
    
    int NumeroEventos = 3*pow(10,0);
    
    double YPrima, XPrima;
    
    ofstream myfilePuntos,myfileTrayectorias;
    
    myfilePuntos.open("Datos.txt");
    myfileTrayectorias.open("Trayectoria.txt");
    
    srand((int)time(0));
    
    int nXFinal, nYFinal, nX1, nY1, nInicial, nFinal;
    double Phi1, Phi2, Phi3, Phi4;
    double aleatorio; 
    
    myfileTrayectorias<<"# nXAux\t nYAux \t XReal \t XReal";
    
    while (i++ < NumeroEventos)
    {
        //  Puntos de emisicion aleatorio
        srandom(time(NULL));
        nXAux = (rand() % (nXMuestraMax-nXMuestraMin+1)) + nXMuestraMin;
        nYAux = (rand() % (nYMuestraMax-nYMuestraMin+1)) + nYMuestraMin;
        
        Phi1=atan((double)(nY-nYAux)/(nX-nXAux));
        Phi2=atan((double)(nY-nYAux)/(nXAux)) + M_PI/2;
        Phi3=atan((double)(nYAux)/(nXAux)) + M_PI;
        Phi4=atan((double)(nYAux)/(nX-nXAux)) + 3*M_PI/2;
        
        cout<<"\n Phi 1: "<<Phi1*180/M_PI;
        cout<<"\t Phi 2: "<<Phi2*180/M_PI;
        cout<<"\t Phi 3: "<<Phi3*180/M_PI;
        cout<<"\t Phi 4: "<<Phi4*180/M_PI;
        
        //Puntos de deteccion
        nX1 = (rand() % (nX));
        nY1 = (rand() % (nY));

        cout<<"\n Deteccion. \t X:"<<nX1<<"\t Y:"<<nY1;
        
        
        nXFinal = 0;
        nYFinal = 0;

        
        //  Direccion aleatoria
        PhiAle = 2*M_PI*(double)rand()/(RAND_MAX);
        pendiente = (tan(PhiAle));
        
        if(PhiAle<Phi1 || PhiAle>Phi4)
        {
            nXFinal = nX;
            nYFinal = (int)((nX-nXAux)*pendiente) + nYAux;
        }
        
        if(PhiAle>= Phi1 && PhiAle<Phi2)
        {
            nYFinal = nY;
            nXFinal = (int)((nY-nYAux)/pendiente) + nXAux;
        }
        
        if(PhiAle>= Phi2 && PhiAle<Phi3)
        {
            nXFinal = 0;
            nYFinal = (int)(nYAux*pendiente) +nYAux;
        }
        
        if(PhiAle>=Phi3 && PhiAle<Phi4)
        {
            nYFinal = 0;
            nXFinal  = (int)((nYAux)/pendiente) + nXAux;
        }
        
        X = deltaX*nXAux + X0;
        Y = deltaY*nYAux + Y0;
        XPrima = deltaX*nXFinal+X0;
        YPrima = deltaY*nYFinal+Y0;
        
        m = (double)(nYFinal-nYAux)/(nXFinal-nXAux);
        b = (double)(nYAux-m*nXAux);
        
        myfilePuntos<<fixed<<setprecision(2);
        myfilePuntos<<"\n nXAux: "<<nXAux;
        myfilePuntos<<"\t nYAux: "<<nYAux;
        myfilePuntos<<"\t X: "<<X;
        myfilePuntos<<"\t Y: "<<Y;
        myfilePuntos<<"\t Phi: "<<PhiAle*180/M_PI;
        myfilePuntos<<"\t m: "<<pendiente;
        myfilePuntos<<"\t XPrima: "<<XPrima;
        myfilePuntos<<"\t YPrima: "<<YPrima;
        myfilePuntos<<"\t nXFinal: "<<nXFinal;
        myfilePuntos<<"\t nYFinal: "<<nYFinal;
        myfilePuntos<<"\t Punto intercepcion, b: "<<b;
        
        if(nXAux>nXFinal)
        {
            nInicial = nXFinal;
            nFinal = nXAux;
        }
        else
        {
            nInicial = nXAux;
            nFinal = nXFinal;
        }
        
        
        for(j=nInicial; j<=nFinal; j++)
        {
//            myfilePuntos<<"\n \t nXAux: "<<j;
//            myfilePuntos<<"\t \t nYAux: "<<(int)(m*j+b);
//            myfilePuntos<<"\t \t XReal: "<<(double)(j*deltaX+X0);
//            myfilePuntos<<"\t \t YReal: "<<(double)((m*j+b)*deltaY+Y0);
            
            myfileTrayectorias<<"\n"<<j;
            myfileTrayectorias<<"\t"<<(int)(m*j+b);
            myfileTrayectorias<<"\t"<<(double)(j*deltaX+X0);
            myfileTrayectorias<<"\t"<<(double)((m*j+b)*deltaY+Y0);
        }
        

    }
    
    cout<<"\n\n";
	return 0;
}
//***************************************************************************************
//***************************************************************************************
