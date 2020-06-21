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

int nRhoMuestra = 0;    // Cuadritos en Rho Muestra
int nRhoMuestraMin = nRho;
int nRhoMuestraMax = 0;
int nPhiMuestra = 0;    // Cuadritos en Phi muestra
int nPhiMuestraMin = nPhi;
int nPhiMuestraMax = 0;

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

double Cilindro[nRho][nPhi][nZ][2];
double CilindroXYZ[nX][nY][nZ][2];

int i, j, k;				// Contadoeres
double Phi, Rho, Z, X, Y;	// Contadores de valores reales
double Rho0, Rho1, Phi0, Phi1;	// Limites Reales, en cm y radianes
double X0, X1, Y0, Y1, Z0, Z1;

double RadioMaterial, RadioRecipiente, RadioVacio, RadioParedes, RadioCristal;
double deltaRho, deltaPhi, deltaZ;		// Diferenciales
double deltaX, deltaY;


//	Functions 
void GeometriaZonas();
void ValoresIniciales();
void LugaresEmisionAleatorios2ParaEscrituraArchivo();

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
    LugaresEmisionAleatorios2ParaEscrituraArchivo();
	cout << "\n 3) LugaresEmisionAleatorios: Fin y O.K.";
    
    cout<<"\n\n";
	return 0;
}
//***************************************************************************************
//***************************************************************************************
void ValoresIniciales()
{
	//--- Valores Limites Realmes ----------------------
	Rho0 = 0;			// mm
	Rho1 = 33.35;		// mm
	Phi0 = 0;			// radianes
	Phi1 = 2 * M_PI;	// radianes
	Z0 = 0;				// mm
	Z1 = 69.7;// mm

    X0 = -40;
    X1 = +40;
    Y0 = -40;
    Y1 = +40;
    Z0 = 0;
    Z1 = 70;
    
	RadioMaterial = 5;			//cm
	RadioRecipiente = 5.2;		//cm
	RadioVacio = 5.5;			//cm
	RadioParedes = 6;			//cm
	RadioCristal = 6.5;			//cm

	// Diferenciales
	deltaRho = (Rho1 - Rho0) / nRho;
	deltaPhi = (Phi1 - Phi0) / nPhi;
    
    deltaX = (X1 - X0) / nX;
    deltaY = (Y1 - Y0) / nY;
    deltaZ = (Z1 - Z0) / nZ;
}
//***************************************************************************************
//***************************************************************************************
void LugaresEmisionAleatorios2ParaEscrituraArchivo()
{
    /*
     Se copia la función anterior con la finalidad de estudiar la dependencia del 
     No. de núcleos que se escapan respecto al número de eventos generados 
     aleatoriamente. 
     
     Se copia para NO modificar el archivo original.
     
     */
    //----------
    ofstream myfileEscape;
    myfileEscape.open("NoEventGenerados_NoEventsEscape_Probabilidad.txt");
    
     myfileEscape<<"#No. de Eventos\t PromedioEscapados \t SigmaEscapados";
    
    //  Contador de No. de Eventos (decaimientos) vs. Eventos que se escapan
    double NumeroEventos = 10;
    double NumeroEventosEscape = 0;
    int NumeroEventosInicialPotenciaDiez = 0;
    int NumeroEventosFinalPotencialDiez = 5;
    int DeltaNumeroEventos;
    

    
    int r;                      // Variables auxiliares
    double ThetaAle, PhiAle;    // Angulos aleatorios
    
    //  Variables para dibujar las trayectorias
    double RadioAux;
    double RadioAuxMax = 45;    //  En mm
    double nRadioAux = 500;
    double color;
    
    double ThetaMin;    // para auxiliar para angulo descartar rayo emisor
    double RhoMax;              //  Variable Rho Ma
    double RadioPozo = 10.5;    //  Radio del pozo del detector
    
    //  Variable auxiliar
    double diffAng;
    int kk;
    int l;
    
    double a, b;
    
    double PromedioPorcentaje;
    double SigmaPorcentaje;
    int LmaxPromedio = 50;
    double Porcentaje[LmaxPromedio-1];

    i = 0;
    srand((int)time(0));
    for(k= NumeroEventosInicialPotenciaDiez; k<=NumeroEventosFinalPotencialDiez; k++)
    {
        for(kk=1; kk<= 9; kk = kk+1)
        {
            cout<<"\n"<<kk*pow(10,k);
            
            NumeroEventos = kk*pow(10,k);
            
            PromedioPorcentaje = 0;
            SigmaPorcentaje = 0;
            a=0;
            
            
            srand((int)time(0));
            for(int l= 0; l<LmaxPromedio; l++)
            {
                i = 0;
                NumeroEventosEscape = 0;


//                cout<<"\nValor L: "<<l;
                    
                while (i++ < NumeroEventos)
                {
                //  Numeros aleatorios
                //  Se debe agregar +1 para que la funcion pueda incluir el extermo superior
                        srandom(time(NULL));
                    r = (rand() % (nXMuestraMax-nXMuestraMin+1)) + nXMuestraMin;
                    nXAux = r;
                    r = (rand() % (nYMuestraMax-nYMuestraMin+1)) + nYMuestraMin;
                    nYAux = r;
                    r = (rand() % (nZMuestraMax-nZMuestraMin+1)) + nZMuestraMin;
                    nZAux = r;
                
                //  Direccion aleatoria
                    ThetaAle = M_PI*(double)rand()/(RAND_MAX);
                    PhiAle = 2*M_PI*(double)rand()/(RAND_MAX) ;
                
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
            
                    if(ThetaAle<=ThetaMin)  NumeroEventosEscape ++;
                }
                
                Porcentaje[l] =NumeroEventosEscape*100/NumeroEventos;
     //           cout<<"\nPorcentaje: "<<Porcentaje[l];
                a = a + Porcentaje[l];
            }
            
            PromedioPorcentaje = a / LmaxPromedio;
            b=0;
            // Calculando sigma
            for(int l= 0; l<LmaxPromedio; l++)
            {
                b = b + pow(PromedioPorcentaje - Porcentaje[l],2);
            }
            
            SigmaPorcentaje = sqrt(b/LmaxPromedio);
            
            cout<<"\n Promedio Porcentaje: "<<PromedioPorcentaje<<" +- "<<SigmaPorcentaje;

            
            myfileEscape<<"\n"<<NumeroEventos;
            myfileEscape<<"\t"<<fixed<<setprecision(4)<<PromedioPorcentaje;
            myfileEscape<<"\t"<<SigmaPorcentaje;
        }
    }
    

    
    myfileEscape.close();
    
}
//***************************************************************************************
//***************************************************************************************
void GeometriaZonas()
{
	ofstream FileCartesianas1, FileCartesianas2;

    FileCartesianas1.open("X_Z_Yfijo.txt");
    FileCartesianas2.open("Y_Z_Xfijo.txt");
    
    FileCartesianas1<<"#Valores de X y Z para Y=0 fijo\n";
    FileCartesianas2<<"#Valores de Y y Z para X=0 fijo\n";
    
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
                CilindroXYZ[i][j][k][0] = 0;
                //--------------------------------------------------------------------
                //	Definiendo lugar del cristal. Cristal Ge = 1
                if ((Rho <= 33.35) && (Z >= 0) && (Z<12.8))
                {
                    CilindroXYZ[i][j][k][0] = 1;
                }
                if ((Rho >= 10.5) && (Rho <= 33.35) && (Z >= 12.8) && (Z<69.7))
                {
                    CilindroXYZ[i][j][k][0] = 1;
                }
                //--------------------------------------------------------------------
                //	Definiendo lugar Copper. Copper = 2
                if ((Rho<=8.25) && (Z >= 28.7) && (Z<29.7))
                {
                    CilindroXYZ[i][j][k][0] = 2;
                }
                if ((Rho >= 7.75) && (Rho<=8.25) && (Z >= 29.7) && (Z<69.7))
                {
                    CilindroXYZ[i][j][k][0] = 2;
                }
                //--------------------------------------------------------------------
                //	Definiendo contenedor de plastico. Plastico = - 2
                if ((Rho <= 5.5) && (Z >= 29.7) && (Z<30.5))
                {
                    CilindroXYZ[i][j][k][0] = -2;
                }
                
                if ((Rho <= 5.5) && (Z >= 59.2) && (Z<59.7))
                {
                    CilindroXYZ[i][j][k][0] = -2;
                }
                
                if ((Rho >= 5) && (Rho <= 5.5) && (Z >= 30.2) && (Z<59.2))
                {
                    CilindroXYZ[i][j][k][0] = -2;
                }
                //--------------------------------------------------------------------
                //	Definiendo muestra. Muestra = -1.5
                //  Definiendo los límites en donde se encuentra el cristal
                if ((Rho <= 5.0) && (Z >= 30.2) && (Z<55.2))
                {
                    CilindroXYZ[i][j][k][0] = -1.5;
        
                    nXAux = i;
                    nYAux = j;
                    nZAux = k;

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
