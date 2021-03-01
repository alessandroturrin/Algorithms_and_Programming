#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quotazioni.h"

typedef struct BSTnode *BSTlink;

struct BSTnode
{
    Quotazione val;
    BSTlink l, r;
};

struct tree
{
    BSTlink root, z;
};


static BSTlink new(Quotazione q, BSTlink l, BSTlink r)
{
    BSTlink x = malloc(sizeof(*x));
    x->val = q;
    x->l = l;
    x->r = r;
    return x;
}

static int QUOTAZIONEcompare(Quotazione q1, Quotazione q2)
{
    if(q1.data.yyyy>q2.data.yyyy)
        if(q1.data.mm>q2.data.mm)
            if(q1.data.dd>q2.data.dd)
                return 1;
            else
                return -1;
        else
            return -1;
    else
        return -1;
}

static void treeFree(BSTlink h, BSTlink z)
{
    if(h==z)
        return;
    treeFree(h->l, z);
    treeFree(h->r, z);
    free(h);
}

static BSTlink insertR(BSTlink h, Quotazione q, BSTlink z)
{
    if(h==z)
        return new(q, z, z);
    
    if(DATAcompare(q.data, h->val.data)<0)
        h->l = insertR(h->l, q, z);
    else
        h->r = insertR(h->r, q, z);
    return h;
}

static void BSTminMaxR(QuotazioneMinMax *q, BSTlink h, BSTlink z, Data dMin, Data dMax, range_t check)
{
    if(h==z)
        return;
    
    BSTminMaxR(q, h->l, z, dMin, dMax, check);
    if(h->val.valoreQuotazione>(*q).max)
    {
        if(check==range)
            if(DATAcompare(h->val.data, dMin)>=0 && DATAcompare(h->val.data, dMax)<=0)
                (*q).max = h->val.valoreQuotazione;
        if(check==full)
            (*q).max = h->val.valoreQuotazione;
    }
    
    if(h->val.valoreQuotazione<(*q).min)
    {
        if(check==range)
            if(DATAcompare(h->val.data, dMin)>=0 && DATAcompare(h->val.data, dMax)<=0)
                (*q).min = h->val.valoreQuotazione;
        if(check==full)
            (*q).min = h->val.valoreQuotazione;
    }
    BSTminMaxR(q, h->r, z, dMin, dMax, check);
}

static BSTlink BSTsearchR(BSTlink h, BSTlink z, Data d)
{
    int dCmp;
    
    if(h==z)
        return NULL;
    
    dCmp = DATAcompare(d, h->val.data);
    if(dCmp==0)
        return h;
    if(dCmp<0)
        return BSTsearchR(h->l, z, d);
    return BSTsearchR(h->r, z, d);
}

static int max(int n1, int n2)
{
    if(n1>n2)
        return n1;
    else
        return n2;
}

static int min(int n1, int n2)
{
    if(n1<n2)
        return n1;
    else
        return n2;
}

static int BSTmaxLength(BSTlink h, BSTlink z)
{
    if(h==z)
        return 0;
    else return 1 + max(BSTmaxLength(h->l, z), BSTmaxLength(h->r, z));
}

static int BSTminLength(BSTlink h, BSTlink z)
{
    if(h==z)
        return 0;
    else return 1 + min(BSTminLength(h->l, z), BSTminLength(h->r, z));
}

static int BSTcount(BSTlink h, BSTlink z)
{
    if(h==z)
        return 0;
    return (BSTcount(h->l, z)+BSTcount(h->r, z)+1);
}


static BSTlink rotL(BSTlink h)
{
    BSTlink x = h->r;
    h->r = x->l;
    x->l = h;
    return x;
}

static BSTlink rotR(BSTlink h)
{
    BSTlink x = h->l;
    h->l = x->r;
    x->r = h;
    return x;
}

static BSTlink partR(BSTlink h, BSTlink z, int r)
{
    int N;
    N = BSTcount(h->l, z);
    int t = N;
    if (t > r)
    {
        h->l = partR(h->l, z, r);
        h = rotR(h);
    }
    if (t < r)
    {
        h->r = partR(h->r, z, r-t-1); h = rotL(h);
    }
    return h;
}

static BSTlink balanceR(BSTlink h, BSTlink z)
{
    int r, N;
    if (h==z)
        return z;
    N = BSTcount(h, z);
    r = (N+1)/2-1;
    h = partR(h, z, r);
    h->l = balanceR(h->l, z);
    h->r = balanceR(h->r, z);
    return h;
}

BST BSTinit()
{
    BST bst = malloc(sizeof(*bst));
    bst->root = (bst->z = new(QUOTAZIONEsetNull(),NULL,NULL));
    return bst;
}

void BSTinsert(BST bst, Item *item, int oN, int nN)
{
    Quotazione q;
    int transTot;
    float valoreTot = 0.0;
    BSTlink x;
    
    for(int i=oN;i<nN;i++) //forse <=
    {
        valoreTot = 0.0;
        q = QUOTAZIONEsetNull();
        q.data = ITEMgetData(item[i]);
        x = BSTsearchR(bst->root, bst->z, q.data);
        while (i<nN && (DATAcompare(q.data, ITEMgetData(item[i]))==0))
        {
            q.transTot+=ITEMgetNumero(item[i]);
            q.valoreTot+=ITEMgetValore(item[i])*ITEMgetNumero(item[i]);
            i++;
        }
        i--;
        q.valoreQuotazione = q.valoreTot/q.transTot;
        if(x==NULL)
            bst->root = insertR(bst->root, q, bst->z);
        else
            x->val.valoreQuotazione = ((x->val.valoreQuotazione + q.valoreQuotazione)*(x->val.nTot+q.nTot))/(x->val.transTot+q.transTot);
    }
}

float BSTsearch(BST bst, Data d)
{
    BSTlink x = BSTsearchR(bst->root, bst->z, d);
    if(x==NULL)
        return -1.0;
    return x->val.valoreQuotazione;
}

void BSTfree(BST bst)
{
    if(bst==NULL)
        return;
    treeFree(bst->root, bst->z);
    free(bst->z);
    free(bst);
}

QuotazioneMinMax BSTminMax(BST bst, Data dMin, Data dMax, range_t check)
{
    QuotazioneMinMax q = QMMset();
    
    BSTminMaxR(&q, bst->root, bst->z, dMin, dMax, check);
    return q;
}

bool BSTminMaxLength(BST bst, int soglia)
{
    if((BSTmaxLength(bst->root, bst->z)-BSTminLength(bst->root, bst->z))>soglia)
        return true;
    return false;
}

void BSTbalance(BST bst)
{
    bst->root = balanceR(bst->root, bst->z);
}
    
void QUOTAZIONEprint(Quotazione q)
{
    printf("%d/%d/%d %d:%d", q.data.yyyy, q.data.mm, q.data.dd, q.data.h, q.data.m);
    printf("Valore quotazione: %.2f\n", q.valoreQuotazione);
}

Quotazione QUOTAZIONEsetNull()
{
    Quotazione q;
    q.data.yyyy = q.data.mm = q.data.dd = 0;
    q.valoreQuotazione = 0.0;
    q.transTot = 0;
    q.nTot = 0;
    q.valoreTot = 0.0;
    return q;
}

QuotazioneMinMax QMMset()
{
    QuotazioneMinMax q;
    q.min = 9999.0;
    q.max = 0.0;
    return q;
}

int QMMdidChange(QuotazioneMinMax q)
{
    if(q.min==9999.0 && q.max==0.0)
        return 0;
    return 1;
}
