#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int s, f; //inizio | fine
}att; //attività

typedef enum {false, true} bool;

att *leggiFile(int *N);
void insertionSort(att *v, int N);
void lisDP(att *v, int N);
void lisPrint(att *v, int *P, int i);
int main()
{
    att *v;
    int N;
    
    v = leggiFile(&N);
    insertionSort(v, N); //ordinamento crescente secondo orario di fine
    for(int i=0;i<N;i++)
    printf("%d %d\n", v[i].s, v[i].f);
    puts("");
    lisDP(v, N);
}

att *leggiFile(int *N)
{
    att *vc;
    char nomeFile[] = "/Users/alexturrin/Desktop/att2.txt";
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

void insertionSort(att *v, int N)
{
    int i, j, l=0, r=N-1;
    att x;
    for(i=l+1;i<=r;i++)
    {
        x = v[i];
        j = i-1;
        while(j>=l && x.f<v[j].f)
        {
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = x;
    }
}

void lisDP(att *v, int N)
{
    int i, j, t = 0, last = 0;
    int *P, *L;
    
    P = calloc(N, sizeof(int));
    L = calloc(N, sizeof(int));
    
    L[0] = v[0].f-v[0].s;
    P[0] = -1;
    for(i=1;i<N;i++)
    {
        L[i] = v[i].f-v[i].s; P[i] = -1;
        for(j=0;j<i;j++)
        {
            if(v[j].f<=v[i].s && L[i]<L[j]+v[i].f-v[i].s)
            {
                L[i] = L[j] + v[i].f-v[i].s;
                P[i] = j;
            }
        }
        if(t<=L[i])
        {
            t = L[i];
            last = i;
        }
    }
    lisPrint(v, P, last);
}

void lisPrint(att *v, int *P, int i)
{
    if(P[i]==-1)
    {
        printf("(%d,%d) ", v[i].s, v[i].f);
        return;
    }
    lisPrint(v, P, P[i]);
    printf("(%d,%d) ", v[i].s, v[i].f);
}



