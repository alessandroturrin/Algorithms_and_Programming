#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxR 1000
#define maxChar 31

typedef enum {false,true} bool;
typedef enum {stampa, ordina_data, ordina_tratta, ordina_partenza, ordina_destinazione, ricerca, fine} comando_e;

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
int selezionaDati(tratta automezzo[maxR], int numeroTratte, comando_e comando, int comandoPrecedente);
void display(tratta automezzo[maxR], int numeroTratte);
void insertionSort(tratta automezzo[maxR], int numeroTratte, int choice); //uso la stessa procedura per confrontare orario, codice tratta, partenza, destinazione
int compare(tratta autoUno, tratta autoDue, int choice); //confronto con switch/case e determino caso
int controlloDate(tratta autoUno, tratta autoDue); //confronto date con precisione al secondo
size_t binarySearch(tratta automezzo[maxR], char *key, size_t low, size_t high);
size_t ricercaLineare(tratta automezzo[maxR], char *key, int numeroTratte);
int main()
{
    char fileLog[] = "log.txt"; //nome file di input
    tratta automezzo[maxR]; //vettore di struct contenente le tratte
    int numeroTratte;
    comando_e comando; //comando da impartire
    int comandoPrecedente = 1;
    bool continua = true;
    
    numeroTratte = inputFile(automezzo, fileLog);
    
    while(continua)
    {
        comando = leggiComando();
        comandoPrecedente = selezionaDati(automezzo, numeroTratte, comando, comandoPrecedente);
        if(comandoPrecedente==0) continua=false;
        printf("\n\n");
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
    comando_e com = stampa;
    char comando[maxChar];
    char *tabella[] = {"stampa","ordina_data","ordina_tratta","ordina_partenza","ordina_destinazione","ricerca","fine"};
    int trovato = 0;
    
    puts("[stampa,ordina_data,ordina_tratta,ordina_partenza,ordina_destinazione,ricerca,fine]");
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

int selezionaDati(tratta automezzo[maxR], int numeroTratte, comando_e comando, int comandoPrecedente)
{
    
    switch (comando)
    {
        case stampa:
            display(automezzo, numeroTratte);
            return 1;
            break;
        case ordina_data:
            insertionSort(automezzo, numeroTratte, 0);
            return 1;
            break;
        case ordina_tratta:
            insertionSort(automezzo, numeroTratte, 1);
            return 1;
            break;
        case ordina_partenza:
            insertionSort(automezzo, numeroTratte, 2);
            return 2;
            break;
        case ordina_destinazione:
            insertionSort(automezzo, numeroTratte, 3);
            return 1;
            break;
        case ricerca:
        {
            char key[maxChar];
            size_t pos;
            printf("Chiave di ricerca: ");
            scanf("%s", key);
            if(comandoPrecedente==2)
            {
                printf("\n---Ricerca mediante metodo dicotomico---\n");
                pos = binarySearch(automezzo, key, 0, numeroTratte);
            }
            else
            {
                printf("\n---Ricerca mediante metodo lineare---\n");
                pos = ricercaLineare(automezzo, key, numeroTratte);
            }
            (pos!=-1) ? printf("Elemento trovato in posizione %lU\n", pos) : puts("Elemento non trovato!");
            return comandoPrecedente;
            break;
        }
        default:
            return 0;
            break;
    }
}

void display(tratta automezzo[maxR], int numeroTratte)
{
    puts("");
    for(int i=0;i<numeroTratte;i++)
    {
        printf("------AUTOMEZZO N°%d------\n", i+1);
        printf("Codice tratta: [%s]\nPartenza: [%s]\tDestinazione: [%s]\n", automezzo[i].codiceTratta, automezzo[i].partenza, automezzo[i].destinazione);
        printf("Data di partenza: [%d/%d/%d]\nOraio di partenza: [%d:%d:%d]", automezzo[i].anno, automezzo[i].mese, automezzo[i].giorno, automezzo[i].oraPartenza, automezzo[i].minutiPartenza, automezzo[i].secondiPartenza);
        printf("\tOrario di arrivo: [%d:%d:%d]\tRitardo: [+%d']\n-------------------------\n\n", automezzo[i].oraArrivo, automezzo[i].minutiArrivo, automezzo[i].secondiArrivo, automezzo[i].ritardo);
    }
}

void insertionSort(tratta automezzo[maxR], int numeroTratte, int choice) //ordinamento crescente
{
    int i, j, l=0, r=numeroTratte-1;
    tratta swap;
    
    for(i=l+1;i<=r;i++)
    {
        j=i-1;
        swap = automezzo[i];
        while(j>=l && compare(automezzo[j],swap,choice)>0)
        {
            automezzo[j+1] = automezzo[j];
            j--;
        }
        automezzo[j+1]=swap;
    }
    
    puts("Ordinamento avvenuto con successo! Immettere 'stampa' per visualizzare le modifiche effettuate!");
}

int compare(tratta autoUno, tratta autoDue, int choice)
{
    switch (choice)
    {
        case 0: //caso ordinamento per data
            return controlloDate(autoUno, autoDue);
            break;
        case 1: //caso ordinamento per codice tratta
            return strcmp(autoUno.codiceTratta, autoDue.codiceTratta);
            break;
        case 2: //caso ordinamento per stazione di partenza
            return strcmp(autoUno.partenza, autoDue.partenza);
            break;
        case 3: //caso ordinamento per stazione d arrivo
            return strcmp(autoUno.destinazione, autoDue.destinazione);
            break;
        default:
            return -1;
            break;
    }
}

int controlloDate(tratta autoUno, tratta autoDue)
{
    if(autoUno.anno==autoDue.anno)
        if(autoUno.mese==autoDue.mese)
            if(autoUno.giorno==autoDue.giorno)
                if(autoUno.oraPartenza==autoDue.oraPartenza)
                    if(autoUno.minutiPartenza==autoDue.minutiPartenza) return autoUno.secondiPartenza-autoDue.secondiPartenza;
                    else return autoUno.minutiPartenza-autoDue.minutiPartenza;
                else return autoUno.oraPartenza-autoDue.oraPartenza;
            else return autoUno.giorno-autoDue.giorno;
        else return autoUno.mese-autoDue.mese;
    else return autoUno.anno-autoDue.anno;
}

size_t binarySearch(tratta automezzo[maxR], char *key, size_t low, size_t high)
{
    int comp;
    while(low<=high)
    {
        size_t middle = (low+high)/2;
        comp = strncmp(automezzo[middle].partenza, key, strlen(key));
        if(comp==0)
            return middle;
        else if(comp>0)
            high = middle-1;
        else
            low = middle+1;
    }
    return -1;;
}

size_t ricercaLineare(tratta automezzo[maxR], char *key, int numeroTratte)
{
    for(int i=0;i<numeroTratte;i++)
        if(strncmp(automezzo[i].partenza, key, strlen(key))==0) return i;
    return -1;
}
