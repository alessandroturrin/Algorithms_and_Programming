#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxChar 51

typedef enum {ins_kb,ins_file,ricerca,del_singolo,del_multiplo,stampa_file,fine,errore} comando_e;
typedef enum {false, true} bool;
typedef enum {kCodice,kData} key;
typedef enum {e_singolo, d_inizio, d_fine, e_fine} comando_elim; //enum per gestione di listExtrKey

typedef struct
{
    int giorno, mese, anno;
}sData;
typedef struct
{
    char codice[6];
    char nome[maxChar];
    char cognome[maxChar];
    sData data;
    char via[maxChar];
    char citta[maxChar];
    char cap[maxChar];
    bool isVoid;
}Item;

typedef struct node *link;
struct node
{
    Item val;
    struct node *next;
};

link eseguiMenu(link h, bool *continua);
void leggiComando(comando_e *com);
Item insKB(void);
link insFile(link h);
link newNode(Item val, link next);
link SortListIns(link h, Item val);
int confrontaDate(sData d1, sData d2);
Item listSearch(link h, Item val, key k);
int compare(Item h, Item c, key k);
void stampa(link h);
void printVal(Item val);
Item ItemSetVoid(bool set);
Item listExtrKey(link *hp, Item comp, key k, comando_elim *cElim, sData dMin);
void freeLista(link h);

int main(int argc, const char * argv[])
{
    link anagrafica=NULL;
    bool continua = true;
    
    while(continua)
        anagrafica = eseguiMenu(anagrafica,&continua);
    
    freeLista(anagrafica);
}

link eseguiMenu(link h, bool *continua)
{
    comando_e comando;
    Item val;
    comando_elim cElim;
    
    leggiComando(&comando);
    switch (comando)
    {
        case ins_kb:
            val = insKB();
            h = SortListIns(h, val);
            printf("Inserimento avvenuto con successo!\n\n");
            break;
        case ins_file:
            h = insFile(h);
            break;
        case ricerca:
            printf("Inserire codice da cercare: ");
            scanf("%s", val.codice);
            val = listSearch(h, val, kCodice);
            if(val.isVoid==true) printf("Elemento non trovato!\n");
            else printVal(val);
            puts("");
            break;
        case del_singolo:
            printf("Inserire codice dell'elemento da eliminare: ");
            scanf("%s", val.codice);
            val = listSearch(h, val, kCodice);
            if(val.isVoid==true) printf("Elemento non trovato!\n\n");
            else
            {
                cElim = e_singolo;
                val = listExtrKey(&h, val, kCodice, &cElim, val.data);
                printf("Elemento eliminato: ");
                printVal(val);
                puts("");
            }
            break;
        case del_multiplo:
        {
            cElim = d_inizio;
            sData d1, d2;
            printf("Data-1 [gg/mm/aaaa]: ");
            scanf("%d/%d/%d", &d1.giorno, &d1.mese, &d1.anno);
            printf("Data-2 [gg/mm/aaaa]: ");
            scanf("%d/%d/%d", &d2.giorno, &d2.mese, &d2.anno);
            if(confrontaDate(d1, d2)>0)
            {
                printf("Impossibile eseguire il comando! Data-1 deve essere antecedente a Data-2!\n");
                break;
            }
            val.data = d2;
            val = listExtrKey(&h, val, kData, &cElim, d1);
            if(val.isVoid==false)
            {
                printf("Elementi eliminati: \n");
                printVal(val);
            }
            while (cElim!=e_fine && val.isVoid==false)
            {
                val.data = d1;
                val = listExtrKey(&h, val, kData, &cElim, d1);
                if(val.isVoid==false) printVal(val);
            }
            if(cElim!=e_fine) printf("Nessun dato corrispondente all'intervallo di date immesso!\n\n");
            else printf("\n\n");
            break;
        }
        case stampa_file:
            stampa(h);
            break;
        case fine:
            puts("Fine dell'esecuzione del programma!");
            *continua = false;
            break;
        case errore:
            printf("Errore rilevato!\n\n");
            break;
        default:
            break;
    }
    return h;
}

void leggiComando(comando_e *com)
{
    bool trovato = false;
    char *tabella[] = {"inserimento_tastiera","inserimento_file","ricerca","elimina_singolo","elimina_intervallo","stampa","fine"};
    char c[maxChar];
    puts("---Comandi disponibili---");
    for(int i=0;i<errore;i++) printf("%s\n", tabella[i]);
    printf("\n\nComando: ");
    scanf("%s", c);
    for(int i=0;i<errore && !trovato;i++)
        if(strcmp(tabella[i], c)==0)
        {
            *com = i;
            trovato = true;
        }
    if(trovato==false) *com = errore;
}

Item insKB()
{
    Item i;
    printf("<codice> <nome> <cognome> <gg/mm/aaaa> <via> <citta'> <cap>\nInput: ");
    scanf("%s %s %s %d/%d/%d %s %s %s", i.codice, i.nome, i.cognome, &i.data.giorno, &i.data.mese, &i.data.anno, i.via, i.citta, i.cap);
    return i;
}

link insFile(link h)
{
    FILE* file;
    char nomeFile[maxChar];
    Item i;
    
    printf("Nome del file di input: ");
    scanf("%s", nomeFile);
    if((file=fopen(nomeFile, "rt"))==NULL)
    {
        printf("Errore nell'apertura di '%s'\n", nomeFile);
        return h;
    }
    while(!feof(file))
    {
        fscanf(file, "%s %s %s%d/%d/%d %s %s %s", i.codice, i.nome, i.cognome, &i.data.giorno, &i.data.mese, &i.data.anno, i.via, i.citta, i.cap);
        if(!feof(file))
            h = SortListIns(h, i);
    }
    printf("Input da file '%s' avvenuto con successo!\n\n", nomeFile);
    fclose(file);
    return h;
}

link newNode(Item val, link next)
{
    link x = malloc(sizeof*x);
    if(x==NULL) return NULL;
    x->val = val;
    x->next = next;
    return x;
}

link SortListIns(link h, Item val)
{
    link x,p;
    if(h == NULL || confrontaDate(val.data, h->val.data)>=0) return newNode(val,h);
    for(x=h->next, p=h; x!=NULL && confrontaDate(val.data, h->val.data)<0; p=x, x=x->next);
    p->next = newNode(val,x);
    return h;
}

int confrontaDate(sData d1, sData d2)
{
    if(d1.anno==d2.anno)
    {
        if(d1.mese==d2.mese) return d1.giorno-d2.giorno;
        else return d1.mese-d2.mese;
    }
    else return d1.anno-d2.anno;
}

Item listSearch(link h, Item val, key k)
{
    link x;
    for(x=h;x!=NULL;x=x->next)
        if (compare(x->val, val, k)==0)
        {
            x->val.isVoid = false;
            return x->val;
        }
    return ItemSetVoid(true);
}

Item listExtrKey(link *hp, Item comp, key k, comando_elim *cElim, sData dMin)
{
        
    int cmp = -1;
    link *xp,t;
    Item i = ItemSetVoid(true);
    for(xp=hp;(*xp)!=NULL;xp=&((*xp)->next))
    {
        if((*cElim)==e_singolo)   //caso elimino singolo elemento, effettuo il confronto effettivo
            cmp = compare((*xp)->val, comp, k);
        else if((*cElim)==d_fine)
        {
            if(compare((*hp)->val, comp, k)>=0)
                cmp = 0;
            else
            {
                cmp = -1;
                *cElim = e_fine;
            }
        }
        else if((*cElim)==d_inizio)
        {
            if(compare((*hp)->val, comp, k)<=0 && confrontaDate((*hp)->val.data, dMin)>=0)
            {
                cmp = 0;
                *cElim = d_fine;
            }
        }
        
        if(cmp==0)
        {
            t = *xp;
            *xp = (*xp)->next;
            i = t->val;
            i.isVoid = false;
            free(t);
            break;
        }
    }
    if((*xp)==NULL && (*cElim)==d_fine) *cElim = e_fine;
    return i;
}

int compare(Item h, Item c, key k)
{
    if(k==kCodice) return strcmp(h.codice, c.codice);
    else return confrontaDate(h.data, c.data);
}

void stampa(link h)
{
    char nomeFile[maxChar];
    FILE* file;
    link x;
    
    printf("Nome del file di output: ");
    scanf("%s", nomeFile);
    file = fopen(nomeFile, "at");
    fprintf(file, "---Output---\n");
    for(x=h;x!=NULL;x=x->next)
        fprintf(file, "%d/%d/%d %s %s %s %s %s %s\n", x->val.data.giorno, x->val.data.mese, x->val.data.anno, x->val.codice, x->val.nome, x->val.cognome, x->val.via, x->val.citta, x->val.cap);
    fprintf(file, "\n\n\n");
    printf("Output su file '%s' completato!\n\n", nomeFile);
    fclose(file);
}

void printVal(Item val)
{
    printf("%d/%d/%d %s %s %s %s %s %s\n", val.data.giorno, val.data.mese, val.data.anno, val.codice, val.nome, val.cognome, val.via, val.citta, val.cap);
}

Item ItemSetVoid(bool set)
{
    Item i;
    i.isVoid = set;
    return i;
}

void freeLista(link h){
    if(h!=NULL) {
        freeLista(h->next);
        free (h);
    }
}
