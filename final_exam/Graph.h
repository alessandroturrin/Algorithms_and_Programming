#ifndef Graph_h
#define Graph_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxC 100

typedef struct
{
    char *nome;
    int profondita, tesoro, oro;
}Stanza;

typedef struct
{
    int N;
    Stanza *stanza;
    int *raccolto;
    int oroTotale;
    int tesoro;
    int totale;
}Path;

typedef struct
{
    int v, w, wt;
}Edge;

typedef struct graph *Graph;
typedef struct symboltable *ST;

Graph GRAPHload(FILE *fp);
Path GRAPHpathLoad(Graph g, FILE* fp);
int GRAPHpathCheck(Graph g, Path p, int M, int PF);
Path GRAPHpathBest(Graph g, int M, int PF);
void GRAPHfree(Graph G);
#endif /* Graph_h */
