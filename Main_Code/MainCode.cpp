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
const int nRho = 200;
const int nPhi = 2;     // Solo dos valores para poder graficar lateralmente.

const int nX = 400;
const int nY = 400;
const int nZ = 400;	// No. de puntos: GRID

int nXMuestra = 0;      // Cuadritos en Z muestra
int nXMuestraMin = nZ;
int nXMuestraMax = 0;
int nYMuestra = 0;      // Cuadritos en Z muestra
int nYMuestraMin = nZ;
int nYMuestraMax = 0;
int nZMuestra = 0;      // Cuadritos en Z muestra
int nZMuestraMin = nZ;
int nZMuestraMax = 0;

int nPhiAux, nRhoAux;   // Variables aux
int nXAux, nYAux, nZAux;

double CilindroXYZ[nX][nY][nZ][2];

int i, j, k,l;				// Contadoeres
double Phi, Rho, Z, X, Y;	// Contadores de valores reales
double X0, X1, Y0, Y1, Z0, Z1;

double RadioMaterial, RadioRecipiente, RadioVacio, RadioParedes, RadioCristal;
double deltaX, deltaY, deltaZ;

int PrintValues = 0;

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
    X0 = -40;
    X1 = +40;
    Y0 = -40;
    Y1 = +40;
    Z0 = 0;
    Z1 = 69.7; // mm
    
	RadioMaterial = 5;			//cm
	RadioRecipiente = 5.2;		//cm
	RadioVacio = 5.5;			//cm
	RadioParedes = 6;			//cm
	RadioCristal = 6.5;			//cm

	// Diferenciales
    deltaX = (X1 - X0) / nX;
    deltaY = (Y1 - Y0) / nY;
    deltaZ = (Z1 - Z0) / nZ;
}
//***************************************************************************************
//***************************************************************************************
void LugaresEmisionAleatorios()
{
    //-----------------------------------------------------------------------------------
    ofstream myfilePuntosAleatorios1, myFilePuntosAleatoriosTrayectorias1;
    
    myfilePuntosAleatorios1.open("PuntosAleatorios1.txt");
    myFilePuntosAleatoriosTrayectorias1.open("PuntosAleatoriosTrayectorias1.txt");

    myfilePuntosAleatorios1<<"# nXAux \t nYAux \t nZAux \t";
    myfilePuntosAleatorios1<<"X \t Y \t Z \t Rho \t Phi \t";
    myfilePuntosAleatorios1<<"ThetaAle \t PhiAle \t ThetaMin \t";
    myfilePuntosAleatorios1<<"color\t nXFinal \t nYFinal \t nZFinal \n";
    myfilePuntosAleatorios1<<"# Todo en mm y angulos en radianes";
    
    myFilePuntosAleatoriosTrayectorias1<<"# X(mm) \t Y(mm) \t Z(mm) \t color";
    //-----------------------------------------------------------------------------------
    
    //  Contador de No. de Eventos (decaimientos) vs. Eventos que se escapan
    int NumeroEventos = 2*pow(10,2);
    int NumeroEventosEscape = 0;
    
    int r;                      // Variables auxiliares
    double ThetaAle, PhiAle;    // Angulos aleatorios
    
    //  Variables para dibujar las trayectorias
    double RadioAux;
    double RadioAuxMax = 45;    //  En mm
    double nRadioAux = 20;
    double color;
    
    double ThetaMin;    // para auxiliar para angulo descartar rayo emisor
    double RhoMax;              //  Variable Rho Ma
    double RadioPozo = 10.5;    //  Radio del pozo del detector
    
    //  Variable auxiliar
    double diffAng;
    
    l = 0;
    srand((int)time(0));
    
    double Ale1Aux, Ale2Aux;
    double a, b, c, d, e, f;
    int nXFinal, nYFinal, nZFinal;
    double ProbAcumulada=1, ProbAcumuladaAnterior=1;
    double deltaR = sqrt(pow(deltaX,2)+pow(deltaY,2)+pow(deltaZ,2));
    int nXmin, nXmax, nYmin, nYmax, nZmin, nZmax;

    int signoX, signoY, signoZ, iPrima, jPrima, kPrima;
    int iFinal;
    int jFinal;
    int kFinal;
    double nXInicial, nYInicial, nZInicial;
    
    double mx, my, mz;
    double pX, pY, pZ;
    double nPendiente;
    
    while (l++ < NumeroEventos)
    {
        //  Numeros aleatorios
        //  Se debe agregar +1 para que la funcion pueda incluir el extermo superior
        r = (int) (((double)rand()/(RAND_MAX))*(nXMuestraMax-nXMuestraMin) + nXMuestraMin);
        nXAux = r;
        nXInicial = r;
        r = (int) (((double)rand()/(RAND_MAX))*(nYMuestraMax-nYMuestraMin) + nYMuestraMin);
        nYAux = r;
        nYInicial = r;
        r = (int) (((double)rand()/(RAND_MAX))*(nZMuestraMax-nZMuestraMin) + nZMuestraMin);
        nZAux = r;
        nZInicial = r;
        
        //  Direccion aleatoria
        ThetaAle = M_PI*((double)rand()/(RAND_MAX));
        PhiAle = 2*M_PI*((double)rand()/(RAND_MAX));
        
        //  Coordenadas Cartesianas
        X = deltaX*nXAux + X0;
        Y = deltaY*nYAux + Y0;
        Z = deltaZ*nZAux + Z0;
        
        //  Coordenadas Cilindricas
        Rho = sqrt(X*X + Y*Y);
        
        
        if( X>=0 && Y>=0)   Phi = atan(Y/X);
        if( X>0 && Y<0)     Phi = 2*M_PI + atan(Y/X);
        if( X<0 && Y<0)     Phi = M_PI + atan(Y/X);
        if( X<0 && Y>0)     Phi = M_PI + atan(Y/X);
        
        if(Phi <= M_PI)     diffAng = M_PI - PhiAle + Phi;
        if(Phi > M_PI)      diffAng = PhiAle - Phi - M_PI;
        
        //  Rho máximo para un z dado (cilindricas)
        RhoMax = sqrt( pow(RadioPozo,2) - pow(Rho*sin(diffAng),2) ) + Rho*cos(diffAng);
        
        //  Valor de Theta Min para que el angulo No sea detectado
        ThetaMin = atan(RhoMax/(Z1-Z));
        color = 2;

        if(ThetaAle<=ThetaMin)
        {
            color = -2;     // Para diferencial visualmente los que se descartan
            NumeroEventosEscape ++;
            nXFinal = 1;
            nYFinal = 1;
            nZFinal = 1;
        }
        
        
        if(ThetaAle>ThetaMin)
        {
            //  a) Puntos finales aleatorios
            Ale1Aux = ((double)rand()/(RAND_MAX));
            
            nXFinal = (int) ((double)rand()*nX/(RAND_MAX));
            nYFinal = (int) ((double)rand()*nY/(RAND_MAX));
            nZFinal = (int) ((double)rand()*nZ/(RAND_MAX));
            
            pX = (int)(sqrt(pow(nXFinal-nXInicial,2)));
            pY = (int)(sqrt(pow(nYFinal-nYInicial,2)));
            pZ = (int)(sqrt(pow(nZFinal-nZInicial,2)));
            
            if(pX<=pY && pX<=pZ)    nPendiente = pX;
            if(pY<=pX && pY<=pZ)    nPendiente = pY;
            if(pZ<=pX && pZ<=pY)    nPendiente = pZ;
            
            cout<<""
            
            
            
            a = ((double)1)/((double)6);
            b = ((double)2)/((double)6);
            c = ((double)3)/((double)6);
            d = ((double)4)/((double)6);
            e = ((double)5)/((double)6);
            
            if(Ale1Aux>=0 && Ale1Aux<a)     nXFinal = 0;
            if(Ale1Aux>=a && Ale1Aux<b)     nXFinal = nX;
            if(Ale1Aux>=b && Ale1Aux<c)     nYFinal = 0;
            if(Ale1Aux>=c && Ale1Aux<d)     nYFinal = nY;
            if(Ale1Aux>=d && Ale1Aux<e)     nZFinal = 0;
            if(Ale1Aux>=e && Ale1Aux<=1)    nZFinal = nZ;
            //--------------------------------------------------------------
            //  b) Calcular probabilidad acumulada
            Ale2Aux =((double)rand()/(RAND_MAX));   // Hasta donde se detendra
            cout<<"\n Ale2Aux: "<<Ale2Aux;
            

            signoX = (int)( (nXFinal-nXAux)/(sqrt(pow(nXFinal-nXAux,2))));
            signoY = (int)( (nYFinal-nYAux)/(sqrt(pow(nYFinal-nYAux,2))));
            signoZ = (int)( (nZFinal-nZAux)/(sqrt(pow(nZFinal-nZAux,2))));
            cout <<"\n Signo X: "<< signoX;
            
            cout<<"\n\t\t nXAux: "<<nXAux<<"\t nYAux: "<<nYAux<<"\t nZAux: "<<nZAux;
            ProbAcumulada = 1;
            ProbAcumuladaAnterior = 1;
            
            iFinal = (int)(sqrt(pow(nXFinal-nXAux,2))) + 1;
            jFinal = (int)(sqrt(pow(nYFinal-nYAux,2))) + 1;
            kFinal = (int)(sqrt(pow(nZFinal-nZAux,2))) + 1;
            /*
            for(i=0; i<iFinal; i++)
            {
                for(j=0; j<jFinal; j++)
                {
                    for(k=0; k<kFinal; k++)
                    {
                        iPrima = (int)(signoX*i+nXAux);
                        jPrima = (int)(signoY*j+nYAux);
                        kPrima = (int)(signoZ*k+nZAux);
                        
                        ProbAcumulada = ProbAcumulada*exp(-CilindroXYZ[iPrima][jPrima][kPrima][0]*deltaR);
//                        cout<<"\n\t Prob.Acu.Anter: "<<ProbAcumuladaAnterior<<"\t Prob. Ac: "<<ProbAcumulada;
                        
                        if((Ale2Aux>ProbAcumuladaAnterior) && (Ale2Aux>ProbAcumulada))
                        {
                            cout<<"\n It stops in Ge at: \t\t iPrima: "<<iPrima<<"\t jPrima: "<<jPrima<<"\t kPrima: "<<kPrima;
                            CilindroXYZ[iPrima][jPrima][kPrima][1]++;  // Vamos a acumular en 1 los eventos
                            
                            i = iFinal-1;
                            j = jFinal-1;
                            k = kFinal-1;
                        }
                        
                        ProbAcumuladaAnterior = ProbAcumulada;
                    }
                }
            }
             */
            cout<<"\n\t\t iFinal: "<<iFinal<<"\t jFinal: "<<jFinal<<"\t kFinal: "<<kFinal;
        }

        myfilePuntosAleatorios1<<"\n"<<fixed<<setprecision(2);
        myfilePuntosAleatorios1<<nXAux<<"\t"<<nYAux<<"\t"<<nZAux<<"\t";
        myfilePuntosAleatorios1<<X<<"\t"<<Y<<"\t"<<Z<<"\t";
        myfilePuntosAleatorios1<<Rho<<"\t"<<Phi<<"\t";
        myfilePuntosAleatorios1<<ThetaAle<<"\t"<<PhiAle<<"\t";
        myfilePuntosAleatorios1<<ThetaMin<<"\t";
        myfilePuntosAleatorios1<<color;
        myfilePuntosAleatorios1<<"\t"<<nXFinal<<"\t"<<nYFinal<<"\t"<<nZFinal;
        
        for(j=0; j<nRadioAux; j++)    // para dibujar trayectorias
        {
            RadioAux = RadioAuxMax*j/(nRadioAux-1);
            
            myFilePuntosAleatoriosTrayectorias1<<"\n";
            myFilePuntosAleatoriosTrayectorias1<<X+RadioAux*cos(PhiAle)*sin(ThetaAle);
            myFilePuntosAleatoriosTrayectorias1<<"\t";
            myFilePuntosAleatoriosTrayectorias1<<Y+RadioAux*sin(PhiAle)*sin(ThetaAle);
            myFilePuntosAleatoriosTrayectorias1<<"\t";
            myFilePuntosAleatoriosTrayectorias1<<Z+RadioAux*cos(ThetaAle);
            myFilePuntosAleatoriosTrayectorias1<<"\t";
            myFilePuntosAleatoriosTrayectorias1<<color;
        }
    }
    cout<<"\n De "<<NumeroEventos<<" eventos, "<<NumeroEventosEscape;
    cout<<" salieron del detector. \tPorcentaje: ";
    cout<<(double)NumeroEventosEscape*100/NumeroEventos;
    
    
    double NumeroEventosGe=0;
    double densidadGe = 5.32*pow(10,-3);   // Densidad Ge, g mm-3
    double mu_rho_Ge = 3.335*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    // Vamos a contar cuantos eventos caen en el detector
    for (k = 0; k<nZ; k++)
    {
        for (j = 0; j<nY; j++)
        {
            for (i = 0; i<nX; i++)
            {
                if(CilindroXYZ[i][j][k][0] == densidadGe*mu_rho_Ge)
                {
                    NumeroEventosGe += CilindroXYZ[i][j][k][1];
                }
            }
        }
    }
    
    
    cout<<"\n \n Numero eventos Ge: "<<NumeroEventosGe;
    
    
    myfilePuntosAleatorios1.close();
    myFilePuntosAleatoriosTrayectorias1.close();
}
//***************************************************************************************
//***************************************************************************************
void GeometriaZonas()
{
	ofstream FileCartesianas1, FileCartesianas2;

    FileCartesianas1.open("XZ_Yfijo.txt");
    FileCartesianas2.open("YZ_Xfijo.txt");
    
    FileCartesianas1<<"#Valores de X y Z para Y=0 fijo\n";
    FileCartesianas2<<"#Valores de Y y Z para X=0 fijo\n";
    
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
    
    for (k = 0; k<nZ; k++)
    {
        for (j = 0; j<nY; j++)
        {
            for (i = 0; i<nX; i++)
            {
                X = deltaX*i + X0;
                Y = deltaY*j + Y0;
                Z = deltaZ*k + Z0;
                Rho = sqrt(X*X + Y*Y);  // Cilindrical coordinates
                //--------------------------------------------------------------------
                //--------------------------------------------------------------------
                //	Si no se define, que sea aire. Aire = 0
                CilindroXYZ[i][j][k][0] = densidadAir*mu_rho_Air;
                CilindroXYZ[i][j][k][1] = 0;    // Vamos a acumular numero de eventos
                //--------------------------------------------------------------------
                //	Definiendo lugar del cristal. Cristal Ge = 1
                if ((Rho <= 33.35) && (Z >= 0) && (Z<12.8))
                {
                    CilindroXYZ[i][j][k][0] = densidadSoil*mu_rho_Soil;
                }
                if ((Rho >= 10.5) && (Rho <= 33.35) && (Z >= 12.8) && (Z<69.7))
                {
                    CilindroXYZ[i][j][k][0] = densidadSoil*mu_rho_Soil;
                }
                //--------------------------------------------------------------------
                //	Definiendo lugar Copper. Aluminio = 2
                if ((Rho<=8.25) && (Z >= 28.7) && (Z<29.7))
                {
                    CilindroXYZ[i][j][k][0] = densidadAl*mu_rho_Al;
                }
                if ((Rho >= 7.75) && (Rho<=8.25) && (Z >= 29.7) && (Z<69.7))
                {
                    CilindroXYZ[i][j][k][0] = densidadAl*mu_rho_Al;
                }
                //--------------------------------------------------------------------
                //	Definiendo contenedor de plastico. Plastico = - 2
                if ((Rho <= 5.5) && (Z >= 29.7) && (Z<30.5))
                {
                    CilindroXYZ[i][j][k][0] = densidadPoly*mu_rho_Poly;
                }
                
                if ((Rho <= 5.5) && (Z >= 59.2) && (Z<59.7))
                {
                    CilindroXYZ[i][j][k][0] = densidadPoly*mu_rho_Poly;
                }
                
                if ((Rho >= 5) && (Rho <= 5.5) && (Z >= 30.2) && (Z<59.2))
                {
                    CilindroXYZ[i][j][k][0] = densidadPoly*mu_rho_Poly;
                }
                //--------------------------------------------------------------------
                //	Definiendo muestra. Muestra = -1.5
                //  Definiendo los límites en donde se encuentra la muestra
                if ((Rho <= 5.0) && (Z >= 30.2) && (Z<55.2))
                {
                    CilindroXYZ[i][j][k][0] = densidadSoil*mu_rho_Soil;
        
                    nXAux = i;
                    nYAux = j;
                    nZAux = k;

                    //  Se utiliza para saber el lugar de la muestra
                    if(nXAux > nXMuestraMax) nXMuestraMax = nXAux;
                    if(nXAux < nXMuestraMin) nXMuestraMin = nXAux;
                    
                    if(nYAux > nYMuestraMax) nYMuestraMax = nYAux;
                    if(nYAux < nYMuestraMin) nYMuestraMin = nYAux;
                    
                    if(nZAux > nZMuestraMax) nZMuestraMax = nZAux;
                    if(nZAux < nZMuestraMin) nZMuestraMin = nZAux;

                }
                //--------------------------------------------------------------------
                //	Definiendo Material encima de la Muestra y dentro del contenedor.
                //  Atmosfera de He. He = 1.5
                if ((Rho <= 5.0) && (Z >= 55.2) && (Z<59.2))
                {
                    CilindroXYZ[i][j][k][0] = 1.5;
                }
                //--------------------------------------------------------------------
                //--------------------------------------------------------------------
                //  Escribiendo en archivo
                if (Y == 0)	// Para valor de y fijo
                {
                    FileCartesianas1 << X << "\t" << Z << "\t";
                    FileCartesianas1 << CilindroXYZ[i][j][k][0] << "\n";
                }
                
                if (X == 0)	// Para valor de x fijo
                {
                    FileCartesianas2 << Y << "\t" << Z << "\t";
                    FileCartesianas2 << CilindroXYZ[i][j][k][0] << "\n";
                }
            }
        }
    }
    
    FileCartesianas1.close();
    FileCartesianas2.close();
}
//***************************************************************************************
//***************************************************************************************
