#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXC 50

#include "invArray.h"

struct invArray_s{
    inv_t *vettInv;
    int nInv;
};

invArray_t invArray_init()
{
    invArray_t t;
    t=malloc(sizeof(invArray_t));
    return t;
}

void invArray_free(invArray_t invArray)
{
    for(int i=0;i<invArray->nInv;i++)
    {
        free(invArray->vettInv[i].nome);
        free(invArray->vettInv[i].tipo);
    }
    free(invArray->vettInv);
    free(invArray);
}

void invArray_read(FILE *fp, invArray_t invArray)
{
    fscanf(fp, "%d", &invArray->nInv);
    invArray->vettInv=(inv_t*)malloc((invArray->nInv)*sizeof(inv_t));
    for(int i=0;i<invArray->nInv;i++)
    {
        inv_read(fp, &invArray->vettInv[i]);
    }
}

void invArray_print(FILE *fp, invArray_t invArray)
{
    for(int i=0;i<invArray->nInv;i++)
        inv_print(fp, &invArray->vettInv[i]);
}

/* stampa un unico oggetto selezionato da indice (nel vettore) */
void invArray_printByIndex(FILE *fp, invArray_t invArray, int index)
{
    inv_print(fp, &invArray->vettInv[index]);
}

/* ritorna puntatore a oggetto selezionato da indice (nel vettore) */
inv_t *invArray_getByIndex(invArray_t invArray, int index)
{
    return &(invArray->vettInv[index]);
}

/* ritorna indice (nel vettore) a oggetto selezionato da nome */
int invArray_searchByName(invArray_t invArray, char *name){
    for(int i=0;i<invArray->nInv;i++)
    {
        if(strcmp(invArray->vettInv[i].nome, name)==0)
            return i;
    }
    return -1;
}
