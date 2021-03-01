#ifndef quotazioni_h
#define quotazioni_h

#include "item.h"

typedef struct
{
    Data data;
    float valoreQuotazione, valoreTot;
    int nTot, transTot;
}Quotazione;

typedef struct
{
    float min, max;
}QuotazioneMinMax;

typedef enum {range, full} range_t;
typedef enum {false, true} bool;

typedef struct tree *BST;


BST BSTinit();
void BSTinsert(BST bst, Item *item, int oN, int nN);
void BSTfree(BST bst);
float BSTsearch(BST bst, Data d);
QuotazioneMinMax BSTminMax(BST bst, Data dMin, Data dMax, range_t check);
bool BSTminMaxLength(BST bst, int soglia);
void BSTbalance(BST bst);

void QUOTAZIONEprint(Quotazione q);
Quotazione QUOTAZIONEsetNull();
int QMMdidChange(QuotazioneMinMax q);

QuotazioneMinMax QMMset();
#endif /* quotazioni_h */
