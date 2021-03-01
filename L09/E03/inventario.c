#include "inventario.h"

void inserimentoInventarioFile(tabInv_t **tabInv)
{
    FILE* file;
    char fileINV[maxLength]; // "/Users/Ale/Desktop/inventario.txt"
    tabInv_t *t; //variabile ausiliaria
    char nome[maxLength], tipo[maxLength];
    
    t = (tabInv_t *)malloc(sizeof(tabInv_t));
    printf("Inserire il nome del file di input: ");
    scanf("%s",fileINV);
    if((file=fopen(fileINV, "rt"))==NULL)
    {
        printf("\nErrore nell'apertura del file!\n\n");
        return;
    }
    fscanf(file, "%d", &t->nInv);
    t->vettInv = (inv_t**)malloc((t->nInv)*sizeof(inv_t*));
    for(int i=0;i<t->nInv;i++)
    {   //input
        fscanf(file, "%s", nome);
        fscanf(file, "%s", tipo);
        //allocazione
        t->vettInv[i] = (inv_t*)malloc(sizeof(inv_t));
        t->vettInv[i]->nome = (char *)malloc((strlen(nome)+1)*sizeof(char));
        t->vettInv[i]->tipo = (char *)malloc((strlen(tipo))+1*sizeof(char));
        //copia
        strcpy(t->vettInv[i]->nome, nome);
        strcpy(t->vettInv[i]->tipo, tipo);
        //input oggetti
        fscanf(file, "%d %d %d %d %d %d", &t->vettInv[i]->stat.hp, &t->vettInv[i]->stat.mp, &t->vettInv[i]->stat.atk, &t->vettInv[i]->stat.def, &t->vettInv[i]->stat.mag, &t->vettInv[i]->stat.spr);
    }
    *tabInv = t;
    fclose(file);
}

int sceltaEquipaggiamento()
{
    char scelta[maxLength];
    
    printf("Aggiungere o rimuovere un equipaggiamento? [aggiungere,rimuovere]\nOpzione: ");
    scanf("%s", scelta);
    if((strcmp(scelta, "aggiungere"))==0) return 1;
    if((strcmp(scelta, "rimuovere"))==0) return 2;
    return -1;
}


inv_t *ricercaEquipaggiamento(tabInv_t* tabInv, char *equipaggiamento)
{
    for(int i=0;i<tabInv->nInv;i++)
        if(strcmp(equipaggiamento,tabInv->vettInv[i]->nome)==0) return tabInv->vettInv[i];
    return NULL;
}

void stampaEquipaggiamento(tabInv_t* tabInv)
{
    char nome[maxLength];
    
    printf("Nome dell'oggetto da stampare: ");
    scanf("%s", nome);
    
    for(int i=0;i<tabInv->nInv;i++)
        if(strcmp(tabInv->vettInv[i]->nome, nome)==0)
        {
            printf("%s %s %d %d %d %d %d %d\n\n", tabInv->vettInv[i]->nome, tabInv->vettInv[i]->tipo, tabInv->vettInv[i]->stat.hp, tabInv->vettInv[i]->stat.mp, tabInv->vettInv[i]->stat.atk, tabInv->vettInv[i]->stat.def, tabInv->vettInv[i]->stat.mag, tabInv->vettInv[i]->stat.spr);
            return;
        }
    printf("Oggetto non trovato!\n\n");
}
