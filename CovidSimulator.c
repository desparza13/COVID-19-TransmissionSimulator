//Omar Chavira Román
//Daniela Esparza Espinosa

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define FILAS 25
#define COLUMNAS 25
#define OCUPACION 0.8 //Porcentaje de ocupación de celdas en decimal
#define SANO 0
#define ENFERMO 1
#define RECUPERADO 2
#define FALSO 0
#define VERDADERO 1
//------------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------------
//VARIABLES GLOBALES Y ESTRUCTURAS
enum proteccion {NoCubreNoVacuna, CubreNoVacuna, NoCubreVacuna, CubreVacuna}; //Menciona los 4 niveles de protección contra COVID posibles. Nota: Cubre simboliza cubrebocas
short numEnfermos = 0; //Cantidad de enfermos en el grupo
int diasTranscurridos = 0; //Días transcurridos desde el inicio de la simulación
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
//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------
//FUNCIONES
void mostrarMatriz (celda *pCeldas); //Imprime la matriz con los formatos s, S, v, V, r
void mezclarPersonas (celda *pCeldas); //Da formato aleatorio a la matriz mezclando los tipos de personas y las celdas vacias
void inicializarMatriz( celda *pCeldas); //Ingresa 25% de la ocupación con cada tipo de persona 
void contagiar(celda *pEnfermo, celda *pLado); //Revisa probabilidades según el nivel de protección de cada persona y decide si se transmite o no
void contagiarAlrededor( celda *pCeldas, celda *pEnfermo, int i); //Revisa a las personas alrededor de alguien en específico y transmite la enfermedad si el azar lo indica
void transmitirEnfermedad(celda *pCeldas); //Recorre toda la matriz, cada que encuentra a alguien enfermo revisa quienes se contagian a su alrededor

//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------
//MAIN
int main(void)
{
    srand(time(0)); //Garantiza el funcionamiento correcto de random cambiando la semilla
    celda celdas[FILAS][COLUMNAS];
    celda *pCeldas = &celdas;
    inicializarMatriz(pCeldas);
    mostrarMatriz(pCeldas);
    while (numEnfermos>0)
    {
        transmitirEnfermedad(pCeldas);
        diasTranscurridos++;
        mostrarMatriz(pCeldas);
    }
    return 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------------
//DEFINICION DE FUNCIONES
/*
La función mostrarMatriz imprime toda la matriz con la siguiente nomenclatura:
    ->S: persona no vacunada con el status de sano y sin cubrebocas
    ->s: persona no vacunada con el status de sano y con cubrebocas
    ->V: persona vacunada con el status de sano y sin cubrebocas
    ->v: persona vacunada con el status de sano y con cubrebocas
    ->E: persona con el status de enfermo y sin cubrebocas
    ->e: persona con el status de enfermo y con cubrebocas
    ->r: persona con el status de recuperado (no se puede volver a enfermar)
*/
void mostrarMatriz (celda *pCeldas)
{
    int cant=0;
    for (int i=0; i<FILAS; i++)
    {
        printf("[");
        for (int j=0; j<COLUMNAS; j++)
        {
            if (pCeldas->cellStat==1)
            {
                if (pCeldas->status==SANO && pCeldas->vacuna==FALSO && pCeldas->cubrebocas==FALSO)
                    printf("S ");
                else if (pCeldas->status==SANO && pCeldas->vacuna==FALSO && pCeldas->cubrebocas==VERDADERO)
                    printf("s ");
                else if (pCeldas->status==SANO && pCeldas->vacuna==VERDADERO && pCeldas->cubrebocas==FALSO)
                    printf("V ");
                else if (pCeldas->status==SANO && pCeldas->vacuna==VERDADERO && pCeldas->cubrebocas==VERDADERO)
                    printf("v ");
                else if (pCeldas->status==ENFERMO  && pCeldas->cubrebocas==FALSO)
                    printf("E ");
                else if (pCeldas->status==ENFERMO && pCeldas->cubrebocas==VERDADERO)
                    printf("e ");
                else if (pCeldas->status==RECUPERADO)
                    printf("r ");
            }
            else
                printf("  "); //Celdas desocupadas
            pCeldas++;
            cant++;
        }
        printf("]\n");
    }
    printf("CANTIDAD DE ENFERMOS: %d\n",numEnfermos);
    printf("DÍAS TRANSCURRIDOS: %d\n",diasTranscurridos);
    printf("-----------------------------------------------------------------------------------------\n");
}
/*
La función mezclarPersonas toma una matriz con las personas segregadas de acuerdo a su nivel de protección
e intercambia a las personas de manera aleatoria, garantizando una matriz con personas y espacios aleatorios
*/
void mezclarPersonas (celda *pCeldas)
{
    for (int i=0; i<(FILAS*COLUMNAS*OCUPACION); i++)
    {
        int indAleatorio = (rand() % (FILAS*COLUMNAS));
        celda temporal = *(pCeldas+i);
        *(pCeldas+i) = *(pCeldas+indAleatorio);
        *(pCeldas+indAleatorio) = temporal;
    }
}
/*
La función inicializarMatriz asigna celdas a la matriz siguiendo las siguientes reglas:
    El Porcentaje indicado por la ocupación se marca como celdas ocupadas por alguien sano de las cuales:
        ->25% no tienen vacuna ni cubrebocas
        ->25% no tienen vacuna pero tienen cubrebocas
        ->25% tiene vacuna pero no cubrebocas
        ->25% tiene vacuna y usa cubrebocas
    El resto de celdas serán marcadas como no ocupadas y ayudan como sana distancia
    Después una de las personas se asigna aleatoriamente como enferma y se mezclan las posiciones
*/
void inicializarMatriz( celda *pCeldas)
{
    int id=1; //Se usará como número único de cada persona
    for (int nivelProteccion=0; nivelProteccion<4; nivelProteccion++) //Por cada nivel de protección
    {
        for (int j=0; j<(FILAS*COLUMNAS*OCUPACION*0.25); j++) //Hay un 25% de cada nivel donde:
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
    pCeldas-=625;
    short personasOcupadas = (FILAS*COLUMNAS*OCUPACION);
    (pCeldas+(rand() % personasOcupadas))->status = ENFERMO; //Enfermar a alguien al azar
    numEnfermos++;
    mezclarPersonas(pCeldas);
}
/*
La función contagiar decide si una persona en contacto con alguien enfermo adquirirá COVID-19 o no
de acuerdo con las siguientes probabilidades de contagio seleccionando un num. al azar de 1 al 100:
    ->60%: Ninguno de los dos tiene cubreboca, ni tiene vacuna
    ->20%: Uno de los dos tiene cubreboca, el sano no tiene vacuna
    ->6%: Ambos tienen cubrebocas, pero no hay vacuna
    ->36%: Ninguno tiene cubrebocas, pero hay vacuna
    ->12%: Alguno tiene cubrebocas, pero hay vacuna
    ->3%: Ambos tiene cubrebocas, pero hay vacuna
Si el número obtenido está en el rango de 1 al porcentaje, la persona sana cambia de estado a enfermo
*/
void contagiar(celda *pEnfermo, celda *pLado) 
{
    if(pLado->cellStat==1 && pLado->status==SANO)
    {
        int azar=(rand()%101)+1; //Selecciono un número de 1 al 100
        int enfermar=FALSO; //Marcador bandera que indica si por la probabilidad se enfermerá o no, trigger de la asignacion de enfermo
        if(pEnfermo->cubrebocas==FALSO && pLado->cubrebocas==FALSO && pLado->vacuna==FALSO) //Ninguno trae cubrebocas y el sano no está vacunado
        {
            if (azar<=60)
                enfermar=VERDADERO;
        }
        else if (pEnfermo->cubrebocas==VERDADERO && pLado==VERDADERO && pLado->vacuna==FALSO) //Ambos con cubrebocas, la persona sana sin vacuna
        {
            if (azar<=6)
                enfermar=VERDADERO;
        }
        else if ((pEnfermo->cubrebocas==VERDADERO || pLado->cubrebocas==VERDADERO) && pLado->vacuna==FALSO) //Alguno de los dos trae cubrebocas y el sano no está vacunado
        {
            if (azar<=20)
                enfermar=VERDADERO;
        }
        else if (pEnfermo->cubrebocas==FALSO && pLado->cubrebocas==FALSO && pLado->vacuna==VERDADERO) //Ninguno trae cubrebocas pero el sano si está vacunado
        {
            if (azar<=36)
                enfermar=VERDADERO;
        }
        else if (pEnfermo->cubrebocas==VERDADERO && pLado==VERDADERO && pLado->vacuna==VERDADERO) //Ambos con cubrebocas, la persona sana con vacuna
        {
            if (azar<=3)
                enfermar=VERDADERO;
        }
        else if ((pEnfermo->cubrebocas==VERDADERO || pLado->cubrebocas==VERDADERO) && pLado->vacuna==VERDADERO) //Alguno de los dos trae cubrebocas y el sano está vacunado
        {
            if (azar<=20)
                enfermar=VERDADERO;
        }

        if(enfermar==VERDADERO) //Si la probabilidad lo indica, se contagia el de al lado
        {
            pLado->status=ENFERMO;
            pLado->dias=0;
            numEnfermos++;
        }
    }
}
/*
La función contagiarAlrededor toma a un enfermo y compara con todas las personas a su alrededor si 
existe un contagio/transmisión de la enfermedad o no
*/
void contagiarAlrededor( celda *pCeldas, celda *pEnfermo, int i) //Revisa a las personas alrededor y transmite la enfermedad si el azar lo indica
{
    //Fila superior
    if (i==0) //Esquina superior izquierda
    {
        contagiar(pEnfermo,pCeldas+i+1); //Ver si se contagia el de la derecha
        contagiar(pEnfermo, pCeldas+i+FILAS); //Ver si se contagia el de abajo
        contagiar(pEnfermo, pCeldas+i+FILAS+1); //Ver si se contagia el de diagonal derecha inferior
    }
    else if (i==FILAS-1) //Esquina superior derecha
    {
        contagiar(pEnfermo, pCeldas+i-1); //Ver si se contagia el de la izquierda
        contagiar(pEnfermo, pCeldas+i+FILAS); //Ver si se contagia el de abajo
        contagiar(pEnfermo, pCeldas+i+FILAS-1); //Ver si se contagia el de diagonal izquierda inferior
    }
    else if (i<FILAS) //Fila superior
    {
        contagiar(pEnfermo, pCeldas+i-1); //Ver si se contagia el de la izquierda
        contagiar(pEnfermo,pCeldas+i+1); //Ver si se contagia el de la derecha
        contagiar(pEnfermo, pCeldas+i+FILAS-1); //Ver si se contagia el de diagonal izquierda inferior
        contagiar(pEnfermo, pCeldas+i+FILAS); //Ver si se contagia el de abajo
        contagiar(pEnfermo, pCeldas+i+FILAS+1); //Ver si se contagia el de diagonal derecha inferior
    }
    else if (i==(FILAS*COLUMNAS)-1) //Esquina inferior derecha
    {
        contagiar(pEnfermo, pCeldas+i-FILAS-1); //Ver si se contagia la diagonal izquierda superior
        contagiar(pEnfermo, pCeldas+i-FILAS); //Ver si se contagia el de arriba
        contagiar(pEnfermo, pCeldas+i-1); //Ver si se contagia el de la izquierda
    }
    else if (i==(FILAS*COLUMNAS)-FILAS) //Esquina inferior izquierda
    {
        contagiar(pEnfermo, pCeldas+i-FILAS); //Ver si se contagia el de arriba
        contagiar(pEnfermo, pCeldas+i-FILAS+1); //Ver si se contagia la diagonal derecha superior
        contagiar(pEnfermo,pCeldas+i+1); //Ver si se contagia el de la derecha
    }
    else if (i>(FILAS*COLUMNAS)-FILAS) //Fila inferior
    {
        contagiar(pEnfermo, pCeldas+i-FILAS-1); //Ver si se contagia la diagonal izquierda superior
        contagiar(pEnfermo, pCeldas+i-FILAS); //Ver si se contagia el de arriba
        contagiar(pEnfermo, pCeldas+i-FILAS+1); //Ver si se contagia la diagonal derecha superior
        contagiar(pEnfermo, pCeldas+i-1); //Ver si se contagia el de la izquierda
        contagiar(pEnfermo,pCeldas+i+1); //Ver si se contagia el de la derecha
    }
    else if (i%25 == 0) //Columna izquierda
    {
        contagiar(pEnfermo, pCeldas+i-FILAS); //Ver si se contagia el de arriba
        contagiar(pEnfermo, pCeldas+i-FILAS+1); //Ver si se contagia la diagonal derecha superior
        contagiar(pEnfermo,pCeldas+i+1); //Ver si se contagia el de la derecha
        contagiar(pEnfermo, pCeldas+i+FILAS); //Ver si se contagia el de abajo
        contagiar(pEnfermo, pCeldas+i+FILAS+1); //Ver si se contagia el de diagonal derecha inferior
    }
    else if ((i+1)%25 == 0 ) //Columna derecha
    {
        contagiar(pEnfermo, pCeldas+i-FILAS-1); //Ver si se contagia la diagonal izquierda superior
        contagiar(pEnfermo, pCeldas+i-FILAS); //Ver si se contagia el de arriba
        contagiar(pEnfermo, pCeldas+i-1); //Ver si se contagia el de la izquierda
        contagiar(pEnfermo, pCeldas+i+FILAS-1); //Ver si se contagia el de diagonal izquierda inferior
        contagiar(pEnfermo, pCeldas+i+FILAS); //Ver si se contagia el de abajo
    }
    else //El resto de personas en el centro
    {
        contagiar(pEnfermo, pCeldas+i-FILAS-1); //Ver si se contagia la diagonal izquierda superior
        contagiar(pEnfermo, pCeldas+i-FILAS); //Ver si se contagia el de arriba
        contagiar(pEnfermo, pCeldas+i-FILAS+1); //Ver si se contagia la diagonal derecha superior
        contagiar(pEnfermo, pCeldas+i-1); //Ver si se contagia el de la izquierda
        contagiar(pEnfermo,pCeldas+i+1); //Ver si se contagia el de la derecha
        contagiar(pEnfermo, pCeldas+i+FILAS-1); //Ver si se contagia el de diagonal izquierda inferior
        contagiar(pEnfermo, pCeldas+i+FILAS); //Ver si se contagia el de abajo
        contagiar(pEnfermo, pCeldas+i+FILAS+1); //Ver si se contagia el de diagonal derecha inferior
    }

}
/*
La función transmitirEnfermedad recorre toda la matriz buscando personas enfermas, cada que encuentra a un enfermo
revisa sus alrededores y transmite la enfermedad.
Igualmente si una persona enferma ya ha cumplido los 10 días de enfermedad cambia su estado a recuperado
*/
void transmitirEnfermedad(celda *pCeldas)
{
    celda *persona = pCeldas;
    for (int i=0; i<(FILAS*COLUMNAS); i++)
    {
        if (persona->cellStat==1 && persona->status==ENFERMO)
        {
            if (persona->dias<10)
            {
                contagiarAlrededor(pCeldas,persona,i);
                persona->dias++;
            }
            else
            {
                persona->status=RECUPERADO;
                numEnfermos--;
            }
        }
        persona++;
    }
}
