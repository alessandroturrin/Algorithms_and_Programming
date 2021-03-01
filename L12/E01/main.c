#include <stdio.h>
#include <stdlib.h>

#include "titoli.h"

typedef enum {input_da_file, ricerca_titolo, ricerca_quotazione, quotazione_minima_massima, bilanciamento_albero, fine, errore} comando_t;
char *tabellaComandi[] = {"Input_da_file", "Ricerca_titolo", "Ricerca_quotazione", "Quotazione_minima_massima", "Bilanciamento_albero", "Fine"};

comando_t leggiComando();
range_t sottoMenu();
int main(int argc, const char * argv[])
{
    FILE* fp;
    char nomeFile[100], nomeTitolo[maxChar];
    List list;
    comando_t comando;
    range_t sottoComando;
    bool continua = true;
    
    
    list = LISTinit();
    
    while (continua)
    {
        comando = leggiComando();
        
        switch(comando)
        {
            case input_da_file:
                printf("Nome file di input: ");
                scanf("%s", nomeFile);
                printf("\n");
                fp = fopen(nomeFile, "rt");
                LISTfread(fp, list);
                break;
            case ricerca_titolo:
                ricercaTitoloAzionario(list);
                break;
            case ricerca_quotazione:
                ricercaData(list);
                break;
            case quotazione_minima_massima:
                sottoComando = sottoMenu();
                ricercaMinMax(list, sottoComando);
                break;
            case bilanciamento_albero:
                bilanciaBST(list);
                break;
            case fine:
                continua = false;
                break;
            case errore:
                break;
        }
    }
    
    LISTfree(list);
}

comando_t leggiComando()
{
    int codiceComando;
    
    puts("---------- MENU' ---------");
    for(int i=input_da_file;i<errore;i++)
        printf("[%d] - %s\n", i+1, tabellaComandi[i]);
    printf("--------------------------\n\n");
    printf("Codice comando: ");
    scanf("%d", &codiceComando);
    codiceComando--;
    if(codiceComando>=input_da_file && codiceComando<errore)
        return codiceComando;
    else
        return errore;
}

range_t sottoMenu()
{
    int scelta;
    printf("------SOTTOMENU'------\n");
    printf("[1] - Ricerca per intervallo di date\n[2] - Ricerca su tutto il periodo\n");
    printf("----------------------\n\n");
    printf("Codice sotto-comando: ");
    scanf("%d", &scelta);
    scelta--;
    if(scelta==0 || scelta==1)
        return scelta;
    return -1;
}
