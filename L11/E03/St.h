#ifndef St_h
#define St_h

#include "Item.h"

typedef struct symboltable *ST;

Item ITEMsearch(ST st, int index);
ST STinit(int maxN);
void STinsert(ST st, Item val);
int STsearch(ST st, Item val);
void STfree(ST st);
ST STsort(ST st, int maxN);
int getN(ST st);
void STprint(ST st, ST tabOrd, int V, int **madj);
void sortVertice(int *v, int N, ST st);
ST STcopy(ST tab, int V);
#endif /* St_h */
