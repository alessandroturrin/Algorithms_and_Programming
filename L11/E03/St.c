#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "St.h"

struct symboltable
{
    Item *a;
    int M; int N;
};

Item ITEMsearch(ST st, int index)
{
    if(index<st->N)
        return st->a[index];
    else
        return ITEMsetvoid();
}

ST STinit(int maxN)
{
    ST st;
    st = malloc(sizeof(*st));
    st->a = malloc(maxN * sizeof(Item) );
    st->M = maxN; st->N= 0; return st;
}

void STinsert(ST st, Item val)
{
    int index = STsearch(st, val);
    if(index==-1)
        st->a[st->N++] = val;
}

int STsearch(ST st, Item val)
{
    for(int i=0;i<st->N;i++)
    {
        if(strcmp(st->a[i].id_elab,val.id_elab)==0)
            return i;
    }
    return -1;
}

void STfree(ST st)
{
    free(st->a);
    free(st);
}

ST STsort(ST st, int maxN)
{
    int i, j, l=0, r=maxN-1;
    Item temp;
    for(i=l;i<r;i++)
    {
        for (j=l;j<r-i+l;j++)
            if(strcmp(st->a[j].id_elab,st->a[j+1].id_elab)>0)
            {
                temp = st->a[j];
                st->a[j] = st->a[j+1];
                st->a[j+1] = temp;
            }
    }
    return st;
}

int getN(ST st)
{
    return st->N;
}

void STprint(ST st, ST tabOrd, int N, int **madj)
{
    int *v = (int*)malloc(N*sizeof(int));
    int k, h;
    Item it;
    
    for(int i=0;i<N;i++)
    {
        k=0;
        printf("---%s---\n", tabOrd->a[i].id_elab);
        h = STsearch(st, tabOrd->a[i]);
        for(int j=0;j<N;j++)
        {
            if(madj[h][j]!=0)
                v[k++] = j;
        }
        sortVertice(v, k, st);
        for(int j=0;j<k;j++)
        {
            it = ITEMsearch(st, v[j]);
            printf("%s ", it.id_elab);
        }
        printf("\n\n");
    }
}

void sortVertice(int *v, int k, ST st)
{
    int i, j, l=0, r=k-1, temp;
    Item i1, i2;
    
    for(i=l;i<r;i++)
    {
        for (j=l;j<=r-i+l;j++)
        {
            i1 = ITEMsearch(st, v[j]);
            i2 = ITEMsearch(st, v[j+1]);
            if(strcmp(i1.id_elab,i2.id_elab)>0)
            {
                temp = v[j];
                v[j] = v[j+1];
                v[j+1] = temp;
            }
        }
    }
}

ST STcopy(ST tab, int V)
{
    ST stOrd = STinit(V);
    
    for(int i=0;i<V;i++)
        stOrd->a[i] = tab->a[i];
    stOrd->M = tab->M;
    stOrd->N = tab->N;
    return stOrd;
}
