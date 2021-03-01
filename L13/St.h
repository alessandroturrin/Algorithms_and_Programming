#ifndef St_h
#define St_h

#define maxChar 30

typedef struct symboltable *ST;
typedef char *Item;

Item ITEMsearch(ST st, int index);
ST STinit(int maxN);
void STinsert(ST st, Item val);
int STsearch(ST st, Item val);
void STfree(ST st);
int STsearchItem(ST st, Item val);
int STgetN(ST st);

Item ITEMsetVoid();
#endif /* St_h */
