/******************************************************************************
 Universidad Nacional Autonoma de Mexico
 Maestria en Ciencias Fisicas
 Proyecto Final
 
 Asignatura: Introduccion a Fisica Computacional
 Profesor: Santiago Caballero
 Estudiante: F. E. Charry-Pastrana
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

double CilindroX[nX][3];
double X;	// Contadores de valores reales
double X0, X1;
double deltaX;

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
    X0 = 0;         // mm
    X1 = +40;       // mm
    deltaX = (X1 - X0) / (nX-1);
}
//***************************************************************************************
//***************************************************************************************
void LugaresEmisionAleatorios()
{

    //---------------------------------
    ofstream myfileCapturaDistancia;
    myfileCapturaDistancia.open("Distancia_mm_emision_Y_captura.txt");
    myfileCapturaDistancia<<"#Distancia emision (mm) \t ";
    myfileCapturaDistancia<<"Distancia captura desde emision \t ";
    myfileCapturaDistancia<<"Distancia captura en detector (mm)\n";
    myfileCapturaDistancia<<fixed<<setprecision(7);
    //---------------------------------
    int NumeroEventos = 6*pow(10,5);
    int NumeroEventosDetector = 0;
    
    int nX_detenerse;
    double Probabilidad_Punto;
    double Evaluacion_Probabildad;
    double r, CaminoAntes = 0, CaminoMismoMaterial = 0;
    
    double X_distancia;
    i = 0;
    j=0;

    
    double Prob_Anterior = 1, Prob_Zona;
    double Prob_Acumulada= 0, Prob_Acumulada_Anterior = 0;
    
    double X_total_Anterior = 0;
    double X_parcial_Zona = 0;
    
    srand((int)time(0));
    while (j++ < NumeroEventos)
    {

        //------------------------
        //  Numeros aleatorios de emision en la muestra
        r = (int) (((double)rand()/(RAND_MAX))*(nXMuestraMax-nXMuestraMin) + nXMuestraMin);
        nXAux = r;
        if(j<3)     cout<<"\n Valor punto emision aleatorio (matriz): "<<nXAux;
        
        //  Coordenadas Cartesianas
        X = deltaX*nXAux + X0;
        if(j<3)    cout<<"\n Valor punto x de emision: "<<X<<" mm";
        myfileCapturaDistancia << X <<"\t";
        
        // Numero aleatorio hasta donde llegara.
        b = (double)rand()/(RAND_MAX);      // Donde cae en probabilidad
        //------------------------
        
        //  Calculando Probabilidad Acumulada
        Prob_Anterior = 1,
        Prob_Acumulada= 0;
        X_total_Anterior = 0;
        X_parcial_Zona = 0;
        a = CilindroX[0][0];
        
        for (i = nXAux; i<nX; i++)
        {
            X = deltaX*(i-nXAux) + X0;
            c = deltaX*(i) + X0;
            
            if(CilindroX[i][0] == a)    // Ver Eq. articulo
            {
                X_parcial_Zona += deltaX;
                Prob_Acumulada = Prob_Anterior*exp(-X_parcial_Zona*CilindroX[i][0]);
            }
            
            if(CilindroX[i][0] != a)
            {
                X_total_Anterior = X_parcial_Zona;
                X_parcial_Zona = 0;
                
                a = Prob_Anterior*exp(-X_total_Anterior*CilindroX[i-1][0]);
                Prob_Anterior = a;
                
                a = CilindroX[i][0];
                i = i-1;
            }
            
            if(b > Prob_Acumulada)
            {
                i = nX-1;
                CilindroX[i][2] += 1;
                if(j<3)
                {
                    cout<<"\n Valor de i: "<<i;
                    cout<<"\n Valor de b: "<<b;
                    cout<<"\n Valor de Prob Acumulada: "<<Prob_Acumulada;
                    cout<<"\n Valor de X relativo: "<<X<<" mm";
                    cout<<"\n Valor de X total: "<<c<<" mm";
                }
                
                double Distancia_Air2 = 10.5;       // mm
                double Distancia_Ge = 33.35;
                
                if(c < 33.35 && c>10.5) NumeroEventosDetector++;
            }

        }
        
        myfileCapturaDistancia<<X<<"\t"<<c<<"\n";
        
        if(j<3)     cout<<"\n .................................................";
    }
    cout<<"\n De "<<NumeroEventos<<" eventos, "<<NumeroEventosDetector;
    cout<<" se detuvieron en detector. \tPorcentaje: ";
    cout<<NumeroEventosDetector*100/NumeroEventos;
    
    myfileCapturaDistancia.close();
     
}
//***************************************************************************************
//***************************************************************************************
void GeometriaZonas()
{
    ofstream FileCartesianas;
    
    FileCartesianas.open("X_AbsorptionProbability.txt");
    FileCartesianas<<"#Valores de X \t AbsorptionProbability\n";
   
    double Limites_Distancias_1D[8][2];
    //--------------------------------------------------------------------
    //  Valores experimentales
    double densidadSoil = 1.2*pow(10,-3);   // Densidad Ge, g mm-3
    double mu_rho_Soil = 0.513*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    
    double densidadPoly = 0.9*pow(10,-3);   // Densidad Ge, g mm-3
    double mu_rho_Poly = 0.2084*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    
    double densidadAir = 1.225*pow(10,-6);   // Densidad Ge, g mm-3
    double mu_rho_Air = 0.2080*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    
    double densidadAl = 2.70*pow(10,-3);   // Densidad Ge, g mm-3
    double mu_rho_Al = 0.3681*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    
    double densidadGe = 5.32*pow(10,-3);   // Densidad Ge, g mm-3
    double mu_rho_Ge = 3.335*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    //--------------------------------------------------------------------
    //  Limites
    double Distancia_Muestra = 5;       // mm
    double Distancia_Poly = 5.5;        // mm
    double Distancia_Air1 = 7.75;       // mm
    double Distancia_Al = 8.25;         // mm
    double Distancia_Air2 = 10.5;       // mm
    double Distancia_Ge = 33.35;        // mm
    double Distancia_Air3 = X1;         // mm
    //--------------------------------------------------------------------
    //  Distancias 1D, limites inferiores y superiores en cada zona
    Limites_Distancias_1D[0][0] =  0;
    Limites_Distancias_1D[1][0] =  Distancia_Muestra;
    Limites_Distancias_1D[2][0] =  Distancia_Poly;
    Limites_Distancias_1D[3][0] =  Distancia_Air1;
    Limites_Distancias_1D[4][0] =  Distancia_Al;
    Limites_Distancias_1D[5][0] =  Distancia_Air2;
    Limites_Distancias_1D[6][0] =  Distancia_Ge;
    Limites_Distancias_1D[7][0] =  Distancia_Air3;
    
    Limites_Distancias_1D[0][1] = 0;
    for(i=1; i<8; i++)
    {
        Limites_Distancias_1D[i][1] = Limites_Distancias_1D[i][0] - Limites_Distancias_1D[i-1][0];
        cout<<"\n Diferencias de distancias: "<< Limites_Distancias_1D[i][1];
    }
    
    cout<<"\n Exponencial min: "<<exp(- densidadSoil*mu_rho_Soil*5 - 0.5*densidadPoly*mu_rho_Poly - 5*densidadAir*mu_rho_Air - 0.5*densidadAl*mu_rho_Al);
    cout<<"\n Exponencial max: "<<exp(- 1*densidadSoil*mu_rho_Soil*5 - 0.5*densidadPoly*mu_rho_Poly - 5*densidadAir*mu_rho_Air - 0.5*densidadAl*mu_rho_Al)*(1-exp(-22.85*densidadGe*mu_rho_Al));
    cout<<"\n Algo: "<<(1-exp(-2*densidadGe*mu_rho_Al));

    //--------------------------------------------------------------------
    //  Definiendo zonas
    for (i = 0; i<nX; i++)
    {
        X = deltaX*i + X0;
        //--------------------------------------------------------------------
        //  Lugar de la muestra
        if(X>=X0 &&  X<Distancia_Muestra)
        {
            CilindroX[i][0] = densidadSoil*mu_rho_Soil;
            nXAux = i;
                
            if(nXAux > nXMuestraMax) nXMuestraMax = nXAux;
            if(nXAux < nXMuestraMin) nXMuestraMin = nXAux;
        }
        //  Pared de contenedor, vial
        if(X>=Distancia_Muestra && X<Distancia_Poly)
        {
            CilindroX[i][0] = densidadPoly*mu_rho_Poly;
        }
        //  Aire 1
        if(X>= Distancia_Poly && X<Distancia_Air1)
        {
            CilindroX[i][0] = densidadAir*mu_rho_Air;;
        }
        //  Pared del detector, Al
        if(X>=Distancia_Air1 && X<Distancia_Al)
        {
            CilindroX[i][0] = densidadAl*mu_rho_Al;
        }
        //  Aire 2
        if(X>= Distancia_Al && X<Distancia_Air2)
        {
            CilindroX[i][0] = densidadAir*mu_rho_Air;;
        }
        //  Lugar del CRISTAL
        if(X>=Distancia_Air2 && X<Distancia_Ge)
        {
         CilindroX[i][0] = densidadGe*mu_rho_Ge;
        }
        //  Aire 3
        if(X>= Distancia_Ge && X<Distancia_Air3)
        {
            CilindroX[i][0] = densidadAir*mu_rho_Air;;
        }
        
        CilindroX[i][2] = 0;    // Se acumulara el No. de eventos.
    }
    //--------------------------------------------------------------------
    //  Calculando Probabilidad Acumulada.

    double Prob_Anterior = 1, Prob_Acumulada= 0;
    double X_total_Anterior = 0,X_parcial_Zona = 0;
    a = CilindroX[0][0];
    
    for (i = 1; i<nX; i++)
    {
        X = deltaX*i + X0;
        
        if(CilindroX[i][0] == a)    // Ver Eq. articulo
        {
            X_parcial_Zona += deltaX;
            Prob_Acumulada = Prob_Anterior*exp(-X_parcial_Zona*CilindroX[i][0]);
        }
        
        if(CilindroX[i][0] != a)
        {
            cout<<"\n Cambio de zona.";
            cout<<"\t X: "<<X<<" mm";
            cout<<"\n X Parcial: "<<X_parcial_Zona<<" mm";
            
            
            X_total_Anterior = X_parcial_Zona;
            X_parcial_Zona = 0;
            
            a = Prob_Anterior*exp(-X_total_Anterior*CilindroX[i-1][0]);
            Prob_Anterior = a;
            
            a = CilindroX[i][0];
            i = i-1;
        }
        
        CilindroX[i][1] = Prob_Acumulada;
        FileCartesianas << fixed<<setprecision(7);
        FileCartesianas << X << "\t" << CilindroX[i][1] <<"\n";
    }

    
    FileCartesianas.close();
    
    cout<<"\n\n Ubicacion de la muestra, Puntos de emision.";
    cout<<"\n\t Valor de nXMuestraMin: "<<nXMuestraMin;
    cout<<"\t Valor de X min: "<<nXMuestraMin*deltaX+X0<<" mm";
    cout<<"\n\t Valor de nXMuestraMax: "<<nXMuestraMax;
    cout<<"\t Valor de X max: "<<nXMuestraMax*deltaX+X0<<" mm";
    cout<<"\n\n";
    
}
//***************************************************************************************
//***************************************************************************************
