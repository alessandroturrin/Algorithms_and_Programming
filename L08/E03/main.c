#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define maxLength 52
#define dimEquip 8
//struct statistiche
typedef struct
{
    int hp, mp, atk, def, mag, spr;
}stat_t;
//struct inventario
typedef struct
{
    char *nome;
    char *tipo;
    stat_t stat;
}inv_t;
//struct equipaggiamento
typedef struct
{
    int inUso;
    inv_t *vettEq[dimEquip];
}tabEquip_t;
//struct giocatore
typedef struct
{
    char *codice;
    char *nome;
    char *classe;
    tabEquip_t equip;
    stat_t stat;
}pg_t;
//struct tabella inventario
typedef struct
{
    int nInv;
    inv_t **vettInv;
}tabInv_t;
//lista
typedef struct nodo *link_pg;
typedef struct nodo
{
    pg_t val;
    link_pg next;
}nodoPg_t;
//struct tabella giocatori
typedef struct
{
    link_pg head;
    link_pg tail;
    int nPg;
}tabPg_t;

typedef enum {inserire_personaggi,inserire_oggetti,aggiungere_personaggio,eliminare_personaggio,modificare_equipaggiamento,calcolare_statistiche,fine,errore} comando_c;
typedef enum {false,true} bool;

//funzioni e procedure
comando_c selezionaComando(void);
void inserimentoPersonaggiFile(tabPg_t **tabPg);
void inserimentoInventarioFile(tabInv_t **tabInv);
void listInsHead(tabPg_t** tabPg, pg_t val);
pg_t creaPersonaggio(void);
void rimozionePersonaggio(tabPg_t** tabPg);
int sceltaEquipaggiamento(void);
void freeLinkPg(link_pg x);
void aggiuntaEquipaggiamento(tabPg_t* tabPg, tabInv_t* tabInv);
void rimozioneEquipaggiamento(tabPg_t* tabPg, tabInv_t* tabInv);
inv_t *ricercaEquipaggiamento(tabInv_t* tabInv, char *equipaggiamento);
void stampa(tabPg_t* tabPg);
int main()
{
    tabPg_t *tabPg;
    tabInv_t *tabInv;
    pg_t pg; //personaggio
    bool esegui = true;
    comando_c comando;
    int scelta;
    
    //allocazione di tabPg e inizializzazione di head a NULL
    tabPg = (tabPg_t*)malloc(sizeof(*tabPg));
    tabPg->head = NULL;
    
    while(esegui)
    {
        comando = selezionaComando();
        switch (comando)
        {
            case inserire_personaggi:
                inserimentoPersonaggiFile(&tabPg);
                break;
            case inserire_oggetti:
                inserimentoInventarioFile(&tabInv);
                break;
            case aggiungere_personaggio:
                pg = creaPersonaggio();
                listInsHead(&tabPg, pg);
                break;
            case eliminare_personaggio:
                rimozionePersonaggio(&tabPg);
                break;
            case modificare_equipaggiamento:
                scelta = sceltaEquipaggiamento();
                if(scelta==1) aggiuntaEquipaggiamento(tabPg, tabInv);
                if(scelta==2) rimozioneEquipaggiamento(tabPg, tabInv);
                if(scelta==-1) printf("\nErrore nell'inserimento del comando!\n\n");
                break;
            case calcolare_statistiche:
                stampa(tabPg);
                break;
            case fine:
                printf("\nFine dell'esecuzione del programma!\n\n");
                esegui = false;
                break;
            case errore:
                printf("\nErrore nell'esecuzione del programma!\n\n");
                exit(1);
                break;
        }
    }
}

comando_c selezionaComando()
{
    char *tab[] = {"inserire_personaggi","inserire_oggetti","aggiungere_personaggio","eliminare_personaggio","modificare_equipaggiamento","calcolare_statistiche","fine"}; //tabella comandi
    char inputComando[maxLength];
    int i;
    
    printf("\n---Comandi disponibili---\n");
    for(i=0;i<errore;i++) printf("%s\n", tab[i]);
    printf("\n\nComando: ");
    scanf("%s", inputComando);
    for(i=inserire_personaggi;i<errore;i++)
        if((strcmp(tab[i], inputComando))==0) return i;
    return i; //i=errore
}

void inserimentoPersonaggiFile(tabPg_t **tabPg)
{
    FILE* file;
    pg_t p;
    char filePG[maxLength];// "/Users/Ale/Desktop/pg.txt"
    char codice[maxLength], nome[maxLength], classe[maxLength];
    
    printf("Inserire il nome del file di input: ");
    scanf("%s",filePG);
    if((file=fopen(filePG, "rt"))==NULL)
    {
        printf("\nErrore nell'apertura del file!\n\n");
        return;
    }
    while(fscanf(file, "%s %s %s %d %d %d %d %d %d", codice, nome, classe, &p.stat.hp, &p.stat.mp, &p.stat.atk, &p.stat.def, &p.stat.mag, &p.stat.spr)!=EOF)
    {
        p.codice=strdup(codice);
        p.nome=strdup(nome);
        p.classe=strdup(classe);
        p.equip.inUso=0;
        listInsHead(tabPg, p);
    }
    fclose(file);
}

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

void listInsHead(tabPg_t** tabPg, pg_t val)
{
    tabPg_t *t;
    nodoPg_t *x;
    
    t = *tabPg;
    x = malloc(sizeof(nodoPg_t));
    x->val=val;
    if(t->head==NULL) //primo input
    {
        x->next = NULL;
        t->head = x;
        t->tail = x;
        t->nPg = 1;
    }
    else{
        x->next = t->head;
        t->head = x;
        t->nPg++;
    }
    *tabPg = t;
}

pg_t creaPersonaggio()
{
    pg_t p;
    char codice[maxLength], nome[maxLength], classe[maxLength];
    //effettuo input
    printf("Codice: ");
    scanf("%s",codice);
    printf("Nome: ");
    scanf("%s",nome);
    printf("Classe: ");
    scanf("%s",classe);
    printf("Hp: ");
    scanf("%d",&p.stat.hp);
    printf("Mp: ");
    scanf("%d",&p.stat.mp);
    printf("Atk: ");
    scanf("%d",&p.stat.atk);
    printf("Def: ");
    scanf("%d",&p.stat.def);
    printf("Mag: ");
    scanf("%d",&p.stat.mag);
    printf("Spr: ");
    scanf("%d%*c",&p.stat.spr);
    //allocazione
    p.codice=(char *)malloc((strlen(codice)+1)*sizeof(char));
    p.nome=(char *)malloc((strlen(nome)+1)*sizeof(char));
    p.classe=(char *)malloc((strlen(classe)+1)*sizeof(char));
    //copia
    strcpy(p.codice,codice);
    strcpy(p.nome,nome);
    strcpy(p.classe,classe);
    p.equip.inUso=0;

    return p;
}

void rimozionePersonaggio(tabPg_t** tabPg)
{
    nodoPg_t *x,*p;
    tabPg_t *t = *tabPg;
    char nome[maxLength];
    //input nome eliminare
    printf("Nome del personaggio da eliminare: ");
    scanf("%s", nome);
    //caso lista vuota
    if(t->head==NULL) printf("\nLa lista non contiene alcun elemento!\n\n");
    //caso elemento in head
    if(strcmp(nome,t->head->val.nome)==0){
        x = t->head;
        t->head = t->head->next;
        freeLinkPg(x);
    }
    //ricerca in lista
    else
    {
        for(x=t->head, p=NULL;x->next!=NULL && (strcmp(nome,x->val.nome)!=0);p=x,x=x->next);
        if(x->next==NULL && strcmp(nome,x->val.nome)==0)
        {
            t->tail = p;
            p->next = NULL;
            freeLinkPg(x);
        }
        else
        {
            p->next = x->next;
            freeLinkPg(x);
        }
    }
    *tabPg = t;
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

void freeLinkPg(link_pg x)
{
    free(x->val.codice);
    free(x->val.nome);
    free(x->val.classe);
    free(x);
}

void aggiuntaEquipaggiamento(tabPg_t* tabPg, tabInv_t* tabInv)
{
    nodoPg_t *x;
    inv_t *equipaggiamento;
    char nome[maxLength], equip[maxLength];
    
    printf("Nome del personaggio: ");
    scanf("%s", nome);
    printf("Equipaggiamento da aggiungere: ");
    scanf("%s", equip);
    
    for(x = tabPg->head; x->next!=NULL && (strcmp(nome,x->val.nome)!=0); x=x->next);
    if(x->next==NULL && (strcmp(nome,x->val.nome)!=0)) printf("\nPersonaggio non presente nella lista!\n\n");
    if(x->val.equip.inUso==dimEquip) printf("\nNumero massimo raggiunto!\n\n");

    equipaggiamento = ricercaEquipaggiamento(tabInv, equip);
    if(equipaggiamento==NULL) printf("\nEquipaggiamento non presente nell'elenco!\n\n");
    else
    {
        x->val.equip.vettEq[x->val.equip.inUso] = equipaggiamento;
        x->val.equip.inUso = x->val.equip.inUso+1;
    }
}

void rimozioneEquipaggiamento(tabPg_t* tabPg, tabInv_t* tabInv)
{
    nodoPg_t *x;
    tabPg_t *t;
    int i;
    
    char nome[maxLength], equip[maxLength];
    
    printf("Nome del personaggio: ");
    scanf("%s", nome);
    printf("Equipaggiamento da rimuovere: ");
    scanf("%s", equip);
    
    t = tabPg;
    //caso lista vuota
    if(t->head==NULL) printf("\nLa lista non contiene alcun elemento!\n\n");
    
    for(x = tabPg->head; x->next!=NULL && (strcmp(nome,x->val.nome)!=0); x=x->next);
    if(x->next==NULL && (strcmp(nome,x->val.nome)!=0)) printf("\nPersonaggio non presente nella lista!\n\n");

    for(i=0;i<x->val.equip.inUso;i++)
        if(strcmp(equip,x->val.equip.vettEq[i]->nome)==0)
            break;
    
    if(i==x->val.equip.inUso) printf("\nEquipaggiamento non presente!\n\n");
    else
    {
        int j=0;
        x->val.equip.inUso = x->val.equip.inUso-1;
        while(j<(x->val.equip.inUso)-1)
        {
            x->val.equip.vettEq[j]=x->val.equip.vettEq[j+1];
            i++;
        }
    }
}

inv_t *ricercaEquipaggiamento(tabInv_t* tabInv, char *equipaggiamento)
{
    for(int i=0;i<tabInv->nInv;i++)
        if(strcmp(equipaggiamento,tabInv->vettInv[i]->nome)==0) return tabInv->vettInv[i];
    return NULL;
}

void stampa(tabPg_t* tabPg)
{
    nodoPg_t *x;
    int i;
    char nome[maxLength];
    
    printf("Nome del personaggio: ");
    scanf("%s", nome);
    
    for(x = tabPg->head; x->next!=NULL && (strcmp(nome,x->val.nome)!=0); x=x->next);
    if(x->next==NULL&&(strcmp(nome,x->val.nome)!=0)) printf("\nPersonaggio non presente nella lista!\n\n");
    else
    {
        for(i=0;i<x->val.equip.inUso;i++){
            x->val.stat.hp+=x->val.equip.vettEq[i]->stat.hp;
            x->val.stat.mp+=x->val.equip.vettEq[i]->stat.mp;
            x->val.stat.atk+=x->val.equip.vettEq[i]->stat.atk;
            x->val.stat.def+=x->val.equip.vettEq[i]->stat.def;
            x->val.stat.mag+=x->val.equip.vettEq[i]->stat.mag;
            x->val.stat.spr+=x->val.equip.vettEq[i]->stat.spr;
        }
        if(x->val.stat.hp<0)
            x->val.stat.hp=0;
        if(x->val.stat.mp<0)
            x->val.stat.mp=0;
        if(x->val.stat.atk<0)
            x->val.stat.atk=0;
        if(x->val.stat.def<0)
            x->val.stat.def=0;
        if(x->val.stat.mag<0)
            x->val.stat.mag=0;
        if(x->val.stat.spr<0)
            x->val.stat.spr=0;
        printf("%s %s %s %d %d %d %d %d %d\n",x->val.codice,x->val.nome,x->val.classe,x->val.stat.hp,x->val.stat.mp,x->val.stat.atk,x->val.stat.def,x->val.stat.mag,x->val.stat.spr);
    }
}
