#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "titoli.h"

typedef struct node *link;

struct node
{
    char nomeTitolo[maxChar];
    Item *val;
    int dimVal;
    BST bst;
    link next;
};

struct lista
{
    link head;
    int N;
};

static link newNode(char *nomeTitolo, link next, int nTrans, FILE *fp)
{
    link x = malloc(sizeof(*x));
    x->next = next;
    x->val = ITEMfirstScan(fp, nTrans);
    x->dimVal = nTrans;
    x->bst = BSTinit();
    strcpy(x->nomeTitolo, nomeTitolo);
    
    return x;
}

static link SortInsList(link h, char *nomeTitolo, int nTrans, FILE *fp)
{
    link x, p;
    
    if(h==NULL || strcmp(h->nomeTitolo, nomeTitolo)>0)
        return newNode(nomeTitolo, h, nTrans, fp);
    for(x=h->next, p=h;x!=NULL && strcmp(x->nomeTitolo, nomeTitolo)<0;p=x, x=x->next);
    p->next = newNode(nomeTitolo, x, nTrans, fp);
    return h;
}

static link SortListSearch(link h, char nomeTitolo[])
{
    link x;
    for(x=h; x!=NULL && (strcmp(nomeTitolo, x->nomeTitolo))>=0;x=x->next)
        if((strcmp(nomeTitolo, x->nomeTitolo))==0)
            return x;
    return NULL;
}

List LISTinit()
{
    List l = malloc(sizeof(*l));
    l->head = NULL;
    l->N = 0;
    return l;
}

void LISTfread(FILE *fp, List list)
{
    int nTitoli, nTransazioni;
    char nomeTitolo[maxChar];
    link x;
    
    if(fp==NULL)
    {
        printf("Errore nell'apertura del file!\n");
        exit(1);
    }
    
    fscanf(fp, "%d", &nTitoli);
    for(int i=0;i<nTitoli;i++)
    {
        fscanf(fp, "%s %d", nomeTitolo, &nTransazioni);
        x = SortListSearch(list->head, nomeTitolo);
        if(x==NULL)
        {
            list->head = SortInsList(list->head, nomeTitolo, nTransazioni, fp);
            x = SortListSearch(list->head, nomeTitolo);
        }
        else
        {
            x->dimVal = ITEMscan(fp, x->dimVal, nTransazioni, &(x->val));
        }
        BSTinsert(x->bst, x->val, x->dimVal-nTransazioni, x->dimVal);
    }
    fclose(fp);
}

void LISTshow(List l)
{
    link x;
    
    for(x=l->head; x!=NULL; x=x->next)
    {
        printf("%s \n", x->nomeTitolo);
        ITEMshow(x->val, x->dimVal);
        printf("\n");
    }
}

void LISTfree(List L)
{
    link x, t;
    for(x=L->head;x!=NULL;x=x->next)
    {
        t = x->next;
        BSTfree(x->bst);
        ITEMfree(x->val, x->dimVal);
    }
    free(L);
}

void ricercaTitoloAzionario(List L)
{
    link x;
    char nomeTitolo[maxChar];
    
    printf("Inserire il codice del titolo azionario: ");
    scanf("%s", nomeTitolo);
    x = SortListSearch(L->head, nomeTitolo);
    if(x==NULL)
    {
        printf("Titolo azionario non trovato!\n");
        return;
    }
    printf("%s\n", nomeTitolo);
    ITEMshow(x->val, x->dimVal);
}

void ricercaData(List L)
{
    link x;
    char nomeTitolo[maxChar];
    float q;
    Data d;
    
    printf("Inserire il codice del titolo azionario: ");
    scanf("%s", nomeTitolo);
    x = SortListSearch(L->head, nomeTitolo);
    if(x==NULL)
    {
        printf("Titolo azionario non trovato!\n");
        return;
    }
    
    d = DATAinput();
    q = BSTsearch(x->bst, d);
    
    if(q!=-1)
    {
        printf("%s\t[", x->nomeTitolo);
        DATAprint(d);
        printf("]\nValore quotazione: %.2f\n\n", q);
    }
    else
        printf("Nessun valore per la data corrispondente!\n\n");
}

void ricercaMinMax(List L, range_t r)
{
    link x;
    char nomeTitolo[maxChar];
    Data dMin, dMax;
    QuotazioneMinMax q;
    
    printf("Inserire il codice del titolo azionario: ");
    scanf("%s", nomeTitolo);
    x = SortListSearch(L->head, nomeTitolo);
    if(x==NULL)
    {
        printf("Titolo azionario non trovato!\n");
        return;
    }
    
    if(r==full)
    {
        dMin = DATAsetVoid();
        dMax = DATAsetVoid();
        q = BSTminMax(x->bst, dMin, dMax, full);
    }
    if(r==range)
    {
        printf("---DATA MINIMA---\n");
        dMin = DATAinput();
        printf("---DATA MASSIMA---\n");
        dMax = DATAinput();
        q = BSTminMax(x->bst, dMin, dMax, range);
    }
    
    if(QMMdidChange(q))
        printf("Quotazione minima: %.2f\nQuotazione massima: %.2f\n\n", q.min, q.max);
    else
        printf("Nessun valore rilevato!\n\n");
}

void bilanciaBST(List L)
{
    int soglia;
    link x;
    char nomeTitolo[maxChar];
    
    printf("Inserire il codice del titolo azionario: ");
    scanf("%s", nomeTitolo);
    x = SortListSearch(L->head, nomeTitolo);
    if(x==NULL)
    {
        printf("Titolo azionario non trovato!\n");
        return;
    }
    printf("Inserire la soglia per il bilanciamento: ");
    scanf("%d", &soglia);
    
    if(BSTminMaxLength(x->bst, soglia))
    {
        BSTbalance(x->bst);
        printf("Bilanciamento avvenuto con successo!\n\n");
    }
    else
        printf("Bilanciamento non necessario, soglia massima non superata!\n\n");
}
