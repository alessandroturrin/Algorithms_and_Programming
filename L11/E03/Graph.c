#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"

int lista = 0;
int ordinati = 1;

typedef struct node *link;
struct node
{
    int v; int wt; link next;
};
struct graph
{
    int V; int E, **madj; //numero di vertici | numero di archi
    link *ladj; //lista di adiacenza
    ST tab, tabOrd; //tabella di simboli
    link z; //sentinella
};

static int **MATRIXint(int r, int c, int val)
{
    int i, j;
    int **t = malloc(r*sizeof(int*));
    for(i=0;i<r;i++)
        t[i] = malloc(c*sizeof(int));
    for(i=0;i<r;i++)
        for(j=0;j<r;j++)
            t[i][j] = val;
    return t;
}

static void  insertE(Graph G, Edge e)
{
    int v = e.v, w = e.w, wt =e.wt;
    if (G->madj[v][w] == 0)
        G->E++;
    G->madj[v][w] = wt;
    G->madj[w][v] = wt;
}

static link NEW(int v, int wt, link next)
{
    link x = malloc(sizeof *x);
    x->v = v;
    x->wt = wt;
    x->next = next;
    return x;
}

static Edge EDGEcreate(int v, int w, int wt)
{
    Edge e;
    e.v = v; e.w = w; e.wt = wt;
    return e;
}

void madjToLadj(Graph G)
{
    int v, w, wt;
    G->z = NEW(-1, -1, NULL);
    G->ladj = malloc(G->V*sizeof(link));
    for(int i=0;i<G->V;i++)
        G->ladj[i] = G->z;
    for(int i=0;i<G->V;i++)
    {
        for(int j=0;j<G->V;j++)
        {
            if(G->madj[i][j]!=0)
            {
                v = i;
                w = j;
                wt = G->madj[i][j];
                G->ladj[v] = NEW(w, wt, G->ladj[v]);
            }
        }
    }
    link x;
    for(int i=0;i<G->V;i++)
    {
        printf("| %d |->%d/%d", i, G->ladj[i]->v, G->ladj[i]->wt);
        x = G->ladj[i]->next;
        while(x!=G->z)
        {
            printf("->%d/%d", x->v, x->wt);
            x = x->next;
        }
        printf("\n");
    }
    lista = 1;
}

Graph GRAPHinit(int V)
{
    Graph G = malloc(sizeof(*G));
    G->V = V;
    G->E = 0;
    G->madj = MATRIXint(V, V, 0);
    G->tab = STinit(V);
    return G;
}

Graph GRAPHload(FILE *fin)
{
    int E=0, i, id1, id2, wt=0;
    Graph G;
    int peso;
    Item I1, I2;
    
    while (fscanf(fin, "%s %s %s %s %d", I1.id_elab, I1.id_elab, I2.id_rete, I2.id_rete, &peso)==5)
        E++;
    rewind(fin); //sposta il cursore a inizio del file
    G = GRAPHinit(2*E);
    for(i=0;i<E;i++)
    {
        fscanf(fin, "%s %s %s %s %d", I1.id_elab, I1.id_rete, I2.id_elab, I2.id_rete, &peso);
        STinsert(G->tab, I1);
        STinsert(G->tab, I2);
    }
    G->V = getN(G->tab);
    rewind(fin);
    for(i=0;i<E;i++)
    {
        fscanf(fin, "%s%s%s%s%d", I1.id_elab, I1.id_rete, I2.id_elab, I2.id_rete, &wt);
        id1=STsearch(G->tab, I1);
        id2=STsearch(G->tab, I2);
        if(id1>=0 && id2>=0)
            GRAPHinsertE(G, id1, id2, wt);
    }
    return G;
}

void GRAPHinsertE(Graph G, int id1, int id2, int wt)
{
    insertE(G, EDGEcreate(id1, id2, wt));
}

void GRAPHfree(Graph G)
{
    int e;
    link t, next;
    
    for(int i=0;i<(2*G->E);i++)
        free(G->madj[i]);
    free(G->madj);
    
    STfree(G->tabOrd);
    if(ordinati)
        STfree(G->tab);
    if(lista)
    {
        for (e=0; e<G->E;e++)
            for (t=G->ladj[e];t!=G->z;t=next)
            {
                next = t->next;
                free(t);
            }
        free(G->ladj);
        free(G->z);
    }
    free(G);
}

void verticiOrdinati(Graph G)
{
    G->tabOrd = STcopy(G->tab, G->V);
    G->tabOrd = STsort(G->tabOrd, G->V);
    STprint(G->tab, G->tabOrd, G->V, G->madj);
    ordinati = 1;
}

int sottoGrafo(Graph G)
{
    char name[DSG][maxChar+1], scelta[maxChar+1];
    int i, vertici[DSG];
    Item item;
    
    for(i=0;i<DSG;i++)
    {
        printf("%d* nome: ", i+1);
        scanf("%s", name[i]);
        strcpy(item.id_elab, name[i]);
        vertici[i]=STsearch(G->tab, item);
        if(vertici[i]==-1)
        {
            printf("Elemento non presente nel grafo!\n");
            return 0;
        }
    }
    printf("Verificare se i %d vertici formano un sottografo completo usando matrice o lista di adiacenza?\n", DSG);
    scanf("%s", scelta);
    if(strcmp(scelta, "matrice")==0)
    {
        for(i=0;i<DSG;i++)
            if(G->madj[vertici[i%DSG]][vertici[(i+1)%DSG]]==0) return 0;
    }
    
    else if(strcmp(scelta, "lista")==0)
        if(lista)
        {
            link x; int presente=0;
            for(i=0;i<DSG;i++)
            {
                for(x=G->ladj[vertici[i%DSG]];x!=G->z && presente==0;x=x->next)
                {
                    if(x->v==vertici[(i+1)%DSG]) presente=1;
                }
                if (!presente) return 0;
                presente=0;
            }
        }
    return 1;
}

