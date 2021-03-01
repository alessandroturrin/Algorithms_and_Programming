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
        return ITEMsetVoid();
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
    if(index<0)
    {
        st->a[st->N++] = strdup(val);
    }
}

int STsearch(ST st, Item val)
{
    for(int i=0;i<st->N;i++)
    {
        if(strcmp(st->a[i],val)==0)
            return i;
    }
    return -1;
}

void STfree(ST st)
{
    free(st->a);
    free(st);
}

int STgetN(ST st)
{
    return st->N;
}

int STsearchItem(ST st, Item val)
{
    return (STsearch(st, val));
}

Item ITEMsetVoid()
{
    return "\0";
}
