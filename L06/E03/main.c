#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define maxChar 31
#define maxFile 100
#define nChiavi 4

typedef enum {false,true} bool;
typedef enum {stampa_originale,stampa_data,stampa_tratta,stampa_partenza,stampa_destinazione,ricerca,cambia_file,fine} comando_e;
typedef enum {pData, pTratta, pPartenza, pDestinazione} tipo_p;

typedef struct
{
    char *codiceTratta;
    char *partenza;
    char *destinazione;
    char *data;
    char *orarioPartenza;
    char *orarioArrivo;
    int ritardo;
}tratta;

tratta *input(int *numeroTratte);
tratta ***setOrdinamenti(tratta *automezzo, int numeroTratte);
void ordinaPuntatori(tratta **pointerAutomezzo, int numeroTratte, tipo_p chiave);
int compare(tratta *autoUno, tratta *autoDue, tipo_p key);
comando_e leggiComando(void);
int esegui(tratta *automezzo, tratta ***o, int numeroTratte, comando_e comando);
void stampaOriginale(tratta a[], int numeroTratte);
void stampaVettori(tratta *automezzo[], int numeroTratte);
void freeVettore(tratta *a, int numeroTratte);
void freeMatrice(tratta ***o, int numeroTratte);
size_t binarySearch(tratta ***o, char *ricerca, tipo_p chiave, size_t low, size_t high);
int binSrcCompare(tratta ***o, char* ricerca, int pos, tipo_p chiave);
int main()
{
    tratta *automezzo, ***ordinamenti;
    int numeroTratte, continua=1;
    comando_e comando;
    
    automezzo = input(&numeroTratte);
    ordinamenti = setOrdinamenti(automezzo, numeroTratte);
    while(continua!=0)
    {
        comando = leggiComando();
        continua = esegui(automezzo, ordinamenti, numeroTratte, comando);
        if(continua==2) //reset di vettore e matrice
        {
            freeVettore(automezzo, numeroTratte);
            freeMatrice(ordinamenti, numeroTratte);
            automezzo = input(&numeroTratte);
            ordinamenti = setOrdinamenti(automezzo, numeroTratte);
        }
    }
}

tratta *input(int *numeroTratte)
{
    char nomeFile[maxFile];
    FILE* file;
    tratta *a;
    char codice[maxChar], partenza[maxChar], destinazione[maxChar], data[maxChar], oPartenza[maxChar], oArrivo[maxChar];
    
    printf("Nome del file di input: ");
    scanf("%s", nomeFile);
    if((file=fopen(nomeFile, "rt"))==NULL)
    {
        printf("Errore nell'apertura di '%s'\Esecuzione del programma interrotta!\n", nomeFile);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", numeroTratte);
    a = (tratta*)malloc((*numeroTratte)*sizeof(tratta));
    for(int i=0;i<(*numeroTratte);i++)
    {
        fscanf(file, "%s %s %s %s %s %s %d", codice, partenza, destinazione, data, oPartenza, oArrivo, &a[i].ritardo);
        a[i].codiceTratta = (char*)malloc((strlen(codice)+1)*sizeof(char));
        a[i].partenza = (char*)malloc((strlen(partenza)+1)*sizeof(char));
        a[i].destinazione = (char*)malloc((strlen(destinazione)+1)*sizeof(char));
        a[i].data = (char*)malloc((strlen(data)+1)*sizeof(char));
        a[i].orarioPartenza = (char*)malloc((strlen(oPartenza)+1)*sizeof(char));
        a[i].orarioArrivo = (char*)malloc((strlen(oArrivo)+1)*sizeof(char));
        strcpy(a[i].codiceTratta, codice);
        strcpy(a[i].partenza, partenza);
        strcpy(a[i].destinazione, destinazione);
        strcpy(a[i].data, data);
        strcpy(a[i].orarioPartenza, oPartenza);
        strcpy(a[i].orarioArrivo, oArrivo);
    }
    fclose(file);
    return a;
}

tratta ***setOrdinamenti(tratta *automezzo, int numeroTratte)
{
    tratta ***o;
    o = (tratta***)malloc(nChiavi*sizeof(tratta**));
    for(int i=0;i<nChiavi;i++)
    {
        o[i] = (tratta**)malloc(numeroTratte*sizeof(tratta*));
        for(int j=0;j<numeroTratte;j++)
            o[i][j] = &automezzo[j];
        ordinaPuntatori(o[i], numeroTratte, i);
    }
    return o;
}

void ordinaPuntatori(tratta **pointerAutomezzo, int numeroTratte, tipo_p chiave)
{
    int i, j, imin;
    tratta *temp;
    for(i=0;i<(numeroTratte-1);i++)
    {
        imin = i;
        for(j=i+1;j<numeroTratte;j++)
            if(compare(pointerAutomezzo[j], pointerAutomezzo[imin], chiave)<0)
                imin = j;
        temp = pointerAutomezzo[i];
        pointerAutomezzo[i] = pointerAutomezzo[imin];
        pointerAutomezzo[imin] = temp;
    }
}

int compare(tratta *autoUno, tratta *autoDue, tipo_p key)
{
    switch (key)
    {
        case pData: //caso ordinamento per data
        {
            int check = strcmp(autoUno->data, autoDue->data);
            if(check!=0) return check; else return strcmp(autoUno->orarioPartenza, autoDue->orarioPartenza);
        }
        case pTratta: //caso ordinamento per codice tratta
            return strcmp(autoUno->codiceTratta, autoDue->codiceTratta);
        case pPartenza: //caso ordinamento per stazione di partenza
            return strcmp(autoUno->partenza, autoDue->partenza);
        case pDestinazione: //caso ordinamento per stazione d arrivo
            return strcmp(autoUno->destinazione, autoDue->destinazione);
        default:
            return -1;
    }
}
//fino a qui bene
comando_e leggiComando(void)
{
    comando_e com = stampa_originale;
    char comando[maxChar];
    char *tabella[] = {"stampa_originale","stampa_data","stampa_tratta","stampa_partenza","stampa_destinazione","ricerca","cambia_file","fine"};
    int trovato = 0;
    
    puts("[stampa_originale, stampa_data, stampa_tratta, stampa_partenza, stampa_destinazione, ricerca, cambia_file, fine]");
    printf("Inserire comando: ");
    scanf("%s", comando);
    
    while(!trovato && com<(fine+1))
    {
        if(strcmp(tabella[com], comando)==0)
            trovato = 1;
        else
            com++;
    }
    return com;
}

int esegui(tratta *a, tratta ***o, int numeroTratte, comando_e comando)
{
    switch (comando)
    {
        case stampa_originale:
            printf("---Stampa del vettore originale!---\n");
            stampaOriginale(a, numeroTratte);
            return 1;
        case stampa_data:
            printf("---Stampa dell'ordinamento rispetto a data!---\n");
            stampaVettori(o[pData], numeroTratte);
            return 1;
        case stampa_tratta:
            printf("---Stampa dell'ordinamento rispetto a tratta!---\n");
            stampaVettori(o[pTratta], numeroTratte);
            return 1;
        case stampa_partenza:
            printf("---Stampa dell'ordinamento rispetto a partenza!---\n");
            stampaVettori(o[pPartenza], numeroTratte);
            return 1;
        case stampa_destinazione:
            printf("---Stampa dell'ordinamento rispetto a destinazione!---\n");
            stampaVettori(o[pDestinazione], numeroTratte);
            return 1;
        case ricerca:
        {
            tipo_p com = pData;
            bool trovato = false;
            char ricerca[maxChar], chiave[maxChar];
            char *tabella[] = {"pData","pTratta","pPartenza","pDestinazione"};
            printf("Elemento da cercare: ");
            scanf("%s", ricerca);
            printf("Chiave di ricerca? [pData,pTratta,pPartenza,pDestinazione]\nChiave: ");
            scanf("%s", chiave);
            while(!trovato && com<=(pDestinazione))
            {
                if(strcmp(tabella[com], chiave)==0)
                    trovato = true;
                else
                    com++;
            }
            if(trovato)
            {
                int pos = binarySearch(o, ricerca, com, 0, numeroTratte);
                if(pos!=-1) printf("L'elemento '%s' si trova in posizione %d\n\n", ricerca, pos);
                else printf("Elemento non trovato!\n\n");
            }
            else printf("---Errore nell'inserimento del comando!---\n\n");
            return 1;
        }
        case cambia_file:
            return 2;
            break;
        case fine:
            printf("---Fine dell'esecuzione del programma!---\n\n");
            return 0;
        default:
            break;
    }
    return 1;
}

void stampaOriginale(tratta a[], int numeroTratte)
{
    for(int i=0;i<numeroTratte;i++)
    {
        printf("%s %s %s %s %s %s %d\n", a[i].codiceTratta, a[i].partenza, a[i].destinazione, a[i].data, a[i].orarioPartenza, a[i].orarioArrivo, a[i].ritardo);
    }
    puts("");
}

void stampaVettori(tratta *a[], int numeroTratte)
{
    for(int i=0;i<numeroTratte;i++)
    {
        printf("%s %s %s %s %s %s %d\n", a[i]->codiceTratta, a[i]->partenza, a[i]->destinazione, a[i]->data, a[i]->orarioPartenza, a[i]->orarioArrivo, a[i]->ritardo);
    }
    puts("");
}

void freeVettore(tratta *a, int numeroTratte)
{
    for(int i=0;i<numeroTratte;i++)
    {
        free(a[i].codiceTratta);
        free(a[i].data);
        free(a[i].partenza);
        free(a[i].destinazione);
        free(a[i].orarioPartenza);
        free(a[i].orarioArrivo);
    }
    free(a);
    puts("---deallocazione del vettore avvenuta con successo---");
}

void freeMatrice(tratta ***o, int numeroTratte)
{
    for(int i=0;i<nChiavi;i++) free(o[i]);
    free(o);
    printf("---deallocazione della matrice avvenuta con successo---\n\n");
}

size_t binarySearch(tratta ***o, char *ricerca, tipo_p chiave, size_t low, size_t high)
{
    int comp;
    while(low<=high)
    {
        size_t middle = (low+high)/2;
        comp = binSrcCompare(o, ricerca, middle, chiave);
        if(comp==0)
            return middle;
        else if(comp>0)
            high = middle-1;
        else if(comp<0)
            low = middle+1;
        else break;
    }
    return -1;;
}

int binSrcCompare(tratta ***o, char* ricerca, int pos, tipo_p chiave)
{
    if(chiave==pData) return strncmp(o[chiave][pos]->data, ricerca, strlen(ricerca));
    if(chiave==pTratta) return strncmp(o[chiave][pos]->codiceTratta, ricerca, strlen(ricerca));
    if(chiave==pPartenza) return strncmp(o[chiave][pos]->partenza, ricerca, strlen(ricerca));
    if(chiave==pDestinazione) return strncmp(o[chiave][pos]->destinazione, ricerca, strlen(ricerca));
    return -1;
}
