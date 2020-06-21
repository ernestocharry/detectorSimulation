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

//	Global variables
int i, j, k;				// Contadores
const int nX = 10000;

int nXMuestra = 0;      // Cuadritos en Z muestra
int nXMuestraMin = nX;
int nXMuestraMax = 0;
int nXAux;

double CilindroX[nX][2];
double X;	// Contadores de valores reales
double X0, X1;
double deltaX;

double densidadGe = 5.32*pow(10,-3);   // Densidad Ge, g mm-3
double mu_rho_Ge = 333.5;   // mu sobre rho a 5 keV,  mm2 g-1

double a, b, c;

//	Functions
void GeometriaZonas();
void ValoresIniciales();
void LugaresEmisionAleatorios();


//***************************************************************************************
//***************************************************************************************
int main()
{
    
    cout << "\n 1) ValoresIniciales: Inicio";
    ValoresIniciales();
    cout << "\n 1) ValoresIniciales: Fin y O.K.";
    
    
    cout << "\n\n --------------------------------------\n";
    
    cout << "\n 2) GeometriaZonas: Inicio";
    GeometriaZonas();
    cout << "\n 2) GeometriaZonas: Fin y O.K.";
    
    cout << "\n\n --------------------------------------\n";
    
    cout << "\n 3) LugaresEmisionAleatorios: Inicio";
    LugaresEmisionAleatorios();
    cout << "\n 3) LugaresEmisionAleatorios: Fin y O.K.";
    
    cout<<"\n\n";
    return 0;
}
//***************************************************************************************
//***************************************************************************************
void ValoresIniciales()
{
    //--- Valores Limites Realmes ----------------------
    X0 = 0;
    X1 = +40;
    deltaX = (X1 - X0) / (nX-1);
}
//***************************************************************************************
//***************************************************************************************
void LugaresEmisionAleatorios()
{
    //----------
    ofstream myfileCapturaDistancia;
    myfileCapturaDistancia.open("Distancia_mm_emision_Y_captura.txt");
    myfileCapturaDistancia<<"#Distancia emision (mm) \t Distancia captura desde emision \t Distancia captura en detector (mm)\n";

    //----------
    
    //  Contador de No. de Eventos (decaimientos) vs. Eventos que se escapan

    int NumeroEventos = pow(10,6);
    int NumeroEventosEscape = 0;
    
    int nX_detenerse;
    double Probabilidad_Punto;
    double Evaluacion_Probabildad;
    double r, CaminoAntes = 0, CaminoMismoMaterial = 0;
    
    double X_distancia;
    i = 0;
    srand((int)time(0));
    while (i++ < NumeroEventos)
    {
        //  Numeros aleatorios de emision en la muestra
        r = (int) (((double)rand()/(RAND_MAX))*(nXMuestraMax-nXMuestraMin) + nXMuestraMin);
        nXAux = r;
        if(i<3)     cout<<"\n Valor punto emision aleatorio (matriz): "<<nXAux;
        //  Coordenadas Cartesianas
        X = deltaX*nXAux + X0;
        if(i<3)    cout<<"\n Valor punto x de emision: "<<X<<" mm";
        myfileCapturaDistancia << X <<"\t";
        
        // Numero aleatorio hasta donde llegara.
        a = (double)rand()/(RAND_MAX);      // Donde cae en probabilidad
        if(i<3)     cout<<"\n Valor aleatorio: "<<a;
        X_distancia = - log(1-a)/(densidadGe*mu_rho_Ge) ;
        myfileCapturaDistancia << X_distancia <<"\t";
        
        if(i<3)    cout<<"\n Valor punto x atrapado: "<<X_distancia+X<<" mm";
        myfileCapturaDistancia << X_distancia +X<<"\n";
        if(i<3)     cout<<"\n .................................................";
    }
    cout<<"\n De "<<NumeroEventos<<" eventos, "<<NumeroEventosEscape;
    cout<<" salieron del detector. \tPorcentaje: ";
    cout<<NumeroEventosEscape*100/NumeroEventos;
    
    myfileCapturaDistancia.close();
}
//***************************************************************************************
//***************************************************************************************
void GeometriaZonas()
{
    ofstream FileCartesianas;
    
    FileCartesianas.open("X_AbsorptionProbability.txt");
    FileCartesianas<<"#Valores de X \t AbsorptionProbability\n";
    
    for (i = 0; i<nX; i++)
        {
            X = deltaX*i + X0;    //    No es necesario
            //--------------------------------------------------------------------
            //	Si no se define, que sea aire. Aire = 0
            CilindroX[i][0] = densidadGe*mu_rho_Ge;
            //  Lugar de la muestra
            if(X>=0 &&  X<5)
            {
                CilindroX[i][0] = densidadGe*mu_rho_Ge;
                nXAux = i;
                
                if(nXAux > nXMuestraMax) nXMuestraMax = nXAux;
                if(nXAux < nXMuestraMin) nXMuestraMin = nXAux;
            }
            //  Pared de contenedor, vial
            if(X>=5 && X<5.5)       CilindroX[i][0] = densidadGe*mu_rho_Ge;
            
            //  Pared del detector, cobre
            if(X>=7.75 && X<8.25)   CilindroX[i][0] = densidadGe*mu_rho_Ge;
            
            //  Lugar del CRISTAL
            if(X>=10.5 && X<33.35)  CilindroX[i][0] = densidadGe*mu_rho_Ge;
            //--------------------------------------------------------------------
            //  Escribiendo en archivo
            FileCartesianas << fixed<<setprecision(7);
            FileCartesianas << X << "\t" << 1-exp(-X*CilindroX[i][0]) << "\n";
            
            //--------------------------------------------------------------------
            // Se guardara el lugar de interaccion.
            CilindroX[i][1] = 0;
        }
    FileCartesianas.close();
    
    cout<<"\n";
    cout<<"\n Valor de nXMuestraMin: "<<nXMuestraMin;
    cout<<"\n Valor de X min: "<<nXMuestraMin*deltaX+X0<<" mm";
    cout<<"\n Valor de nXMuestraMax: "<<nXMuestraMax;
    cout<<"\n Valor de X max: "<<nXMuestraMax*deltaX+X0<<" mm";
    cout<<"\n";
    
}
//***************************************************************************************
//***************************************************************************************
