#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "equipArray.h"

struct equipArray_s{
    int inUso;
    int *vettEq;
};

equipArray_t equipArray_init()
{
    equipArray_t t;
    t=malloc(sizeof(equipArray_t));
    t->inUso=0;
    t->vettEq=malloc(EQUIP_SLOT*sizeof(int));
    for(int i=0;i<EQUIP_SLOT;i++)
        t->vettEq[i]=-1;
    return t;
}

void equipArray_free(equipArray_t equipArray)
{
    free(equipArray->vettEq);
}

int equipArray_inUse(equipArray_t equipArray)
{
    return equipArray->inUso;
}

void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray)
{
    for(int i=0;i<equipArray_inUse(equipArray);i++)
    {
        invArray_printByIndex(fp, invArray, i);
    }
}

int equipArray_update(equipArray_t equipArray, invArray_t invArray)
{
    int scelta, i;
    char nome[LEN];
    printf("1 per aggiungere, 2 per rimuovere: ");
    scanf("%d", &scelta);
    printf("Inserire nome dell'equipaggiamento: "); scanf("%s", nome);
    if(scelta==1 && equipArray->inUso<=EQUIP_SLOT)
    {
        if((i=invArray_searchByName(invArray, nome))>=0)
        {
            equipArray->vettEq[equipArray_inUse(equipArray)]=i;
            equipArray->inUso++;
            return i;
        }
        else printf("Equipaggiamento non disponibile\n");
        return -1;
    }
    if((i=invArray_searchByName(invArray, nome))>=0)
    {
        equipArray->vettEq[i]=equipArray->vettEq[equipArray->inUso--];
        equipArray->vettEq[equipArray_inUse(equipArray)]=-1;
        return (i+100);
    }
    printf("Equipaggiamento non presente\n");
    return -1;
}

int equipArray_getEquipByIndex(equipArray_t equipArray, int index)
{
    return equipArray->vettEq[index];
}

