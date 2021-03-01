#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "item.h"

struct item
{
    Data data;
    float valore;
    int numero;
};

Item *ITEMfirstScan(FILE *fp, int N)
{
    Item *item;
    item = malloc(N*sizeof(*item));
    for(int i=0;i<N;i++)
    {
        item[i] = malloc(sizeof(struct item));
        fscanf(fp, "%d/%d/%d %d:%d %f %d", &item[i]->data.yyyy, &item[i]->data.mm, &item[i]->data.dd, &item[i]->data.h, &item[i]->data.m, &item[i]->valore, &item[i]->numero);
    }
    return item;
}

int ITEMscan(FILE *fp, int oN, int nN, Item **item)
{
    *item = realloc(*item, (oN+nN)*sizeof(*item));
    
    for(int i=oN;i<(nN+oN);i++)
    {
        (*item)[i] = malloc(sizeof(struct item));
        fscanf(fp, "%d/%d/%d %d:%d %f %d", &(*item)[i]->data.yyyy, &(*item)[i]->data.mm, &(*item)[i]->data.dd, &(*item)[i]->data.h, &(*item)[i]->data.m, &(*item)[i]->valore, &(*item)[i]->numero);
    }
    return nN+oN;
}

void ITEMshow(Item *item, int N)
{
    for(int i=0;i<N;i++)
        printf("%d/%d/%d %d:%d %.2f %d\n", item[i]->data.yyyy, item[i]->data.mm, item[i]->data.dd, item[i]->data.h, item[i]->data.m, item[i]->valore, item[i]->numero);
}

Data ITEMgetData(Item item)
{
    return item->data;
}

float ITEMgetValore(Item item)
{
    return item->valore;
}

int ITEMgetNumero(Item item)
{
    return item->numero;
}

void ITEMfree(Item *item, int dim)
{
    for(int i=0;i<dim;i++)
    {
        free(item[i]);
    }
    free(item);
}

int DATAcompare(Data q1, Data q2)
{
    if(q1.yyyy==q2.yyyy)
        if(q1.mm==q2.mm)
            if(q1.dd==q2.dd)
                return 0;
            else
                return q1.dd-q2.dd;
        else
            return q1.mm-q2.mm;
    else
        return q1.yyyy-q2.yyyy;
}

Data DATAinput()
{
    Data d;
    printf("Inserire data [yyyy/mm/dd] : ");
    scanf("%d/%d/%d", &d.yyyy, &d.mm, &d.dd);
    return d;
}

void DATAprint(Data d)
{
    printf("%d/%d/%d", d.yyyy, d.mm, d.dd);
}

Data DATAsetVoid()
{
    Data d;
    d.yyyy = d.mm = d.dd = d.h = d.m = 0;
    return d;
}
