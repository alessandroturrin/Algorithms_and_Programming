#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define nArray 100 //numero massimo di vettori di di interi
#define max 5 //dimensione massima di ciascun vettore

typedef struct
{
    int vettore[max];
    int dimensione;
    
    int numeroScambi;
    int iterazioniEserno;
    int iterazioniInterno;
    int totaleIterazioni;
}dati;

int inputFile(dati array[], char nomeFile[]);
void printVettori(dati array[], int index);

void save(dati array[], int copia[], int index);
void reset(dati array[], int copia[], int index);

void BubbleSort(dati array[], int copia[], int index);
void InsertionSort(dati array[], int copia[], int index);
void ShellSort(dati array[], int copia[], int index);

void printRisultati(dati array[], int index, char funzione[]);

int main()
{
    char nomeFile[] = "sort.txt";
    dati array[nArray]; //vettore di strutture
    int copia[nArray]; //per ripristinare array
    int nVettori;
    
    nVettori = inputFile(array, nomeFile);
    
    for(int i=0;i<nVettori;i++)
    {
        save(array, copia, i);
        printVettori(array, i);
        BubbleSort(array, copia, i);
        InsertionSort(array, copia, i);
        ShellSort(array, copia, i);
    }
    
}


int inputFile(dati array[], char nomeFile[])
{
    FILE* file;
    int nVettori; //numero di vettori indicato da file
    
    if((file=fopen(nomeFile, "rt"))==NULL)
    {
        printf("Errore nell'apertura di '%s'!\n", nomeFile);
        exit(-1);
    }
    
    fscanf(file, "%d", &nVettori);
    
    for(int i=0;i<nVettori;i++)
    {
        fscanf(file, "%d", &array[i].dimensione);
        for(int j=0;j<array[i].dimensione;j++)
            fscanf(file, "%d", &array[i].vettore[j]);
    }
    
    return nVettori;
}

void printVettori(dati array[], int index)
{
    printf("\n\n\n\n\nVettore: [");
    
    for(int i=0;i<array[index].dimensione;i++)
    {
        if(i==((array[index].dimensione)-1))
            printf("%d]\n", array[index].vettore[i]);
        else
            printf("%d,", array[index].vettore[i]);
    }
    puts("");
}

void save(dati array[], int copia[], int index)
{
    for(int i=0;i<array[index].dimensione;i++)
        copia[i] = array[index].vettore[i];
    
    array[index].numeroScambi = 0;
    array[index].iterazioniInterno = 0;
    array[index].iterazioniEserno = 0;
}

void reset(dati array[], int copia[], int index)
{
    for(int i=0;i<array[index].dimensione;i++)
        array[index].vettore[i] = copia[i];
    
    array[index].numeroScambi = 0;
    array[index].iterazioniInterno = 0;
    array[index].iterazioniEserno = 0;
}

void BubbleSort(dati array[], int copia[], int index) //index rappresenta il vettore corrente
{
    int i, j, l=0, r=array[index].dimensione-1;
    int scambio;

    for(i=l;i<r;i++)
    {
        for(j=l;j<r-i+l;j++)
        {
            if(array[index].vettore[j]>array[index].vettore[j+1])
            {
                scambio = array[index].vettore[j];
                array[index].vettore[j] = array[index].vettore[j+1];
                array[index].vettore[j+1] = scambio;
                array[index].numeroScambi+=3;
            }
            array[index].iterazioniInterno++;
        }
        array[index].iterazioniEserno++;
    }
    
    printRisultati(array, index, "Bubble Sort");
    reset(array, copia, index);
}

void InsertionSort(dati array[], int copia[], int index)
{
    int i, j, l=0, r=array[index].dimensione-1;
    int x;
    
    for(i=l+1;i<=r;i++)
    {
        x = array[index].vettore[i];
        j = i-1;
        while(j>=l && x<array[index].vettore[j])
        {
            array[index].vettore[j+1] = array[index].vettore[j];
            j--;
            array[index].iterazioniInterno++;
            array[index].numeroScambi++;
        }
        array[index].vettore[j+1] = x;
        array[index].iterazioniEserno++;
        array[index].numeroScambi++;
    }
    
    printRisultati(array, index, "Insertion Sort");
    reset(array, copia, index);
    
}

void ShellSort(dati array[], int copia[], int index)
{
    int i, j, l=0, r=array[index].dimensione-1, h=1;
    int temp;
    
    while(h<index/3)
        h = 3*h+1;
    while(h>=1)
    {
        for(i=l+h;i<=r;i++)
        {
            j = i;
            temp = array[index].vettore[i];
            array[index].numeroScambi++;
            
            while(j>=l+h && temp<array[index].vettore[j-h])
            {
                array[index].vettore[j] = array[index].vettore[j-h];
                array[index].numeroScambi++;
                j-=h;
                array[index].iterazioniInterno++;
            }
            array[index].vettore[j] = temp;
        }
        h = h/3;
        array[index].iterazioniEserno++;
    }
    
    printRisultati(array, index, "Shell Sort");
    reset(array, copia, index);
    
}

void printRisultati(dati array[], int index, char funzione[])
{
    printf("------%s------\n", funzione);
    
    array[index].totaleIterazioni = array[index].iterazioniInterno + array[index].iterazioniEserno;
    
    printf("\nNumero di iterazioni del ciclo interno: %d\n", array[index].iterazioniInterno);
    printf("Numero di iterazioni del ciclo esterno: %d\n", array[index].iterazioniEserno);
    printf("Numero di iterazioni totali: %d\n", array[index].totaleIterazioni);
    printf("Numero di scambi totali: %d\n", array[index].numeroScambi);
    
    for(int i=0;i<strlen(funzione)+12;i++)
        printf("-");
    puts("");
}
