#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

#define MAXC 30

typedef struct node *link;

struct node
{
    int v, wt;
    link next;
};

struct graph
{
    int V, E;
    link *ladj, *ladjProv, z;
    int **madj;
    ST tab;
};

typedef struct
{
    Edge *v;
    int wt;
}minCard;

static link NEW(int v, int wt, link next)
{
    link x=malloc(sizeof *x);
    x->v=v; x->wt=wt; x->next=next;
    return x;
}

static int **MATRIXinit(int r, int c, int val)
{
    int i, j, **m;
    m=malloc(r*sizeof(int*));
    for(i=0;i<r;i++)
        m[i]=malloc(c*sizeof(int));
    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
            m[i][j]=val;
    return m;
}

static Edge EDGEcreate(int v, int w, int wt)
{
    Edge e;
    e.v=v; e.w=w; e.wt=wt;
    return e;
}

static void insertE(Graph G, Edge e)
{
    int v=e.v, w=e.w, wt=e.wt;
    if(G->madj[v][w]==0)
        G->E++;
    G->madj[v][w]=wt;
    G->ladj[v]=NEW(w, wt, G->ladj[v]);
}

static void GRAPHinsertE(Graph G, int id1, int id2, int wt)
{
    insertE(G, EDGEcreate(id1, id2, wt));
}

static void removeE(Graph G, Edge e)
{
    int v=e.v, w=e.w;
    link x, next;
    if(G->madj[v][w]!=0)
        G->E--;
    G->madj[v][w]=0;
    if(G->ladj[v]->v==w)
    {
        x=G->ladj[v];
        G->ladj[v]=G->ladj[v]->next;
        free(x);
        return;
    }
    for(link x=G->ladj[v];x!=G->z;x=x->next)
        if(x->next->v==w)
        {
            next = x->next;
            x->next = x->next->next;
            free(next);
        }
}

static void LISTdealloc(Graph G)
{
    link t, next;
    int i;
    for(i=0;i<G->V;i++)
    {
        for(t=G->ladjProv[i];t!=G->z;t=next)
        {
            next=t->next;
            free(t);
        }
        for(i=0;i<G->V;i++)
            G->ladjProv[i]=G->z;
    }
}

static void LISTinit(Graph G, Edge archi[], int mark[])
{
    for(int i=0;i<G->E;i++)
            if(mark[i]==1)
                G->ladjProv[archi[i].v]=NEW(archi[i].w, archi[i].wt, G->ladjProv[archi[i].v]);
}

static void GRAPHremoveE(Graph G, int id1, int id2)
{
    removeE(G, EDGEcreate(id1, id2, 0));
}

static void printSpace(int n)
{
    for(int i=0;i<n;i++)
        printf("-");
    printf("\n");
}

Graph GRAPHinit(int V)
{
    Graph G=malloc(sizeof *G);
    G->V=V;
    G->E=0;
    G->madj=MATRIXinit(V, V, 0);
    G->z=NEW(-1, -1, NULL);
    G->ladj=malloc(G->V*sizeof(link));
    G->ladjProv=malloc(G->V*sizeof(link));
    for(int v=0;v<G->V;v++)
    {
        G->ladj[v]=G->z;
        G->ladjProv[v]=G->z;
    }
    G->tab=STinit(V);
    return G;
}

void GRAPHfree(Graph G)
{
    link t, next;
    for(int v=0;v<G->V;v++)
    {
        free(G->madj[v]);
        for(t=G->ladj[v];t!=G->z;t=next)
        {
            next=t->next;
            free(t);
        }
        for(t=G->ladjProv[v];t!=G->z;t=next)
        {
            next=t->next;
            free(t);
        }
    }
    free(G->madj);
    free(G->ladj);
    free(G->ladjProv);
    free(G->z);
    STfree(G->tab);
    free(G);
}

Graph GRAPHload(FILE *fp)
{
    int v, id1, id2, wt;
    char name1[MAXC+1], name2[MAXC+1];
    Graph G;
    fscanf(fp, "%d", &v);
    G=GRAPHinit(v);
    for(v=0;v<G->V;v++)
    {
        fscanf(fp, "%s", name1);
        STinsert(G->tab, name1);
    }
    while(fscanf(fp, "%s%s%d", name1, name2, &wt)==3)
    {
        id1=STsearchItem(G->tab, name1);
        id2=STsearchItem(G->tab, name2);
        if(id1>=0 && id2>=0)
            GRAPHinsertE(G, id1, id2, wt);
    }
    return G;
}

int GRAPHgetIndex(Graph G, char *name){
    return STsearchItem(G->tab, name);
}

void GRAPHedges(Graph G, Edge *a){
    int v, E=0;
    link t;
    for(v=0;v<G->V;v++)
        for(t=G->ladj[v];t!=G->z;t=t->next)
            a[E]=EDGEcreate(v, t->v, t->wt);
}

void GRAPHdisplay(Graph G)
{
    int v, w;
    link x;
    
    printf("\n\n\n");
    printSpace(11);
    printf("Vertici: %d\nArchi: %d\n", G->V, G->E);
    printSpace(11);
    printf("\n");
    printf("---MATRICE DI ADIACENZA--- \n");
    for(v=0;v<G->V;v++)
    {
        printf("%s|\t", ITEMsearch(G->tab, v));
        for(w=0;w<G->V;w++)
        {
            if(G->madj[v][w]<10)
                printf("0");
            printf("%d ", G->madj[v][w]);
        }
        printf(" |\n");
    }
    printSpace(26);
    printf("\n\n----LISTA DI ADIACENZA---- \n");
    for(v=0;v<G->V;v++)
    {
        printf("%s| ", ITEMsearch(G->tab, v));
        for(x=G->ladj[v];x!=G->z;x=x->next)
            printf("->%s/%d", ITEMsearch(G->tab,x->v), x->wt);
        printf("\n");
    }
    printSpace(26);
    printf("\n\n");
}

static int dfsR(Graph G, Edge e, int *time, int *pre, int *post, int *st)
{
    link t;
    int v, w=e.w;
    st[e.w]=e.v;
    pre[w]=(*time)++;
    for(t=G->ladjProv[w];t!=G->z;t=t->next){
        if(pre[t->v]==-1){
            if(dfsR(G, EDGEcreate(w, t->v, t->wt), time, pre, post, st)==1)
                return 1;
        }
        else{
            v=t->v;
            if(post[v]==-1) //arco back
                return 1;
        }
    }
    post[w]=(*time)++;
    return 0;
}

int GRAPHdfs(Graph G, int id, int *pre, int *post, int *st){
    int v, time=0;
    for(v=0;v<G->V;v++){
        pre[v]=-1;
        post[v]=-1;
        st[v]=-1;
    }
    if(dfsR(G, EDGEcreate(id, id, 0), &time, pre, post, st)==1)
        return 1;
    for(v=0;v<G->V;v++)
        if(pre[v]==-1)
            if(dfsR(G, EDGEcreate(v, v, 0), &time, pre, post, st)==1)
                return 1;
    return 0;
}

static int powerset(Graph G, int *mark, Edge *archi, int j, int pos, int start, int *pre, int *post, int *st)
{
    int i;
    if(pos==j)
    {
        LISTdealloc(G);
        for(i=0;i<G->V;i++)
        for(i=0;i<G->E;i++)
            if(mark[i]==1)
                G->ladjProv[archi[i].v]=NEW(archi[i].w, archi[i].wt, G->ladjProv[archi[i].v]);
        return GRAPHdfs(G, 0, pre, post, st);

    }
    for(i=start;i<G->E;i++)
    {
        mark[i]=0;
        if(powerset(G, mark, archi, j, pos+1, i+1, pre, post, st)==0)
            return 0;
        mark[i]=1;
    }
    return 1;
}

static void comb(Graph G, int pos, Edge *archi,  minCard **sol, int *mark, int k, int *MAXN, int *dim,  int *pre, int *post, int *st, int start)
{
    int i;
    if(pos>=k)
    {
        LISTdealloc(G);
        for(i=0;i<G->E;i++)
            if(mark[i]==1)
                G->ladjProv[archi[i].v]=NEW(archi[i].w, archi[i].wt, G->ladjProv[archi[i].v]);
        if(GRAPHdfs(G, 0, pre, post, st)==0)
        { //non ci sono archi back
            if(*dim>=*MAXN)
            {
                *MAXN=*MAXN*2;
                *sol=realloc(*sol, *MAXN*sizeof(minCard));
            }
            int j=0;
            (*sol)[*dim].v=malloc(k*sizeof(Edge));
            (*sol)[*dim].wt = 0;
            for(i=0;i<G->E && j<k;i++)
            {
                if(mark[i]==0)
                {
                    (*sol)[*dim].v[j++]=archi[i];
                    (*sol)[*dim].wt+=archi[i].wt;
                }
            }
            (*dim)++;
        }
        return;
    }
    for(i=start;i<G->E;i++)
        if(mark[i]==1)
        {
            mark[i]=0;
            comb(G, pos+1, archi, sol, mark , k, MAXN, dim, pre, post, st, i+1);
            mark[i]=1;
        }
}

void DAG(Graph G)
{
    int i, j, k=0, minArchi, *mark, *pre, *post, *st, *ordTopologico, max, *maxDist, index=0;
    int MAXN=1, dim=0;
    minCard toRemove, *v;
    mark=malloc(G->E*sizeof(int));
    pre=malloc(G->V*sizeof(int));
    st=malloc(G->V*sizeof(int));
    post=malloc(G->V*sizeof(int));
    Edge *archi=malloc(G->E*sizeof(Edge));
    v=malloc(MAXN*sizeof(minCard));
    

    //INIZIO PUNTO 1
    printf("\n\n-----PUNTO 1-----\n");
    for(i=0;i<G->E;i++)
        mark[i]=1;
    for(int v=0;v<G->V;v++)
        for(int w=0;w<G->V;w++)
            if(G->madj[v][w]!=0)
                archi[k++]=EDGEcreate(v, w, G->madj[v][w]);
    LISTinit(G, archi, mark);
    if(GRAPHdfs(G, 0, pre, post, st)==0)
        printf("Cardinalità minima -> DAG: 0\n");
    else
    {
        for(j=1;j<=G->E;j++)
        {
            if(powerset(G, mark, archi, j, 0, 0, pre, post, st)==0) //ho un dag
                break;
        }
        minArchi=j;
        printf("Cardinalita' minima -> DAG: %d\n", minArchi);
        printSpace(17);
        //FINE PUNTO 1
        
        //INIZIO PUNTO 2
        printf("\n-----PUNTO 2-----");
        for(i=0;i<G->E;i++)
            mark[i]=1;
        comb(G, 0, archi, &v, mark, minArchi, &MAXN, &dim, pre, post, st, 0);
        printf("\n[Archi dell'insieme]\n");
        for(i=0;i<dim;i++)
        {
            printf("(%d) -> ", i+1);
            for(j=0;j<minArchi;j++)
                printf("|%s %s|\t", ITEMsearch(G->tab, v[i].v[j].v), ITEMsearch(G->tab, v[i].v[j].w));
            printf("\n");
        }
        
        LISTdealloc(G);
        toRemove.v = malloc(minArchi*sizeof(Edge));
        toRemove.wt = 0;
        for(i=0;i<dim;i++)
            if(v[i].wt>toRemove.wt)
                toRemove = v[i];
        printf("\n[Peso massimo]: %d ->", toRemove.wt);
        for(i=0;i<minArchi;i++)
        {
            printf("|%s %s|\t", ITEMsearch(G->tab, toRemove.v[i].v), ITEMsearch(G->tab, toRemove.v[i].w));
            GRAPHremoveE(G, toRemove.v[i].v, toRemove.v[i].w);
        }
        printf("\n");
        printSpace(17);
    }
    //FINE PUNTO 2
        
    //INIZIO PUNTO 3
    printf("\n-----PUNTO 3-----\n");
    k=0;
    for(i=0;i<G->E;i++)
        mark[i]=1;
    for(int v=0;v<G->V;v++)
        for(int w=0;w<G->V;w++)
            if(G->madj[v][w]!=0)
                archi[k++]=EDGEcreate(v, w, G->madj[v][w]);
    LISTdealloc(G);
    LISTinit(G, archi, mark);
    
    GRAPHdfs(G, 0, pre, post, st);
    ordTopologico = malloc(G->V*sizeof(int));
    maxDist = calloc(G->V, sizeof(int));

        for(i=0;i<G->V;i++){
            max=0;
            for(k=0;k<G->V;k++){
                if(post[k]>max){
                    max=post[k];
                    index=k;
                }
            }
            post[index]=-1;
            ordTopologico[i]=index;
        }
    int w, wt;
    for(i=0;i<G->V;i++)
    {
        for(link x=G->ladj[i];x!=G->z;x=x->next)
        {
            w = x->v;
            wt = x->wt;
            if(maxDist[w]<maxDist[i]+wt)
            {
                maxDist[w] = maxDist[i]+wt;
            }
        }
    }
    printf("[Nodo sorgente] |%s|\n", ITEMsearch(G->tab, ordTopologico[0]));
    for(i=0;i<G->V;i++)
    {
        printf("%s| ", ITEMsearch(G->tab, ordTopologico[i]));
        if(maxDist[i]<100)
        {
            if(maxDist[i]<10)
                printf("00");
            else
                printf("0");
        }
        printf("%d\n", maxDist[i]);
    }
    printSpace(17);
    printf("\n\n\n");
    
    //FINE PUNTO 3
    
    
    free(mark);
    free(pre);
    free(post);
    free(st);
    free(archi);
    free(ordTopologico);
    free(maxDist);
    for(i=0;i<dim;i++)
        free(v[i].v);
    free(v);
}
