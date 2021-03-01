#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int s, f; //inizio | fine
}att; //attività

typedef enum {false, true} bool;

att *leggiFile(int *N);
void attSel(int N, att *v);
void attSelRic(att *v, int* sol, int *bestSol, int pos, int N, int *maxSum, int currentSum);
bool sovrappone(att* v, int *sol, int pos);
void stampa(att v[], int sol[], int N, int maxSum);
int main()
{
    att *v;
    int N;
    
    v = leggiFile(&N);
    attSel(N, v);
}

att *leggiFile(int *N)
{
    att *vc;
    char nomeFile[] = "att1.txt";
    FILE* file = fopen(nomeFile, "rt");
    
    if(file==NULL)
    {
        printf("Errore nell'apertura del file '%s'!\n", nomeFile);
        exit(1);
    }
    //input numero intervalli
    fscanf(file, "%d", N);
    //allocazione vettori
    vc = (att*)malloc((*N)*sizeof(att));
    //input N attività
    for(int i=0;i<*N;i++)
    fscanf(file, "%d %d", &vc[i].s, &vc[i].f);
    return vc;
}

void attSel(int N, att *v)
{
    int *sol = (int*)malloc(N*sizeof(int)); //assume valori 0|1
    int *bestSol = (int*)malloc(N*sizeof(int)); //assume valori soluzione migliore 0|1
    int maxSum = 0; //somma durata maggiore
    
    attSelRic(v, sol, bestSol, 0, N, &maxSum, 0);
    stampa(v, bestSol, N, maxSum);
}

void attSelRic(att *v, int* sol, int *bestSol, int pos, int N, int *maxSum, int currentSum)
{
    if(pos>=N)
    {
        if(currentSum>*maxSum)
        {
            *maxSum = currentSum;
            for(int i=0;i<N;i++)
                bestSol[i] = sol[i];
        }
        return; //fine ricorsione
    }
    
    sol[pos] = 0;
    attSelRic(v, sol, bestSol, pos+1, N, maxSum, currentSum);
    if(!sovrappone(v, sol, pos))
    {
        sol[pos] = 1; //marco sol come utilizzato, valore valido
        currentSum = currentSum + (v[pos].f-v[pos].s);
        attSelRic(v, sol, bestSol, pos+1, N, maxSum, currentSum);
    }
    
}

bool sovrappone(att* v, int *sol, int pos)
{
    for(int i=0;i<pos;i++)
        if(sol[i]!=0)
            if(v[i].f>v[pos].s)
                return true; //si sovrappone
    return false; //non si sovrappone
}

void stampa(att v[], int sol[], int N, int maxSum)
{
    printf("Somma delle durate massima: %d\nCatena delle durate massima: ", maxSum);
    for(int i=0;i<N;i++)
        if(sol[i]==1)
            i!=N-1 ? printf("(%d,%d), ", v[i].s, v[i].f) : printf("(%d,%d)\n", v[i].s, v[i].f);
}
