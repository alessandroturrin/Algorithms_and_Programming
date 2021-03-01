#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "pgList.h"

typedef struct nodoPg_t *link;

struct nodoPg_t
{
    pg_t personaggio;
    link next;
};

struct pgList_s
{
    link headPg;
    link tailPg;
    int nPg;
};

pgList_t pgList_init()
{
    pgList_t t;
    t=malloc(sizeof(pgList_t));
    t->headPg=t->tailPg=NULL;
    return t;
}

void pgList_free(pgList_t pgList)
{
    link t, x, h=pgList->headPg;
    x=h;
    while(x!=NULL){
        t=x;
        x=x->next;
        pg_clean(&(t->personaggio));
    }
    //free(pgList);
}

void pgList_read(FILE *fp, pgList_t pgList)
{
    pg_t pg;

    if(fp==NULL)
    {
        printf("Errore nell'apertura del file\n");
        exit(EXIT_FAILURE);
    }
    while(!feof(fp)){
        if(pg_read(fp, &pg)){
            pgList_insert(pgList, pg);
        }
    }
}

void pgList_print(FILE *fp, pgList_t pgList, invArray_t invArray)
{
    link h=pgList->headPg, x;
    for(x=h;x!=NULL;x=x->next){
        pg_print(fp, &x->personaggio, invArray);
    }
}

link newNode(pg_t val, link next)
{
    link x=malloc(sizeof *x);
    if(x==NULL) //controllo che la malloc vada a buon fine
        return NULL;
    x->personaggio=val;
    x->next=next;
    return x;
}

/* inserimento di un nuovo personaggio */
void pgList_insert(pgList_t pgList, pg_t pg)
{
    if(pgList->headPg==NULL)
    {
        pgList->headPg=pgList->tailPg=newNode(pg, NULL);
        pgList->headPg->personaggio.equip=equipArray_init();
        pgList->nPg=1;
        return;
    }
    pgList->tailPg->next=newNode(pg, NULL);
    pgList->tailPg=pgList->tailPg->next;
    pgList->tailPg->personaggio.equip=equipArray_init();
    pgList->nPg++;
}

/* cancellazione con rimozione */
void pgList_remove(pgList_t pgList, char* cod)
{
    link x, p, h=pgList->headPg;
    pg_t *ppg=pgList_searchByCode(pgList, cod);
    if(ppg==&pgList->headPg->personaggio)
    {
        x=pgList->headPg;
        pgList->headPg=pgList->headPg->next;
        equipArray_free(ppg->equip);
        pg_clean(ppg);
        free(x);
        return;
    }
    for(x=h->next,p=h;x!=NULL;p=x,x=x->next)
    {
        if(&x->personaggio==ppg){
            if(x->next==NULL)
                pgList->tailPg=p;
            p->next=x->next;
            equipArray_free(ppg->equip);
            pg_clean(ppg);
            free(x);
            return;
        }
    }
}

/* ricerca per codice, ritornando il puntatore */
pg_t *pgList_searchByCode(pgList_t pgList, char* cod)
{
    link x, h=pgList->headPg;
    for(x=h; x!=NULL;x=x->next)
        if(strcmp(cod, x->personaggio.cod)==0)
            return &x->personaggio;
    return NULL;
}
