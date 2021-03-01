#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

#define fileGrafo "grafo1.txt"
#define filePath "path1.txt"

#define M 6
#define PF 4

int main(int argc, const char * argv[])
{
    Graph G;
    Path p;
    FILE *fG = fopen(fileGrafo, "rt");
    FILE *fP = fopen(filePath, "rt");
    
    G = GRAPHload(fG);
    p = GRAPHpathLoad(G, fP);
    if(GRAPHpathCheck(G, p, 6, 4)==1)
        printf("Il percorso contenuto nel file '%s' e' valido!\n",fileGrafo);
    else
        printf("Il percorso contenuto nel file '%s' non e' valido!\n", filePath);
    p = GRAPHpathBest(G, M, PF);
    GRAPHfree(G);
}
