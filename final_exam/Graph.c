#include "Graph.h"

struct symboltable
{
    char **a;
    int N, M;
};

struct graph
{
    int V, E;
    int **madj;
    ST tab;
    Stanza *stanza;
};

//#################FUNZIONI MATRICE DI ADIACENZA##############
static int **MATRIXinit(int r, int c, int val)
{
    int **M = malloc(r*sizeof(int*));
    for(int i=0;i<r;i++)
    {
        M[i] = malloc(c*sizeof(int));
        for(int j=0;j<c;j++)
            M[i][j] = val;
    }
    return M;
}

static void MATRIXfree(int **m, int r)
{
    for(int i=0;i<r;i++)
        free(m[i]);
    free(m);
}
//###########################################################

//################FUNZIONI TABELLA DI SIMBOLI################
static ST STinit(int maxN)
{
    ST st;
    st = malloc(sizeof(struct symboltable));
    st->a = malloc(maxN*sizeof(char*));
    st->M = maxN;
    st->N = 0;
    return st;
}

static void STinsert(ST st, char *el)
{
    st->a[st->N++] = strdup(el);
}

static int STsearch(ST st,char *el)
{
    for(int i=0;i<st->N;i++)
        if(!strcmp(st->a[i], el))
            return i;
    return -1;
}

static void STfree(ST st)
{
    for(int i=0;i<st->N;i++)
        free(st->a[i]);
    free(st->a);
    free(st);
}
//###########################################################

//###############FUNZIONI GRAFO##############################
static Graph GRAPHinit(int V)
{
    Graph G = malloc(sizeof(struct graph));
    G->V = V;
    G->E = 0;
    G->madj = MATRIXinit(V, V, 0);
    G->tab = STinit(V);
    return G;
}

static Edge EDGEcreate(int v, int w, int wt)
{
    Edge e;
    e.v = v;
    e.w = w;
    e.wt = wt;
    return e;
}

static void GRAPHinsert(Graph G, Edge e)
{
    if(G->madj[e.v][e.w]==0)
        G->E++;
    G->madj[e.v][e.w] = e.wt;
    G->madj[e.w][e.v] = e.wt;
}

static void PATHcreate(Path p, Path *P)
{
    for(int i=0;i<p.N;i++)
    {
        (*P).stanza[i].nome = p.stanza[i].nome; //assegno puntatore
        (*P).stanza[i].profondita = p.stanza[i].profondita;
        (*P).stanza[i].tesoro = p.stanza[i].tesoro;
        (*P).stanza[i].oro = p.stanza[i].oro;
    }
    (*P).N = p.N;
    (*P).oroTotale = p.oroTotale;
    (*P).tesoro = p.tesoro;
    (*P).totale = p.totale;
}

static void GRAPHpathPrint(Path p)
{
    printf("\n###SOLUZIONE OTTIMA###\n");
    if(p.N==-1)
    {
        printf("Path is void!\n");
        return;
    }
    printf("Oro: %d\nTesoro: %d\nGuadagno totale: %d\n\n", p.oroTotale, p.tesoro, p.totale);
    for(int i=0;i<p.N;i++)
        printf("%s %d %d %d\n", p.stanza[i].nome, p.stanza[i].profondita, p.stanza[i].tesoro, p.stanza[i].oro);
    printf("######################\n");
}

static void calcolaRisorse(Path *p)
{
    int last = (*p).N-1;
    
    (*p).totale = (*p).oroTotale+(*p).tesoro;

    if((*p).stanza[last].profondita==1)
        (*p).totale = (*p).totale - (*p).totale/3-1;
    if((*p).stanza[last].profondita==2)
        (*p).totale = (*p).totale - (*p).totale/2-1;
}


static Path GRAPHpathBestR(Graph g, Path p, Path bestP, int M, int PF, int v, int pos)
{
    if((pos>0 && v==0) || pos>=M)
    {
        calcolaRisorse(&p);
        if(p.totale>bestP.totale)
            if(GRAPHpathCheck(g, p, M, PF))
                PATHcreate(p, &bestP);
        return bestP;
    }
    
    for(int t=0;t<g->V;t++)
    {
        if(g->madj[v][t]>0)
        {
            p.stanza[p.N++] = g->stanza[t];
            if(p.tesoro==0 || g->stanza[t].tesoro>p.tesoro)
                p.tesoro = g->stanza[t].tesoro;
            
            if(p.raccolto[t]==0)
            {
                p.raccolto[t] = 1;
                p.oroTotale = p.oroTotale + g->stanza[t].oro;
            }
            else
                p.raccolto[t]++;
            
            bestP = GRAPHpathBestR(g, p, bestP, M, PF, t, pos+1);
            
            p.tesoro = 0;
            if(p.raccolto[t]==1)
            {
                p.raccolto[t] = 0;
                p.oroTotale = p.oroTotale - g->stanza[t].oro;
            }
            else
                p.raccolto[t]--;
    
            p.N--;
        }
    }
    return bestP;
}

Graph GRAPHload(FILE *fp)
{
    Graph G;
    int S, trappola;
    char id1[maxC], id2[maxC], nome[maxC];
    int i1,i2;
    
    if(fp==NULL)
    {
        printf("Errore nella lettura del file!\n");
        exit(EXIT_FAILURE);
    }
    
    fscanf(fp,"%d", &S);
    G = GRAPHinit(S);
    G->stanza = malloc(S*sizeof(Stanza));
    
    for(int i=0;i<S;i++)
    {
        fscanf(fp, "%s %d %d %d", nome, &(G->stanza[i].profondita), &(G->stanza[i].tesoro), &(G->stanza[i].oro));
        G->stanza[i].nome = strdup(nome);
        STinsert(G->tab, nome);
    }
    
    while (fscanf(fp,"%s %s %d", id1, id2, &trappola)==3)
    {
        i1 = STsearch(G->tab, id1);
        i2 = STsearch(G->tab, id2);
        if(i1>=0 && i2>=0)
            GRAPHinsert(G, EDGEcreate(i1, i2, trappola+1)); //trappola==1 ->no trappola | trappola==2 si trappola
    }
    fclose(fp);
    return G;
}

Path GRAPHpathLoad(Graph g, FILE* fp)
{
    Path p;
    char nome[maxC];
    int index;
    
    if(fp==NULL)
    {
        printf("Errore nella lettura del file!\n");
        exit(EXIT_FAILURE);
    }
    
    fscanf(fp, "%d", &p.N);
    p.stanza = malloc(p.N*sizeof(Stanza));
    p.raccolto = calloc(p.N, sizeof(int));
    
    for(int i=0;i<p.N;i++)
    {
        fscanf(fp, "%s", nome);
        p.stanza[i].nome = strdup(nome);
        index = STsearch(g->tab, nome);
        if(index>=0)
        {
            p.stanza[i].profondita = g->stanza[index].profondita;
            p.stanza[i].tesoro = g->stanza[index].tesoro;
            p.stanza[i].oro = g->stanza[index].oro;
        }
    }
    
    return p;
}

int GRAPHpathCheck(Graph g, Path p, int M, int PF)
{
    int i1, i2;
    int last = p.stanza[(p.N)-1].profondita;
    M++; //non considero l'ingresso
    
    if((strcmp(p.stanza[0].nome,"Ingresso"))!=0 || last>2 || p.N>M)
      return 0;
    
    for(int i=0;i<p.N-1;i++)
    {
        i1 = STsearch(g->tab, p.stanza[i].nome);
        i2 = STsearch(g->tab, p.stanza[i+1].nome);
       
        if(i1<0 || i2<0)//elemento non presente nel grafo
            return 0;
        
        if(g->madj[i1][i2]==0) //stanze non connesse
            return 0;
        else
        {
            if(g->madj[i1][i2]==2) //trappola
            {
                if(PF>0)
                    PF--;
                else
                    return 0;
            }
        }
    }
    
    return 1;
}

Path GRAPHpathBest(Graph g, int M, int PF)
{
    Path p, bestP;
    p.N = 1;
    p.stanza = malloc((M+1)*sizeof(Stanza));
    bestP.stanza = malloc((M+1)*sizeof(Stanza));
    p.raccolto = calloc(g->V, sizeof(int));
    p.stanza = malloc(M*sizeof(Stanza));
    p.oroTotale = p.tesoro = bestP.oroTotale = bestP.tesoro = p.totale =bestP.totale = 0;
    
    p.stanza[0] = g->stanza[0];
    bestP = GRAPHpathBestR(g, p, bestP, M, PF, 0, 0);
    GRAPHpathPrint(bestP);
    return bestP;
}

void GRAPHfree(Graph G)
{
    STfree(G->tab);
    MATRIXfree(G->madj, G->V);
    free(G->stanza);
    free(G);
}
