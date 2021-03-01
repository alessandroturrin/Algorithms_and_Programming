#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int u,v;
}vertex;

void input(vertex **e, int *N, int *E);
void powerSWrapper(vertex *e, int E, int N);
int powerS(vertex *e, int pos, int *val, int *sol, int k, int start, int count, int E);
int isVertexCover(vertex *e, int *sol, int pos, int E);
void printVertex(int *sol, int n);
int main()
{
    vertex *e;
    int N, E; //intervallo numeri e numero archi
    
    input(&e, &N, &E);
    powerSWrapper(e, N, E);
}

void input(vertex **e, int *N, int *E)
{
    FILE* f;
    char fileName[] = "grafo.txt";
    
    if((f=fopen(fileName, "rt"))==NULL)
    {
        printf("Errore nell'apertura di '%s'\nFine dell'esecuzione del programma!\n", fileName);
        exit(1);
    }
    fscanf(f, "%d %d", N, E);
    vertex *e1 = (vertex*)malloc((*E)*sizeof(vertex));
    for(int i=0;i<(*E);i++)
        fscanf(f,"%d %d", &e1[i].u, &e1[i].v);
    *e = e1;
}

void powerSWrapper(vertex *e, int E, int N)
{
    int *val, *sol, totalV;
    val = (int*)malloc(N*sizeof(int));
    sol = (int*)malloc(N*sizeof(int));
    for(int i=0;i<N;i++) val[i] = i;
    printf("---Vertex Cover---\n");
    totalV = powerS(e, 0, val, sol, N, 0, 0, E);
}

int powerS(vertex *e, int pos, int *val, int *sol, int k, int start, int count, int E)
{
    int i;
    if(start>=k)
    {
        if(isVertexCover(e, sol, pos, E))
            printVertex(sol, pos);
        return count+1;
    }
    for(i=start;i<k;i++)
    {
        sol[pos] = val[i];
        count = powerS(e, pos+1, val, sol, k, i+1, count, E);
    }
    count = powerS(e, pos, val, sol, k, k, count, E);
    return count;
}

int isVertexCover(vertex *e, int *sol, int pos, int E)
{
    int isVertex = 1, has;
    
    for(int i=0;i<E;i++)
    {
        has = 0;
        for(int j=0;j<pos;j++)
        {
            if(sol[j]==e[i].u || sol[j]==e[i].v)
            {
                has = 1;
                break;
            }
        }
        if(!has)
        {
            isVertex = 0;
            break;
        }
    }
    return isVertex;
}

void printVertex(int *sol, int n)
{
    printf("(");
    for(int i=0;i<n;i++) printf(" %d ", sol[i]);
    printf(")\n");
}

