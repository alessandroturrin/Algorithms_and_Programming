#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxR 1000
#define maxChar 31

typedef enum {r_date, r_partenza, r_capolinea, r_ritardo, r_ritardo_tot,r_fine} comando_e;

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
int selezionaDati(tratta automezzo[maxR], int numeroTratte, comando_e comando);
void date(tratta automezzo[maxR], int numeroTratte);
void partenza(tratta automezzo[maxR], int numeroTratte);
void capolinea(tratta automezzo[maxR], int numeroTratte);
void ritardo(tratta automezzo[maxR], int numeroTratte);
void ritardoComplessivo(tratta automezzo[maxR], int numeroTratte);

int main()
{
    char fileLog[] = "log.txt"; //nome file di input
    tratta automezzo[maxR]; //vettore di struct contenente le tratte
    int numeroTratte;
    comando_e comando; //comando da impartire
    int continua = 1;
    
    numeroTratte = inputFile(automezzo, fileLog);
    
    while(continua)
    {
        comando = leggiComando();
        continua = selezionaDati(automezzo, numeroTratte, comando);
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
    comando_e com = r_date;
    char comando[maxChar];
    char *tabella[] = {"data","partenza","capolinea","ritardo","ritardo_totale","fine"};
    int trovato = 0;
    
    puts("[data,partenza,capolinea,ritardo,ritardo_totale,fine]");
    printf("Inserire comando: ");
    scanf("%s", comando);
    
    while(!trovato && com<(r_fine+1))
    {
        if(strcmp(tabella[com], comando)==0)
            trovato = 1;
        else
            com++;
    }
    
    return com;
}

int selezionaDati(tratta automezzo[maxR], int numeroTratte, comando_e comando)
{
    int continua = 1;
    
    switch (comando)
    {
        case r_date:
            date(automezzo, numeroTratte);
            break;
        case r_partenza:
            partenza(automezzo, numeroTratte);
            break;
        case r_capolinea:
            capolinea(automezzo, numeroTratte);
            break;
        case r_ritardo:
            ritardo(automezzo, numeroTratte);
            break;
        case r_ritardo_tot:
            ritardoComplessivo(automezzo, numeroTratte);
            break;
        case r_fine:
            continua=0;
            break;
    }
    
    return continua;
}

void date(tratta automezzo[maxR], int numeroTratte)
{
    int anno[2], mese[2], giorno[2];
    
    for(int i=0;i<2;i++)
    {
        printf("Inserire la data della %d° tratta [aaaa/mm/gg]: ", i+1);
        scanf("%d/%d/%d", &anno[i], &mese[i], &giorno[i]);
    }
    
    for(int i=0;i<numeroTratte;i++)
    {
        if((anno[0]>=automezzo[i].anno && anno[1]<=automezzo[i].anno) || (anno[1]>=automezzo[i].anno && anno[0]<=automezzo[i].anno))
            if(((mese[0]>=automezzo[i].mese && mese[1]<=automezzo[i].mese)) || (mese[1]>=automezzo[i].mese && mese[0]<=automezzo[i].mese))
                if(((giorno[0]>=automezzo[i].giorno && giorno[1]<=automezzo[i].giorno)) || (giorno[1]>=automezzo[i].giorno && giorno[0]<=automezzo[i].giorno))
                    printf("-%s\t%s->%s\n", automezzo[i].codiceTratta, automezzo[i].partenza, automezzo[i].destinazione);
    }
}

void partenza(tratta automezzo[maxR], int numeroTratte)
{
    char partenza[maxR];
    
    printf("Inserire la fermata di partenza: ");
    scanf("%s", partenza);
    
    for(int i=0;i<numeroTratte;i++)
        if(strcmp(automezzo[i].partenza, partenza)==0)
            printf("-%s\n", automezzo[i].codiceTratta);
}

void capolinea(tratta automezzo[maxR], int numeroTratte)
{
    char destinazione[maxR];
    
    printf("Inserire la fermata di capolinea: ");
    scanf("%s", destinazione);
    
    for(int i=0;i<numeroTratte;i++)
        if(strcmp(automezzo[i].destinazione, destinazione)==0)
            printf("-%s\n", automezzo[i].codiceTratta);
}

void ritardo(tratta automezzo[maxR], int numeroTratte)
{
    int anno[2], mese[2], giorno[2];
    
    for(int i=0;i<2;i++)
    {
        printf("Inserire la data della %d° tratta [aaaa/mm/gg]: ", i+1);
        scanf("%d/%d/%d", &anno[i], &mese[i], &giorno[i]);
    }
    
    for(int i=0;i<numeroTratte;i++)
    {
        if((anno[0]>=automezzo[i].anno && anno[1]<=automezzo[i].anno) || (anno[1]>=automezzo[i].anno && anno[0]<=automezzo[i].anno))
            if(((mese[0]>=automezzo[i].mese && mese[1]<=automezzo[i].mese)) || (mese[1]>=automezzo[i].mese && mese[0]<=automezzo[i].mese))
                if(((giorno[0]>=automezzo[i].giorno && giorno[1]<=automezzo[i].giorno)) || (giorno[1]>=automezzo[i].giorno && giorno[0]<=automezzo[i].giorno))
                    if(automezzo[i].ritardo!=0)
                        printf("-%s\t%s->%s\t+%d minuti\n", automezzo[i].codiceTratta, automezzo[i].partenza, automezzo[i].destinazione, automezzo[i].ritardo);
    }
}

void ritardoComplessivo(tratta automezzo[maxR], int numeroTratte)
{
    int ritardoTotale = 0;
    char codiceTratta[maxChar];
    
    printf("Inserire il codice tratta: ");
    scanf("%s", codiceTratta);
    
    for(int i=0;i<numeroTratte;i++)
        if(strcmp(codiceTratta,automezzo[i].codiceTratta)==0)
            ritardoTotale+=automezzo[i].ritardo;
    
    printf("Ritardo totale: %d\n", ritardoTotale);
}
