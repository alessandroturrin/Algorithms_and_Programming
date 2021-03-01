#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define numeroPietre 4
typedef enum {zaffiro, rubino, topazio, smeraldo} pietra;
typedef enum {false, true} bool;

int inputFile(FILE* file, int nPietre[]);
void disposizioni(int val[], int sol[], int bestSol[], int nPietre[], int pos, int n, int k, int *maxLength);
bool check(int sol[], int nPietre[], int pos, int val);
void printStart(int nPietre[], int nPietreTot, int nTest);
int main()
{
    FILE* file;
    char nomeFile[] = "/Users/alexturrin/Desktop/easy_test_set.txt";
    int nInput, nPietre[numeroPietre], nPietreTot; //numero di input totali, numero di pietre per input e nTot pietre per input
    int val[numeroPietre], *sol, *bestSol;
    int maxLength;
    bool found;
    clock_t begin, end;
    
    if((file=fopen(nomeFile, "rt"))==NULL)
    {
        printf("Errore nell'apertura del file '%s'\nEsecuzione del programma interrotta!\n", nomeFile);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", &nInput);
   
    for(int i=0;i<numeroPietre;i++) val[i] = i;
    for(int i=0;i<nInput;i++)
    {
        nPietreTot = inputFile(file, nPietre);
        maxLength = 0;
        found = false;
        sol = (int*)malloc(nPietreTot*sizeof(int));
        bestSol = (int*)malloc((nPietreTot)*sizeof(int));
        printStart(nPietre, nPietreTot, i);
        begin = clock();
        disposizioni(val, sol, bestSol, nPietre, 0, numeroPietre, nPietreTot, &maxLength);
        end = clock();
        printf("Collana di massima lunghezza: %d\n", maxLength);
        printf("Tempo impiegato per il calcolo: %f secondi\n\n", (double)(end - begin) / CLOCKS_PER_SEC);
        free(bestSol);
        free(sol);
    }
}

int inputFile(FILE* file, int nPietre[])
{
    int nPietreTot = 0;
    for(int i=0;i<numeroPietre;i++)
    {
        fscanf(file, "%d", &nPietre[i]);
        nPietreTot += nPietre[i];
    }
    return nPietreTot;
}

void disposizioni(int val[], int sol[], int bestSol[], int nPietre[], int pos, int n, int k, int *maxLength)
{
    int i;
    if(pos>=k)
    {
        if(pos>*maxLength)
        {
            *maxLength = pos;
            for(i=0;i<*maxLength;i++) bestSol[i] = sol[i];
        }
        return;
    }
    
    for(i=0;i<n;i++)
    {
        if(nPietre[i]>0 && check(sol,nPietre,pos,val[i]))
        {
            sol[pos] = val[i];
            nPietre[i] -= 1;
            disposizioni(val, sol, bestSol, nPietre, pos+1, n, k, maxLength);
            nPietre[i] += 1;
        }
        else
        {
            if(pos>*maxLength)
            {
                *maxLength = pos;
                for(i=0;i<*maxLength;i++) bestSol[i] = sol[i];
            }
        }
    }
    return;
}

bool check(int sol[], int nPietre[], int pos, int val)
{
    bool fine = false;
    
    if (pos==0) return true; //caso iniziale
    
    switch (val)
    {
        case zaffiro:
            if(sol[pos-1]==zaffiro || sol[pos-1]==topazio) fine = true;
            if((nPietre[rubino]==0 && nPietre[zaffiro]==1) || (nPietre[topazio]==0 && nPietre[rubino]==1)) return false;
            break;
        case smeraldo:
            if(sol[pos-1]==smeraldo || sol[pos-1]==rubino) fine = true;
            if((nPietre[smeraldo]==0 && nPietre[topazio]==1) || (nPietre[topazio]==0 && nPietre[smeraldo]==1)) return false;
            break;
        case rubino:
            if(sol[pos-1]==zaffiro || sol[pos-1]==topazio) fine = true;
            if((nPietre[smeraldo]==0 && nPietre[topazio]==1) || (nPietre[topazio]==0 && nPietre[smeraldo]==1)) return false;
            break;
        case topazio:
            if(sol[pos-1]==smeraldo || sol[pos-1]==rubino) fine = true;
            if((nPietre[rubino]==0 && nPietre[zaffiro]==1) || (nPietre[topazio]==0 && nPietre[rubino]==1)) return false;
            break;
        default:
            break;
    }
    return fine;
}

void printStart(int nPietre[], int nPietreTot, int nTest)
{
    printf("TEST #%d\nzaffiro=%d rubino=%d topazio=%d smeraldo=%d TOT=%d\n", nTest, nPietre[zaffiro], nPietre[rubino], nPietre[topazio], nPietre[smeraldo], nPietreTot);
}
