#include "personaggi.h"

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

void ricercaPersonaggio(tabPg_t* tabPg)
{
    char codice[maxLength];
    nodoPg_t *x;
    
    printf("Codice del personaggio da cercare: ");
    scanf("%s", codice);
    
    for(x = tabPg->head;x!=NULL;x=x->next)
        if(strcmp(x->val.codice,codice)==0)
        {
            printf("Personaggio trovato\n%s: %s %s\n\n", x->val.codice, x->val.nome, x->val.classe);
            return;
        }
    printf("Personaggio non trovato!\n\n");
}

void stampaPersonaggioEquipaggiamento(tabPg_t* tabPg)
{
    nodoPg_t *x;
    char codice[maxLength];
    int i;
    
    printf("Codice del personaggio da stampare: ");
    scanf("%s", codice);
    for(x = tabPg->head;x!=NULL;x=x->next)
        if(strcmp(x->val.codice,codice)==0)
        {
            printf("%s %s %s %d %d %d %d %d %d \n", x->val.codice, x->val.nome, x->val.classe, x->val.stat.hp, x->val.stat.mp, x->val.stat.atk, x->val.stat.def, x->val.stat.mag, x->val.stat.spr);
            printf("Equipaggiamento ");
            for(i=0;i<x->val.equip.inUso;i++)
                printf("\n-%s: %s", x->val.equip.vettEq[i]->nome, x->val.equip.vettEq[i]->tipo);
            if(i==0)
                printf("non presente!\n\n");
            else printf("\n\n");
            return;
        }
    printf("Personaggio non trovato!\n\n");
    
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

void calcolaStatistiche(tabPg_t* tabPg)
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

void freeLinkPg(link_pg x)
{
    free(x->val.codice);
    free(x->val.nome);
    free(x->val.classe);
    free(x);
}
