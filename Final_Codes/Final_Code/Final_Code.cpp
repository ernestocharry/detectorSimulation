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
#include <string>
#include <math.h>
#include <time.h>
#include <random>

#include <string>

// Para interpolar
#include <cstdio>
#include <cstdlib>
#include <vector>


using namespace std;

//	Global variables
const int nX = 500;
const int nY = 500;
const int nZ = 500;	// No. de puntos: GRID

int nXMuestra = 0;      // Cuadritos en Z muestra
int nXMuestraMin = nZ;
int nXMuestraMax = 0;
int nYMuestra = 0;      // Cuadritos en Z muestra
int nYMuestraMin = nZ;
int nYMuestraMax = 0;
int nZMuestra = 0;      // Cuadritos en Z muestra
int nZMuestraMin = nZ;
int nZMuestraMax = 0;


int nXAux, nYAux, nZAux;

double CilindroXYZ[nX][nY][nZ][2];

int i, j, k, l;				// Contadoeres
double Phi, Rho, Z, X, Y;	// Contadores de valores reales
double X0, X1, Y0, Y1, Z0, Z1;

double deltaX, deltaY, deltaZ;

int ProblemRastreo = 0;     // Problemas en rastreo del rayo

//	Functions 
void GeometriaZonas();
void ValoresIniciales();
void LugaresEmisionAleatorios();
void DondeSeDetieneRadi(int nXInicial, int nYInicial, int nZInicial, double mX,  double mY, double mZ, int nMin);
double InterGe2(int CompFoto, double Energia);  //Extrapola lineal
int RastreoCompton(int nXInicial, int nYInicial, int nZInicial, int nXInicialAnterior, int nYInicialAnterior, int nZInicialAnterior, int nXFinalInicial, int nYFinalInicial, int nZFinalInicial);
//--------------------------------------------------------------------

//--------------------------------------------------------------------
//  Valores experimentales
//  A 46.54 keV keV
int Natenuacion = 25;
int NSelection;             //  Numeros de No. en el intervalo
double PosiblesEnergias[25];
double AtenuacionSoil[25];
double AtenuacionPoly[25];
double AtenuacionAir[25];
double AtenuacionAl[25];




double EnergiaGammaInicial = PosiblesEnergias[NSelection]; //MeV !!!!!

double mu_rho_Soil;   // mu sobre rho a 5 keV,  mm2 g-1
double mu_rho_Poly;   // mu sobre rho a 5 keV,  mm2 g-1
double mu_rho_Air ;   // mu sobre rho a 5 keV,  mm2 g-1
double mu_rho_Al;   // mu sobre rho a 5 keV,  mm2 g-1
double mu_rho_Ge;   // mu sobre rho a 5 keV,  mm2 g-1

double densidadSoil = 1*pow(10,-3);   // Densidad Ge, g mm-3
double densidadPoly = 1.3*pow(10,-3);   // Densidad Ge, g mm-3
double densidadAir = 1.225*pow(10,-6);   // Densidad Ge, g mm-3
double densidadAl = 2.70*pow(10,-3);   // Densidad Ge, g mm-3
double densidadGe = 5.32*pow(10,-3);   // Densidad Ge, g mm-3

//--------------------------------------------------------------------
int  NumeroEventos = (1*pow(10,5));
//--------------------------------------------------------------------

int NumeroEventosSoil = 0;    // No. de Eventos en detectores
int NumeroEventosPoly = 0;    // No. de Eventos en detectores
int NumeroEventosAir = 0;    // No. de Eventos en detectores
int NumeroEventosAl = 0;    // No. de Eventos en detectores
int NumeroEventosGe = 0;    // No. de Eventos en detectores

int NumeroEventosGeFoto = 0;    // No. de Eventos en detectores
int NumeroEventosGeCompton = 0;    // No. de Eventos en detectores


int NumeroEventosRegistrados = 0;     // No. de Eventos escapan del detector

int C2Compton   = 0;
int C2Foto      = 0;
int C2Salio     = 0;
int C2Problemas = 0;
//***************************************************************************************
//***************************************************************************************
int main()
{
    PosiblesEnergias[0] = 0.001;	AtenuacionSoil[0] = 4078;	AtenuacionPoly[0] = 1894;	AtenuacionAir[0] = 3606;	AtenuacionAl[0] = 1185;
    PosiblesEnergias[1] = 0.0015;	AtenuacionSoil[1] = 1376;	AtenuacionPoly[1] = 599.9;	AtenuacionAir[1] = 1191;	AtenuacionAl[1] = 402.2;
    PosiblesEnergias[2] = 0.002;	AtenuacionSoil[2] = 617.3;	AtenuacionPoly[2] = 259.3;	AtenuacionAir[2] = 527.9;	AtenuacionAl[2] = 2263;
    PosiblesEnergias[3] = 0.003;	AtenuacionSoil[3] = 192.9;	AtenuacionPoly[3] = 77.43;	AtenuacionAir[3] = 162.5;	AtenuacionAl[3] = 788;
    PosiblesEnergias[4] = 0.004;	AtenuacionSoil[4] = 82.78;	AtenuacionPoly[4] = 32.42;	AtenuacionAir[4] = 77.88;	AtenuacionAl[4] = 360.5;
    PosiblesEnergias[5] = 0.005;	AtenuacionSoil[5] = 42.58;	AtenuacionPoly[5] = 16.43;	AtenuacionAir[5] = 40.27;	AtenuacionAl[5] = 193.4;
    PosiblesEnergias[6] = 0.006;	AtenuacionSoil[6] = 24.64;	AtenuacionPoly[6] = 9.432;	AtenuacionAir[6] = 23.41;	AtenuacionAl[6] = 115.3;
    PosiblesEnergias[7] = 0.008;	AtenuacionSoil[7] = 10.37;	AtenuacionPoly[7] = 3.975;	AtenuacionAir[7] = 9.921;	AtenuacionAl[7] = 50.33;
    PosiblesEnergias[8] = 0.01;	AtenuacionSoil[8] = 5.329;	AtenuacionPoly[8] = 2.088;	AtenuacionAir[8] = 5.12;	AtenuacionAl[8] = 26.23;
    PosiblesEnergias[9] = 0.015;	AtenuacionSoil[9] = 1.673;	AtenuacionPoly[9] = 0.7452;	AtenuacionAir[9] = 1.614;	AtenuacionAl[9] = 7.955;
    PosiblesEnergias[10] = 0.02;	AtenuacionSoil[10] = 0.8096;	AtenuacionPoly[10] = 0.4315;	AtenuacionAir[10] = 0.7779;	AtenuacionAl[10] = 3.441;
    PosiblesEnergias[11] = 0.03;	AtenuacionSoil[11] = 0.3756;	AtenuacionPoly[11] = 0.2706;	AtenuacionAir[11] = 0.3538;	AtenuacionAl[11] = 1.128;
    PosiblesEnergias[12] = 0.04;	AtenuacionSoil[12] = 0.2683;	AtenuacionPoly[12] = 0.2275;	AtenuacionAir[12] = 0.2485;	AtenuacionAl[12] = 0.5685;
    PosiblesEnergias[13] = 0.05;	AtenuacionSoil[13] = 0.2269;	AtenuacionPoly[13] = 0.2084;	AtenuacionAir[13] = 0.208;	AtenuacionAl[13] = 0.3681;
    PosiblesEnergias[14] = 0.06;	AtenuacionSoil[14] = 0.2059;	AtenuacionPoly[14] = 0.197;	AtenuacionAir[14] = 0.1875;	AtenuacionAl[14] = 0.2778;
    PosiblesEnergias[15] = 0.08;	AtenuacionSoil[15] = 0.1837;	AtenuacionPoly[15] = 0.1823;	AtenuacionAir[15] = 0.1662;	AtenuacionAl[15] = 0.2018;
    PosiblesEnergias[16] = 0.1;	AtenuacionSoil[16] = 0.1707;	AtenuacionPoly[16] = 0.1719;	AtenuacionAir[16] = 0.1541;	AtenuacionAl[16] = 0.1704;
    PosiblesEnergias[17] = 0.15;	AtenuacionSoil[17] = 0.1505;	AtenuacionPoly[17] = 0.1534;	AtenuacionAir[17] = 0.1356;	AtenuacionAl[17] = 0.1378;
    PosiblesEnergias[18] = 0.2;	AtenuacionSoil[18] = 0.137;	AtenuacionPoly[18] = 0.1402;	AtenuacionAir[18] = 0.1233;	AtenuacionAl[18] = 0.1223;
    PosiblesEnergias[19] = 0.3;	AtenuacionSoil[19] = 0.1186;	AtenuacionPoly[19] = 0.1217;	AtenuacionAir[19] = 0.1067;	AtenuacionAl[19] = 0.1042;
    PosiblesEnergias[20] = 0.4;	AtenuacionSoil[20] = 0.1061;	AtenuacionPoly[20] = 0.1089;	AtenuacionAir[20] = 0.09549;	AtenuacionAl[20] = 0.09276;
    PosiblesEnergias[21] = 0.5;	AtenuacionSoil[21] = 0.09687;	AtenuacionPoly[21] = 0.09947;	AtenuacionAir[21] = 0.08712;	AtenuacionAl[21] = 0.08445;
    PosiblesEnergias[22] = 0.6;	AtenuacionSoil[22] = 0.08956;	AtenuacionPoly[22] = 0.09198;	AtenuacionAir[22] = 0.08055;	AtenuacionAl[22] = 0.07802;
    PosiblesEnergias[23] = 0.8;	AtenuacionSoil[23] = 0.07865;	AtenuacionPoly[23] = 0.08078;	AtenuacionAir[23] = 0.07074;	AtenuacionAl[23] = 0.06841;
    PosiblesEnergias[24] = 1;	AtenuacionSoil[24] = 0.07072;	AtenuacionPoly[24] = 0.07262;	AtenuacionAir[24] = 0.06358;	AtenuacionAl[24] = 0.06146;
    
    NSelection = 24;
    
    EnergiaGammaInicial = PosiblesEnergias[NSelection]; //MeV !!!!!
    
    mu_rho_Soil  = AtenuacionSoil[NSelection]*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    mu_rho_Poly  = AtenuacionPoly[NSelection]*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    mu_rho_Air   = AtenuacionAir[NSelection]*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    mu_rho_Al    = AtenuacionAl[NSelection]*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    mu_rho_Ge    = (InterGe2(0,EnergiaGammaInicial)+InterGe2(1,EnergiaGammaInicial))*pow(10,2);   // mu sobre rho a 5 keV,  mm2 g-1
    
    ofstream Resultados, Resultados2;
    
    Resultados.open("Resultados_Todo.txt",std::ios::out | std::ios::app);
    Resultados2.open("Resultados_Resumen.txt",std::ios::out | std::ios::app);
    
    Resultados<<EnergiaGammaInicial<<"\t";
    Resultados<<NumeroEventos<<"\t";
    Resultados2<<EnergiaGammaInicial<<"\t";
    Resultados<<nX<<"\t"<<nY<<"\t"<<nZ<<"\t";
    
    
    int l2, l3;
    int promedio = 10;       // Para calcular promedios
    double Registro[10][promedio];
    double Promedios[10][2];    // En 0 va el valor y 1 va la desviación
    
    for(l2 = 0; l2<promedio; l2++)
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
        

        cout<<"\n\n Eventos totales iniciales: "<<NumeroEventos;
        cout<<"\n Problemas rastreo: "<<ProblemRastreo;
        NumeroEventos = NumeroEventos - ProblemRastreo;
        cout<<"\n Eventos netos: "<<NumeroEventos;
        
        
        cout<<"\n\n Eventos eventos Registrados: "<<NumeroEventosRegistrados;
        
        
        cout<<"\n\n Porcentaje eventos Ge: "<<(double)NumeroEventosGe*100/NumeroEventos;
        cout<<"\n\n Porcentaje eventos Ge - FOTO: "<<(double)NumeroEventosGeFoto*100/NumeroEventos;
        cout<<"\n Porcentaje eventos Ge - Compton: "<<(double)NumeroEventosGeCompton*100/NumeroEventos;
        
        Registro[0][l2] = (double)NumeroEventosGeFoto*100/NumeroEventos;        // Foto 1
        Registro[1][l2] = (double)NumeroEventosGeCompton*100/NumeroEventos;     // Compton 1
        
        if(NumeroEventosGeCompton==0)   NumeroEventosGeCompton = 1;
        
        cout<<"\n\t\t Porcentaje C2, Foto:  "<<(double)C2Foto*100/NumeroEventosGeCompton;
        cout<<"\n\t\t Porcentaje C2, Compton:  "<<(double)C2Compton*100/NumeroEventosGeCompton;
        cout<<"\n\t\t Porcentaje C2, Salio:  "<<(double)C2Salio*100/NumeroEventosGeCompton;
        cout<<"\n\t\t Porcentaje C2, Problm:  "<<(double)C2Problemas*100/NumeroEventosGeCompton;
        
        
        cout<<"\n\n Porcentaje eventos Soil: "<<(double)NumeroEventosSoil*100/NumeroEventos;
        cout<<"\n Porcentaje eventos Al: "    <<(double)NumeroEventosAl*100/NumeroEventos;
        cout<<"\n Porcentaje eventos Poly: "  <<(double)NumeroEventosPoly*100/NumeroEventos;
        cout<<"\n Porcentaje eventos Aire: "  <<(double)NumeroEventosAir*100/NumeroEventos;
        
        cout<<"\n\n";
        
        
        

        
        Registro[2][l2] = (double)C2Foto*100/NumeroEventosGeCompton;            // Foto 2
        Registro[3][l2] = (double)C2Compton*100/NumeroEventosGeCompton;         // Compton 2
        Registro[4][l2] = (double)C2Salio*100/NumeroEventosGeCompton;           // Sale C2
        Registro[5][l2] = (double)C2Problemas*100/NumeroEventosGeCompton;       // Problemas C2
        Registro[6][l2] = (double)NumeroEventosSoil*100/NumeroEventos;          // Muestra
        Registro[7][l2] = (double)NumeroEventosAl*100/NumeroEventos;            // Al
        Registro[8][l2] = (double)NumeroEventosPoly*100/NumeroEventos;          // Poly
        Registro[9][l2] = (double)NumeroEventosAir*100/NumeroEventos;           // Aire
    
        cout << "\n 3) LugaresEmisionAleatorios: Fin y O.K.";
    
        NumeroEventos = (1*pow(10,5));
        ProblemRastreo = 0;
        NumeroEventosSoil = 0;    // No. de Eventos en detectores
        NumeroEventosPoly = 0;    // No. de Eventos en detectores
        NumeroEventosAir = 0;    // No. de Eventos en detectores
        NumeroEventosAl = 0;    // No. de Eventos en detectores
        NumeroEventosGe = 0;    // No. de Eventos en detectores
        NumeroEventosGeFoto = 0;    // No. de Eventos en detectores
        NumeroEventosGeCompton = 0;    // No. de Eventos en detectores
        NumeroEventosRegistrados = 0;     // No. de Eventos escapan del detector
        
        C2Compton   = 0;
        C2Foto      = 0;
        C2Salio     = 0;
        C2Problemas = 0;
        
        cout<<"\n __________________________________________________________________________";
        cout<<"\n Acabo de terminar el programa "<<l2+1<<" de "<<promedio<<" para calcular promedios";
        cout<<"\n __________________________________________________________________________";
    }
    
    Resultados<<X0<<"\t"<<X1<<"\t"<<Y0<<"\t"<<Y1<<"\t"<<Z0<<"\t"<<Z1<<"\t";
    Resultados<<densidadSoil<<"\t"<<densidadPoly<<"\t"<<densidadAir<<"\t"<<densidadAl<<"\t"<<densidadGe<<"\t";
    Resultados<<mu_rho_Soil<<"\t"<<mu_rho_Poly<<"\t"<<mu_rho_Air<<"\t"<<mu_rho_Al<<"\t"<<mu_rho_Ge<<"\t";
    
    cout<<"\n Selector en Energias y Atenuacion: "<<NSelection;
    cout<<"\n E Gamma Inicial: "<<EnergiaGammaInicial<<" MeV.";
    cout<<"\t Mu Ge: "<<mu_rho_Ge;
    
    for(l3 = 0; l3<10; l3++)    // Inicializar valores
    {
        Promedios[l3][0] = 0;
        Promedios[l3][1] = 0;
    }

    for(l2 = 0; l2<promedio; l2++)  for(l3 = 0; l3<10; l3++)    Promedios[l3][0] += Registro[l3][l2];
    for(l3 = 0; l3<10; l3++)    Promedios[l3][0] = Promedios[l3][0]/promedio;       // Promedios
    for(l2 = 0; l2<promedio; l2++)  for(l3 = 0; l3<10; l3++)    Promedios[l3][1] += pow(Registro[l3][l2]-Promedios[l3][0],2);
    for(l3 = 0; l3<10; l3++)    Promedios[l3][1] = sqrt( Promedios[l3][1]/promedio);    // Desviaciones
    
    cout<<"\n\n\n RESULTADOS\n";
    for(l3 = 0; l3<10; l3++)
    {
        cout<<"\n Para "<<l3<<" : "<<Promedios[l3][0]<<" +o- "<<Promedios[l3][1];
        Resultados<<Promedios[l3][0]<<"\t"<<Promedios[l3][1]<<"\t";
        Resultados2<<Promedios[l3][0]<<"\t"<<Promedios[l3][1]<<"\t";
    }
    

    double Foto1, Compton1, Foto2, FotoTotal;  // Valores promedios
    double SFoto1, SCompton1, SFoto2, SFotoTotal;        // Sigma
    
    Foto1 = Promedios[0][0];
    Compton1 = Promedios[1][0];
    Foto2 =  Promedios[2][0]/100;
    FotoTotal = Foto1+Compton1*Foto2;

    SFoto1 = Promedios[0][1];
    SCompton1 = Promedios[1][1];
    SFoto2 =  Promedios[2][1]/100;
    SFotoTotal = sqrt( pow(SFoto1,2) + pow(Compton1*SFoto2,2) + pow(Foto2*SCompton1,2));
    
    cout<<"\n\n Eficiencia a FotoPico:\t" <<FotoTotal<<"\t+o-\t"<< SFotoTotal;
    Resultados<<FotoTotal<<"\t"<<SFotoTotal<<"\n";
    Resultados2<<FotoTotal<<"\t"<<SFotoTotal<<"\n";
    
    
    cout<<"\n\n";
    Resultados.close();
    Resultados2.close();
    
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
    Z1 = 80; // mm

	// Diferenciales
    deltaX = (X1 - X0) / nX;
    deltaY = (Y1 - Y0) / nY;
    deltaZ = (Z1 - Z0) / nZ;
}
//***************************************************************************************
//***************************************************************************************
void LugaresEmisionAleatorios()
{
    //  Contador de No. de Eventos (decaimientos) vs. Eventos que se escapan

    double ThetaAle, PhiAle;    // Angulos aleatorios

    double ThetaMin;    // para auxiliar para angulo descartar rayo emisor
    double RhoMax;              //  Variable Rho Ma
    double RadioPozo = 10.5;    //  Radio del pozo del detector
    double diffAng;
    
    int l2 = 0;

    srand((int)time(0));
    
    double Ale1Aux, Ale2Aux;
    double a, b, c, d, e, f, r;
    
    int nXInicial, nYInicial, nZInicial;
    int nXFinal, nYFinal, nZFinal;
    
    double ProbAcumulada;
    double deltaR;

    double mX, mY, mZ;  //  Pendientes
    int pX, pY, pZ;     // Distancias
    int nMin;           // Pendiente min
    int nX1, nY1, nZ1;  // Auxiliar


    
    l2 = 0;
    a = ((double)1)/((double)6);
    b = ((double)2)/((double)6);
    c = ((double)3)/((double)6);
    d = ((double)4)/((double)6);
    e = ((double)5)/((double)6);
    
    while (l2 < NumeroEventos)
    {
        //  Numeros aleatorios
        //  Se debe agregar +1 para que la funcion pueda incluir el extermo superior
        
        nXInicial = (int) (((double)rand()/(RAND_MAX))*(nXMuestraMax-nXMuestraMin) + nXMuestraMin);
        nYInicial = (int) (((double)rand()/(RAND_MAX))*(nYMuestraMax-nYMuestraMin) + nYMuestraMin);
        nZInicial = (int) (((double)rand()/(RAND_MAX))*(nZMuestraMax-nZMuestraMin) + nZMuestraMin);

        nXFinal = (int) ((double)rand()*nX/(RAND_MAX));
        nYFinal = (int) ((double)rand()*nY/(RAND_MAX));
        nZFinal = (int) ((double)rand()*nZ/(RAND_MAX));


        Ale1Aux =((double)rand()/(RAND_MAX));

        if(Ale1Aux>=0 && Ale1Aux<a)     nXFinal = 0;
        if(Ale1Aux>=a && Ale1Aux<b)     nXFinal = nX-1;
        if(Ale1Aux>=b && Ale1Aux<c)     nYFinal = 0;
        if(Ale1Aux>=c && Ale1Aux<d)     nYFinal = nY-1;
        if(Ale1Aux>=d && Ale1Aux<e)     nZFinal = 0;
        if(Ale1Aux>=e && Ale1Aux<=1)    nZFinal = nZ-1;
        
        
        pX = (int)(sqrt(pow(nXFinal-nXInicial,2)));
        pY = (int)(sqrt(pow(nYFinal-nYInicial,2)));
        pZ = (int)(sqrt(pow(nZFinal-nZInicial,2)));
            
        if(pX<=pY && pX<=pZ)    nMin = pX;
        if(pY<=pX && pY<=pZ)    nMin = pY;
        if(pZ<=pX && pZ<=pY)    nMin = pZ;
            
        mX = (double)(nXFinal-nXInicial)/nMin;
        mY = (double)(nYFinal-nYInicial)/nMin;
        mZ = (double)(nZFinal-nZInicial)/nMin;
            
        DondeSeDetieneRadi( nXInicial,  nYInicial,  nZInicial,  mX,   mY,  mZ,  nMin);
 
        l2++;
    }
    

    
}
//***************************************************************************************
//***************************************************************************************
void DondeSeDetieneRadi(int nXInicial, int nYInicial, int nZInicial, double mX,  double mY, double mZ, int nMin)
{
    double a, b, ProbAcumulada;
    
    double ProbCompFoto;
    
    double Ale1Aux, Ale2Aux, deltaR;
    
    Ale2Aux =((double)rand()/(RAND_MAX));   // Hasta donde se detendra
    deltaR = (double)sqrt(pow(deltaX*mX,2)+pow(deltaY*mY,2)+pow(deltaZ*mZ,2));
    
    int nX1, nY1, nZ1;
    
    ProbAcumulada = 1;

    ProbCompFoto = InterGe2(0,EnergiaGammaInicial)/(InterGe2(0,EnergiaGammaInicial)+InterGe2(1,EnergiaGammaInicial));

                                 
  //  cout<<"\n \n Energia inicial: "<<EnergiaGammaInicial<<"\t Mu original: "<<InterGe2(0,EnergiaGammaInicial)+InterGe2(1,EnergiaGammaInicial);
    
    
    int nXInicialAnterior = 0;
    int nYInicialAnterior = 0;
    int nZInicialAnterior = 0;
    
    int C2=10;
    
    int nXFinalInicial, nYFinalInicial, nZFinalInicial;
    
    nXFinalInicial = (int) (mX*nMin + nXInicial);
    nYFinalInicial = (int) (mY*nMin + nYInicial);
    nZFinalInicial = (int) (mZ*nMin + nZInicial);
    
    // cout<<"\n Valores Finales Iniciales: "<<nXFinalInicial<<"\t"<<nYFinalInicial<<"\t"<<nZFinalInicial;
    for(i=0; i<(nMin+1); i++)
    {
        nX1 = (int) (mX*i + nXInicial);
        nY1 = (int) (mY*i + nYInicial);
        nZ1 = (int) (mZ*i + nZInicial);

        if((sqrt(pow(nX1,2))<nX) && (sqrt(pow(nY1,2))<nY) && (sqrt(pow(nZ1,2))<nZ))
        {
            a = ProbAcumulada;
            ProbAcumulada = a*exp(-deltaR*CilindroXYZ[nX1][nY1][nZ1][0]);
            
            if(ProbAcumulada<Ale2Aux)
            {
                CilindroXYZ[nX1][nY1][nZ1][1] += 1;
                NumeroEventosRegistrados++;
                
                i = nMin;

                if(CilindroXYZ[nX1][nY1][nZ1][0] ==  densidadSoil*mu_rho_Soil )     NumeroEventosSoil ++;
                if(CilindroXYZ[nX1][nY1][nZ1][0] ==  densidadPoly*mu_rho_Poly )     NumeroEventosPoly ++;
                if(CilindroXYZ[nX1][nY1][nZ1][0] ==  densidadAir*mu_rho_Air )       NumeroEventosAir ++;
                if(CilindroXYZ[nX1][nY1][nZ1][0] ==  densidadAl*mu_rho_Al )         NumeroEventosAl ++;
                if(CilindroXYZ[nX1][nY1][nZ1][0] ==  densidadGe*mu_rho_Ge )
                {
                    NumeroEventosGe ++;
                    Ale1Aux =((double)rand()/(RAND_MAX));   // Compton o Foto
                    if(Ale1Aux>=ProbCompFoto)
                    {
                        NumeroEventosGeCompton++;
                   //     cout<<"\t Proba Acumulada: "<<ProbAcumulada<<"\t Aleato: "<<Ale2Aux<<"\t Delta R: "<<deltaR<<"\t Pro Comp: "<<ProbCompFoto;
                      
                        
                        C2 =RastreoCompton(nX1, nY1, nZ1, nXInicialAnterior, nYInicialAnterior, nZInicialAnterior, nXFinalInicial, nYFinalInicial, nZFinalInicial);

                        if(C2==0)   C2Foto++;
                        if(C2==1)   C2Compton++;
                        if(C2==2)   C2Salio++;
                        if(C2==3)   C2Problemas++;
                    }
                    else
                    {
                        NumeroEventosGeFoto++;
                    }
                }
            }
        }
        
        if(sqrt(pow(nX1,2))>nX || (sqrt(pow(nY1,2))>nY) || (sqrt(pow(nZ1,2))>nZ))
        {
            ProblemRastreo++;// Presentaron problemas
            i = nMin;
        }
        nXInicialAnterior = nX1;
        nYInicialAnterior = nY1;
        nZInicialAnterior = nZ1;
    }
    

    
}
//***************************************************************************************
//***************************************************************************************

int RastreoCompton(int nXInicial, int nYInicial, int nZInicial, int nXInicialAnterior, int nYInicialAnterior, int nZInicialAnterior, int nXFinalInicial, int nYFinalInicial, int nZFinalInicial)
{
    double Ale1Aux, Ale2Aux, deltaR;
    
    double a, b, c, d, e, f, r;
    int nXFinal, nYFinal, nZFinal;
    double mX, mY, mZ;  //  Pendientes
    int pX, pY, pZ;     // Distancias
    int nMin;           // Pendiente min
    int nX1, nY1, nZ1;  // Auxiliar
    
    nXFinal = (int) ((double)rand()*nX/(RAND_MAX));
    nYFinal = (int) ((double)rand()*nY/(RAND_MAX));
    nZFinal = (int) ((double)rand()*nZ/(RAND_MAX));
    
  //  nXFinal = nXFinalInicial+1;
  //  nYFinal = nYFinalInicial+1;
  //  nZFinal = nZFinalInicial+1;
    
    
    if(nXFinalInicial==nX-1 || nXFinalInicial==0)   nXFinal = nXFinalInicial;
    if(nYFinalInicial==nY-1 || nYFinalInicial==0)   nYFinal = nYFinalInicial;
    if(nZFinalInicial==nZ-1 || nZFinalInicial==0)   nZFinal = nZFinalInicial;
    
    
    
    pX = (int)(sqrt(pow(nXFinal-nXInicial,2)));
    pY = (int)(sqrt(pow(nYFinal-nYInicial,2)));
    pZ = (int)(sqrt(pow(nZFinal-nZInicial,2)));
    
    if(pX<=pY && pX<=pZ)    nMin = pX;
    if(pY<=pX && pY<=pZ)    nMin = pY;
    if(pZ<=pX && pZ<=pY)    nMin = pZ;
    
    mX = (double)(nXFinal-nXInicial)/nMin;
    mY = (double)(nYFinal-nYInicial)/nMin;
    mZ = (double)(nZFinal-nZInicial)/nMin;
    
    double CosTheta, norma1, norma2, Theta;
    
    //Producto Punto
    double vec1x, vec1y, vec1z;
    double vec2x, vec2y, vec2z;
    
    vec1x = deltaX*(nXInicial- nXInicialAnterior);
    vec1y = deltaY*(nXInicial- nXInicialAnterior);
    vec1z = deltaZ*(nXInicial- nXInicialAnterior);
    
    vec2x = deltaX*(nXFinal-nXInicial);
    vec2y = deltaY*(nYFinal-nYInicial);
    vec2z = deltaZ*(nZFinal-nZInicial);
    
    
    norma1 = sqrt(pow(vec1x,2) + pow(vec1y,2) + pow(vec1z,2));
    norma2 = sqrt(pow(vec2x,2) + pow(vec2y,2) + pow(vec2z,2));
    
    CosTheta = (vec1x*vec2x + vec1y*vec2y + vec1z*vec2z)/(norma2*norma1); // Cuidado!
    
    double NuevaEnergia;
    NuevaEnergia = EnergiaGammaInicial/(1+(1-CosTheta)*EnergiaGammaInicial/0.5);

  //  NuevaEnergia = 0.3;
    
    Ale2Aux =((double)rand()/(RAND_MAX));   // Hasta donde se detendra
    deltaR = (double)sqrt(pow(deltaX*mX,2)+pow(deltaY*mY,2)+pow(deltaZ*mZ,2));
    
    
    double ProbAcumulada, ProbCompFoto;
    
    
    ProbAcumulada = 1;
    
    int Respuesta=10;

    
    
    
    double muFoto, muCompton, nuevoMu;
    muFoto = InterGe2(0,NuevaEnergia)*pow(10,2);
    muCompton = InterGe2(1,NuevaEnergia)*pow(10,2);
    nuevoMu = muFoto+muCompton;
    ProbCompFoto =muFoto/nuevoMu;

    

    
    /*
     
     muFoto = -1;
     muCompton = -1;
    
    cout<<"\nC2: \n";
    cout<<"\t Valores iniciales x: "<<nXInicial<<"\t inicial Anterior x: "<<nXInicialAnterior;
    cout<<"\t Valores finales x: "<<(mX*nMin + nXInicial)<<"\t final Anterior x: "<<nXFinalInicial;
    cout<<"\n";
    cout<<"\t Valores iniciales y: "<<nYInicial<<"\t inicial Anterior x: "<<nYInicialAnterior;
    cout<<"\t Valores finales Y: "<<(mY*nMin + nYInicial)<<"\t final Anterior x: "<<nYFinalInicial;
    cout<<"\n";
    cout<<"\t Valores iniciales Z: "<<nZInicial<<"\t inicial Anterior x: "<<nZInicialAnterior;
    cout<<"\t Valores finales Z: "<<(mZ*nMin + nZInicial)<<"\t final Anterior Z: "<<nZFinalInicial;
    cout<<"\n";
    

*/


    Ale1Aux =((double)rand()/(RAND_MAX));   // Compton o Foto

    if(muCompton>0 && muFoto>0)
    {
        for(int ii=0; ii<(nMin+1); ii++)
        {
            nX1 = (int) (mX*ii + nXInicial);
            nY1 = (int) (mY*ii + nYInicial);
            nZ1 = (int) (mZ*ii + nZInicial);
            
            if(sqrt(pow(nX1,2))>nX)
            {
                ii = nMin;
                Respuesta = 3;    // Descartar por problemas de rastreo
            }
            else{
                if(CilindroXYZ[nX1][nY1][nZ1][0] ==  densidadGe*mu_rho_Ge  )
                {
                    
                    //      cout<<"\n Ge\t Prob: "<< ProbAcumulada<<"\t No. Ale: "<<Ale2Aux;
                    //        cout<<"\t Nueva Ener: "<<NuevaEnergia<<"\t Nuevo mu:"<<nuevoMu<<"\t Angle: "<<acos(CosTheta)*180/3.1416<<"\t Delta R: "<<deltaR<<"\t Prob Comp: "<<muCompton/nuevoMu;
                    //                cout<<"\t Prob Foto: "<<muFoto/nuevoMu;
                    
                    a = ProbAcumulada;
                    ProbAcumulada = a*exp(-deltaR*nuevoMu*densidadGe);
                    if(ProbAcumulada<Ale2Aux)   // Se detuvo en el cristal.
                    {
                        ii = nMin;
                        if(Ale1Aux>=ProbCompFoto)
                        {
                            Respuesta = 1;    // Compton x2
                            //      cout<<"\t Compton";
                        }
                        if(Ale1Aux < ProbCompFoto)
                        {
                            Respuesta = 0;        //Foto x2
                            //      cout<<"\t Foto";
                        }
                        
                    }
                    
                }
                
                if(CilindroXYZ[nX1][nY1][nZ1][0] !=  densidadGe*mu_rho_Ge )
                {
                    ii = nMin;
                    Respuesta = 2;        // Salio
                    
                }
            }
        }
    }
    
    if(muCompton<0 || muFoto<0)
    {
        Respuesta = 3;    // Descartar por problemas de extrapolacion
  //      cout<<"\n mu negativo. ";
    }
 
 ////   cout<<"\n  Respuesta C2: "<<Respuesta;
    return Respuesta;
}

//***************************************************************************************
//***************************************************************************************
void GeometriaZonas()
{
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
                if ((Rho <= 33.35) && (Z >= 0) && (Z<13))
                {
                    CilindroXYZ[i][j][k][0] = densidadGe*mu_rho_Ge;
                }
                if ((Rho >= 10.5) && (Rho <= 33.35) && (Z >= 13) && (Z<69.7))
                {
                    CilindroXYZ[i][j][k][0] = densidadGe*mu_rho_Ge;
                }
                //--------------------------------------------------------------------
                //	Definiendo lugar Aluminio. Aluminio = 2
                if ((Rho<=7.9) && (Z >= 28.84) && (Z<29.84))
                {
                    CilindroXYZ[i][j][k][0] = densidadAl*mu_rho_Al;
                }
                if ((Rho >= 7.4) && (Rho<=7.9) && (Z >= 29.84) && (Z<69.7))
                {
                    CilindroXYZ[i][j][k][0] = densidadAl*mu_rho_Al;
                }
                //--------------------------------------------------------------------
                //	Definiendo contenedor de plastico. Plastico = - 2
                if ((Rho <= 7.4) && (Z >= 29.84) && (Z<30.84))
                {
                    CilindroXYZ[i][j][k][0] = densidadPoly*mu_rho_Poly;
                }
                
                if ((Rho >= 5.826) && (Rho <= 7.4) && (Z >= 29.84) && (Z<69.7))
                {
                    CilindroXYZ[i][j][k][0] = densidadPoly*mu_rho_Poly;
                }
                //--------------------------------------------------------------------
                //	Definiendo muestra. Muestra = -1.5
                //  Definiendo los límites en donde se encuentra la muestra
                if ((Rho <= 5.826) && (Z >= 30.84) && (Z<69.02))
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
                //--------------------------------------------------------------------

            }
        }
    }
}
//***************************************************************************************
//***************************************************************************************

//**********
// Extrapolacion lineal. A la chingada.
double InterGe2(int CompFoto, double Energia)
{
    // Lo conocido
    int N = 33;             //  Numeros de No. en el intervalo
    double VecX[N], VecY[N];    // Valores en X donde se conoce la funcion
    
    if(CompFoto==0)  // Fotoelec
    {
        VecX[0]=	0.001000;   VecY[0]=    1887;
        VecX[1]=	0.001103;   VecY[1]=    1495;
        VecX[2]=	0.001217;   VecY[2]=	185	;
        VecX[3]=	0.001232;   VecY[3]=	4651;
        VecX[4]=	0.001248;   VecY[4]=	6650;
        VecX[5]=	0.001328;   VecY[5]=	6074;
        VecX[6]=	0.001414;   VecY[6]=	5549;
        VecX[7]=	0.001500;   VecY[7]=	5470;
        VecX[8]=	0.002000;   VecY[8]=	2706.000;
        VecX[9]=	0.003000;   VecY[9]=	957.100;
        VecX[10]=	0.004000;   VecY[10]=	446.100;
        VecX[11]=	0.005000;   VecY[11]=	244.100;
        VecX[12]=	0.006000;   VecY[12]=	148.100;
        VecX[13]=	0.008000;   VecY[13]=	66.750;
        VecX[14]=	0.010000;   VecY[14]=	35.710;
        VecX[15]=	0.011100;   VecY[15]=	196.600;
        VecX[16]=	0.015000;   VecY[16]=	90.400;
        VecX[17]=	0.020000;   VecY[17]=	41.440;
        VecX[18]=	0.030000;   VecY[18]=	13.350;
        VecX[19]=	0.040000;   VecY[19]=	5.843;
        VecX[20]=	0.050000;   VecY[20]=	3.044;
        VecX[21]=	0.060000;   VecY[21]=	1.775000;
        VecX[22]=	0.080000;   VecY[22]=	0.751500;
        VecX[23]=	0.100000;   VecY[23]=	0.383500;
        VecX[24]=	0.150000;   VecY[24]=	0.112500;
        VecX[25]=	0.200000;   VecY[25]=	0.047320;
        VecX[26]=	0.300000;   VecY[26]=	0.014370;
        VecX[27]=	0.400000;   VecY[27]=	0.006412;
        VecX[28]=	0.500000;   VecY[28]=	0.003541;
        VecX[29]=	0.600000;   VecY[29]=	0.002238;
        VecX[30]=	0.800000;   VecY[30]=	0.001144;
        VecX[31]=	1.000000;   VecY[31]=	0.000712;
        VecX[32]=	1.022000;   VecY[32]=	0.00067660;
    }
    
    if(CompFoto==1)  // Compton
    {
        VecX[0]=	0.001000	;VecY[0]=	0.0061860	;
        VecX[1]=	0.001103	;VecY[1]=	0.0072300	;
        VecX[2]=	0.001217	;VecY[2]=	0.0084270	;
        VecX[3]=	0.001232	;VecY[3]=	0.0085910	;
        VecX[4]=	0.001248	;VecY[4]=	0.0087580	;
        VecX[5]=	0.001328	;VecY[5]=	0.0096220	;
        VecX[6]=	0.001414	;VecY[6]=	0.0105400	;
        VecX[7]=	0.001500	;VecY[7]=	0.0114500	;
        VecX[8]=	0.002000	;VecY[8]=	0.0166500	;
        VecX[9]=	0.003000	;VecY[9]=	0.02603000	;
        VecX[10]=	0.004000	;VecY[10]=	0.0342200	;
        VecX[11]=	0.005000	;VecY[11]=	0.0416800	;
        VecX[12]=	0.006000	;VecY[12]=	0.0485700	;
        VecX[13]=	0.008000	;VecY[13]=	0.0607800	;
        VecX[14]=	0.010000	;VecY[14]=	0.0711500	;
        VecX[15]=	0.011100	;VecY[15]=	0.0761400	;
        VecX[16]=	0.015000	;VecY[16]=	0.0904900	;
        VecX[17]=	0.020000	;VecY[17]=	0.1027000	;
        VecX[18]=	0.030000	;VecY[18]=	0.1161000	;
        VecX[19]=	0.040000	;VecY[19]=	0.1223000	;
        VecX[20]=	0.050000	;VecY[20]=	0.124800	;
        VecX[21]=	0.060000	;VecY[21]=	0.125400	;
        VecX[22]=	0.080000	;VecY[22]=	0.123900	;
        VecX[23]=	0.100000	;VecY[23]=	0.121000	;
        VecX[24]=	0.150000	;VecY[24]=	0.112500	;
        VecX[25]=	0.200000	;VecY[25]=	0.104800	;
        VecX[26]=	0.300000	;VecY[26]=	0.092310	;
        VecX[27]=	0.400000	;VecY[27]=	0.083190	;
        VecX[28]=	0.500000	;VecY[28]=	0.076200	;
        VecX[29]=	0.600000	;VecY[29]=	0.070630	;
        VecX[30]=	0.800000	;VecY[30]=	0.062180	;
        VecX[31]=	1.000000	;VecY[31]=	0.055960	;
        VecX[32]=	1.022000	;VecY[32]=	0.055370	;
    }
    
    //--------------------------------------------------------------------------
    //*****************************************************************************
    // Es necesario buscar en donde cae los nuevos numeros aleatorios
    int i2;
    double Aprox;
    
    double x1, x2, y1, y2; ;
    int a, b, c;
    float NumDeseado = Energia;
    
    for(i2=0; i2<32; i2++)
    {
        if(NumDeseado>=VecX[i2] && NumDeseado<VecX[i2+1])
        {
            x1 = VecX[i2];
            x2 = VecX[i2+1];
            y1 = VecY[i2];
            y2 = VecY[i2+1];
            
            Aprox = (y2-y1)*(NumDeseado-x1)/(x2-x1) + y1;
        }
            
    }

    return Aprox;
}
