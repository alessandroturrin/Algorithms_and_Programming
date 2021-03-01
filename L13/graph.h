#ifndef graph_h
#define graph_h

#include "St.h"

typedef struct
{
    int v, w, wt;
}Edge;

typedef struct list *List;
typedef struct graph *Graph;

Graph GRAPHload(FILE *fin);
Graph GRAPHinit(int V);
void DAG(Graph G);
void GRAPHdisplay(Graph G);
void GRAPHfree(Graph G);
#endif /* graph_h */
