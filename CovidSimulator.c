//Omar Chavira Román
//Daniela Esparza Espinosa

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FILAS 25
#define COLUMNAS 25
#define OCUPACION 0.2
#define SANO 0
#define ENFERMO 1
#define RECUPERADO 2
#define FALSO 0
#define VERDADERO 1
//------------------------------------------------------------------------------------------------------------------------------------
//VARIABLES GLOBALES Y ESTRUCTURAS
enum proteccion {NoCubreNoVacuna, CubreNoVacuna, NoCubreVacuna, CubreVacuna}; //Menciona los 4 niveles de protección contra COVID posibles. Nota: Cubre simboliza cubrebocas
short numEnfermos = 0;
typedef struct
{
    char cellStat;
    short id;
    char edad;
    char status;
    char vacuna;
    char dias;
    char cubrebocas;
} celda;
//---------------------------------------------------------------------------------------------------------------------------------------------
//FUNCIONES
void mostrarMatriz (celda *pCeldas);
void mezclarPersonas (celda *pCeldas);
void inicializarMatriz( celda *pCeldas);

//---------------------------------------------------------------------------------------------------------------------------------------------
int main(void)
{
    srand(time(0));
    celda celdas[FILAS][COLUMNAS];
    celda *pCeldas = &celdas;
    inicializarMatriz(pCeldas);
    return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------
//DEFINICION DE FUNCIONES
void mostrarMatriz (celda *pCeldas)
{
    int cant=0;
    for (int i=0; i<FILAS; i++)
    {
        printf("[");
        for (int j=0; j<COLUMNAS; j++)
        {
            if (pCeldas->status==SANO && pCeldas->vacuna==FALSO && pCeldas->cubrebocas==FALSO)
                printf("S ");
            if (pCeldas->status==SANO && pCeldas->vacuna==FALSO && pCeldas->cubrebocas==VERDADERO)
                printf("s ");
            if (pCeldas->status==SANO && pCeldas->vacuna==VERDADERO && pCeldas->cubrebocas==FALSO)
                printf("V ");
            if (pCeldas->status==SANO && pCeldas->vacuna==VERDADERO && pCeldas->cubrebocas==VERDADERO)
                printf("v ");
            if (pCeldas->status==ENFERMO  && pCeldas->cubrebocas==FALSO)
                printf("E ");
            if (pCeldas->status==ENFERMO && pCeldas->cubrebocas==VERDADERO)
                printf("e ");
            if (pCeldas->status==RECUPERADO)
                printf("r ");
            pCeldas++;
            cant++;
        }
        printf("]\n");
    }
    printf("Total: %d\n",cant);
}
void mezclarPersonas (celda *pCeldas)
{
    for (int i=0; i<(FILAS*COLUMNAS*0.8); i++)
    {
        int indAleatorio = (rand() % (FILAS*COLUMNAS));
        celda temporal = *(pCeldas+i);
        *(pCeldas+i) = *(pCeldas+indAleatorio);
        *(pCeldas+indAleatorio) = temporal;
    }
}
void inicializarMatriz( celda *pCeldas)
{
    int id=1; //Se usará como número único de cada persona
    for (int nivelProteccion=0; nivelProteccion<4; nivelProteccion++) //Por cada nivel de protección
    {
        for (int j=0; j<(FILAS*COLUMNAS*0.8*0.25); j++) //Hay un 25% de cada nivel donde:
        {
            pCeldas->cellStat = 1; //La celda está ocupada por una persona
            pCeldas->id = id; //Id de la persona
            id++;
            pCeldas->edad = (rand() % 91); //Edad aleatoria entre 0 y 90 años
            pCeldas->status = SANO;
            pCeldas->dias = 0; //Sin días enfermos

            switch(nivelProteccion) //Se asigna su uso/falta de cubrebocas y/o vacuna
            {
                case NoCubreNoVacuna:
                    pCeldas->cubrebocas = FALSO;
                    pCeldas->vacuna = FALSO;
                    break;
                case CubreNoVacuna:
                    pCeldas->cubrebocas = VERDADERO;
                    pCeldas->vacuna = FALSO;
                    break;
                case NoCubreVacuna:
                    pCeldas->cubrebocas = FALSO;
                    pCeldas->vacuna = VERDADERO;
                    break;
                case CubreVacuna:
                    pCeldas->cubrebocas = VERDADERO;
                    pCeldas->vacuna = VERDADERO;
            }
            pCeldas++;
        }
    }
    for (id=id-1; id<(FILAS*COLUMNAS); id++) //El 20% restante de celdas están vacias
    {
        pCeldas->cellStat = 0; 
        pCeldas->id = 0; 
        pCeldas->edad = 0; 
        pCeldas->status = 0; 
        pCeldas->dias = 0; 
        pCeldas->cubrebocas = 0;
        pCeldas->vacuna = 0;
        pCeldas++;
    }
    printf("MATRIZ ORDENADA\n");
    pCeldas-=625;
    mostrarMatriz(pCeldas);
    mezclarPersonas(pCeldas);
    (pCeldas+(rand() % (FILAS*COLUMNAS)))->status = ENFERMO; //Enfermar a alguien al azar
    numEnfermos++;
    printf("MATRIZ ALEATORIA\n");
    mostrarMatriz(pCeldas);
}

