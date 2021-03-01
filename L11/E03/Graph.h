#ifndef Graph_h
#define Graph_h

#include "St.h"

#define DSG 3

typedef struct edge
{
    int v; int w; int wt;
} Edge;
typedef struct graph *Graph;

void madjToLadj(Graph G);
Graph GRAPHinit(int V);
Graph GRAPHload(FILE *fin);
void GRAPHinsertE(Graph G, int id1, int id2, int wt);
void GRAPHfree(Graph G);
void verticiOrdinati(Graph G);
int sottoGrafo(Graph G);
#endif /* Graph_h */
