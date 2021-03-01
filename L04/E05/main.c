#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxR 1000
#define maxChar 31

typedef enum {false,true} bool;
typedef enum {stampa_originale,stampa_data,stampa_tratta,stampa_partenza,stampa_destinazione,ricerca,fine} comando_e;
typedef enum {pData, pTratta, pPartenza, pDestinazione} tipo_p;
typedef struct
{
    char codiceTratta[maxR];
    char partenza[maxR];
    char destinazione[maxR];
    int anno, mese, giorno;
    int oraPartenza, minutiPartenza, secondiPartenza;
    int oraArrivo, minutiArrivo, secondiArrivo;
    int ritardo;
}tratta;

int inputFile(tratta automezzo[], char fileLog[]);
comando_e leggiComando(void);
void ordinamentoPointer(tratta *pointerAutomezzo[], int numeroTratte, tipo_p com);
int compare(tratta *autoUno, tratta *autoDue, tipo_p com);
int controlloDate(tratta *autoUno, tratta *autoDue);
void print(tratta *automezzo[], int numeroTratte);
size_t binarySearch(tratta *automezzo[], char *key, size_t low, size_t high);
size_t ricercaLineare(tratta automezzo[], char *key, int numeroTratte);
int main()
{
    tratta automezzo[maxR]; //vettore di struct contenente le tratte
    tratta *pointerData[maxR], *pointerTratta[maxR], *pointerPartenza[maxR], *pointerDestinazione[maxR];
    char fileLog[] = "log.txt"; //nome file di input
    int numeroTratte;
    comando_e comando; //comando da impartire
    bool continua = true;
    
    numeroTratte = inputFile(automezzo, fileLog);
    
    for(int i=0;i<numeroTratte;i++)
    {
        pointerData[i] = &automezzo[i];
        pointerTratta[i] = &automezzo[i];
        pointerPartenza[i] = &automezzo[i];
        pointerDestinazione[i] = &automezzo[i];
    }
    
    ordinamentoPointer(pointerData, numeroTratte, pData);
    ordinamentoPointer(pointerTratta, numeroTratte, pTratta);
    ordinamentoPointer(pointerPartenza, numeroTratte, pPartenza);
    ordinamentoPointer(pointerDestinazione, numeroTratte, pDestinazione);
    
    while(continua)
    {
        comando = leggiComando();
        
        switch (comando)
        {
            case stampa_originale:
            {
                printf("\nStampa del vettore originale!\n\n");
                puts("");
                for(int i=0;i<numeroTratte;i++)
                {
                    printf("------AUTOMEZZO N°%d------\n", i);
                    printf("Codice tratta: [%s]\nPartenza: [%s]\tDestinazione: [%s]\n", automezzo[i].codiceTratta, automezzo[i].partenza, automezzo[i].destinazione);
                    printf("Data di partenza: [%d/%d/%d]\nOraio di partenza: [%d:%d:%d]", automezzo[i].anno, automezzo[i].mese, automezzo[i].giorno, automezzo[i].oraPartenza, automezzo[i].minutiPartenza, automezzo[i].secondiPartenza);
                    printf("\tOrario di arrivo: [%d:%d:%d]\tRitardo: [+%d']\n-------------------------\n\n", automezzo[i].oraArrivo, automezzo[i].minutiArrivo, automezzo[i].secondiArrivo, automezzo[i].ritardo);
                }
            }
                break;
            case stampa_data:
                printf("\nStampa del vettore di piuntatori per l'ordinamento delle date!\n\n");
                print(pointerData, numeroTratte);
                break;
            case stampa_tratta:
                printf("\nStampa del vettore di piuntatori per l'ordinamento delle tratte!\n\n");
                print(pointerTratta, numeroTratte);
                break;
            case stampa_partenza:
                printf("\nStampa del vettore di piuntatori per l'ordinamento delle partenze!\n\n");
                print(pointerPartenza, numeroTratte);
                break;
            case stampa_destinazione:
                printf("\nStampa del vettore di piuntatori per l'ordinamento delle destinazioni!\n\n");
                print(pointerDestinazione, numeroTratte);
                break;
            case ricerca:
                {
                    char key[maxChar], modalita[maxChar];
                    size_t pos;
                    printf("Chiave di ricerca: ");
                    scanf("%s", key);
                    printf("Ricerca dicotomica o lineare? [dicotomica, lineare]\nModalita': ");
                    scanf("%s", modalita);
                    
                    if(strcmp(modalita,"dicotomica")==0)
                    {
                        printf("\n---Ricerca mediante metodo dicotomico---\n");
                        pos = binarySearch(pointerPartenza, key, 0, numeroTratte);
                    }
                    else if(strcmp(modalita, "lineare")==0)
                    {
                        printf("\n---Ricerca mediante metodo lineare---\n");
                        pos = ricercaLineare(automezzo, key, numeroTratte);
                    }
                    else
                    {
                        puts("Errore nell'inserimento del comando!\n");
                        break;
                    }
                    (pos!=-1) ? printf("Elemento trovato in posizione %lU\n", pos) : puts("Elemento non trovato!");
                }
                break;
            case fine:
                printf("\nFine dell'esecuzione del programma!\n\n");
                continua=false;
                break;
            default:
                printf("\nErrore nella selezione del comando!\n\n");
                break;
        }
    }
}

int inputFile(tratta automezzo[], char fileLog[])
{
    FILE* input;
    int numeroTratte;
    
    if((input = fopen(fileLog, "rt"))==NULL)
    {
        printf("Errore nell'apertura di '%s'!\n", fileLog);
        exit(-1);
    }
    
    fscanf(input, "%d", &numeroTratte);
    for(int i=0;i<numeroTratte;i++)
    {
        fscanf(input, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d", automezzo[i].codiceTratta, automezzo[i].partenza, automezzo[i].destinazione, &automezzo[i].anno, &automezzo[i].mese, &automezzo[i].giorno, &automezzo[i].oraPartenza, &automezzo[i].minutiPartenza, &automezzo[i].secondiPartenza, &automezzo[i].oraArrivo, &automezzo[i].minutiArrivo, &automezzo[i].secondiArrivo, &automezzo[i].ritardo);
    }
    
    return numeroTratte;
}

comando_e leggiComando(void)
{
    comando_e com = stampa_originale;
    char comando[maxChar];
    char *tabella[] = {"stampa_originale","stampa_data","stampa_tratta","stampa_partenza","stampa_destinazione","ricerca","fine"};
    int trovato = 0;
    
    puts("[stampa_originale, stampa_data, stampa_tratta, stampa_partenza, stampa_destinazione, ricerca, fine]");
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

void ordinamentoPointer(tratta *pointerAutomezzo[], int numeroTratte, tipo_p com)
{
    tratta *temp;
    int i, j, imin;
    for(i=0;i<numeroTratte-1;i++)
    {
        imin=i;
        for(j=i+1;j<numeroTratte;j++)
            if(compare(pointerAutomezzo[j], pointerAutomezzo[imin], com)<0)
                imin = j;
        temp = pointerAutomezzo[i];
        pointerAutomezzo[i] = pointerAutomezzo[imin];
        pointerAutomezzo[imin] = temp;
    }
}

int compare(tratta *autoUno, tratta *autoDue, tipo_p com)
{
    switch (com)
    {
        case pData: //caso ordinamento per data
            return controlloDate(autoUno, autoDue);
            break;
        case pTratta: //caso ordinamento per codice tratta
            return strcmp(autoUno->codiceTratta, autoDue->codiceTratta);
            break;
        case pPartenza: //caso ordinamento per stazione di partenza
            return strcmp(autoUno->partenza, autoDue->partenza);
            break;
        case pDestinazione: //caso ordinamento per stazione d arrivo
            return strcmp(autoUno->destinazione, autoDue->destinazione);
            break;
        default:
            return -1;
            break;
    }
}

int controlloDate(tratta *autoUno, tratta *autoDue)
{
    if(autoUno->anno==autoDue->anno)
        if(autoUno->mese==autoDue->mese)
            if(autoUno->giorno==autoDue->giorno)
                if(autoUno->oraPartenza==autoDue->oraPartenza)
                    if(autoUno->minutiPartenza==autoDue->minutiPartenza) return autoUno->secondiPartenza-autoDue->secondiPartenza;
                    else return autoUno->minutiPartenza-autoDue->minutiPartenza;
                else return autoUno->oraPartenza-autoDue->oraPartenza;
            else return autoUno->giorno-autoDue->giorno;
        else return autoUno->mese-autoDue->mese;
    else return autoUno->anno-autoDue->anno;
}

void print(tratta *automezzo[], int numeroTratte)
{
    puts("");
    for(int i=0;i<numeroTratte;i++)
    {
        printf("------AUTOMEZZO N°%d------\n", i);
        printf("Codice tratta: [%s]\nPartenza: [%s]\tDestinazione: [%s]\n", automezzo[i]->codiceTratta, automezzo[i]->partenza, automezzo[i]->destinazione);
        printf("Data di partenza: [%d/%d/%d]\nOraio di partenza: [%d:%d:%d]", automezzo[i]->anno, automezzo[i]->mese, automezzo[i]->giorno, automezzo[i]->oraPartenza, automezzo[i]->minutiPartenza, automezzo[i]->secondiPartenza);
        printf("\tOrario di arrivo: [%d:%d:%d]\tRitardo: [+%d']\n-------------------------\n\n", automezzo[i]->oraArrivo, automezzo[i]->minutiArrivo, automezzo[i]->secondiArrivo, automezzo[i]->ritardo);
    }
}

size_t binarySearch(tratta *automezzo[], char *key, size_t low, size_t high)
{
    int comp;
    while(low<=high)
    {
        size_t middle = (low+high)/2;
        comp = strncmp(automezzo[middle]->partenza, key, strlen(key));
        if(comp==0)
            return middle;
        else if(comp>0)
            high = middle-1;
        else
            low = middle+1;
    }
    return -1;;
}

size_t ricercaLineare(tratta automezzo[], char *key, int numeroTratte)
{
    for(int i=0;i<numeroTratte;i++)
        if(strncmp(automezzo[i].partenza, key, strlen(key))==0) return i;
    return -1;
}
