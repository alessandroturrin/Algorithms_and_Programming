#ifndef titoli_h
#define titoli_h

#include "quotazioni.h"

typedef struct lista *List;

List LISTinit();
void LISTfread(FILE *fp, List list);
void LISTshow(List l);
void LISTfree(List L);

void ricercaTitoloAzionario(List L);
void ricercaData(List L);
void ricercaMinMax(List L, range_t r);
void bilanciaBST(List L);
#endif
