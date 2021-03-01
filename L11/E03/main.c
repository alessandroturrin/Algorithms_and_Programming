#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"

typedef enum {elenco_ordine,vertici_adiacenti,generare_lista,fine,errore} comando_t;

char *tab[] = {"elenco_ordine","vertici_adiacenti","genera_lista","fine"}; //tabella comandi

comando_t eseguiMenu();
comando_t selezionaComando();
int main(int argc, const char * argv[])
{
    FILE* fin;
    comando_t comando;
    Graph G;
    
    if(argc!=2)
    {
        printf("Errore linea di comando!\n");
        exit(EXIT_FAILURE);
    }
    if((fin = fopen(argv[1], "rt"))==NULL) //sostituire nome con 'argv[1]
    {
        printf("Errore nell'apertura di '%s'!\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    G = GRAPHload(fin);
    
    do
    {
        comando = eseguiMenu();
        switch (comando)
        {
            case elenco_ordine:
                verticiOrdinati(G);
                break;
            case vertici_adiacenti:
                if(!sottoGrafo(G))
                    printf("I 3 vertici non formano un sottografo completo!\n");
                else
                    printf("I 3 vertici formano un sottografo completo!\n");
                break;
            case generare_lista:
                madjToLadj(G);
                break;
            case fine:
                break;
            case errore:
                break;
        }
    }while(comando!=fine);
    
    GRAPHfree(G);
}

comando_t eseguiMenu()
{
    printf("---MENU---\n");
    for(int i=elenco_ordine;i<errore;i++)
        printf("-[%s]\n", tab[i]);
    return selezionaComando();
}

comando_t selezionaComando()
{
    char com[maxChar+1];
    
    printf("Comando: ");
    scanf("%s", com);
    for(int i=elenco_ordine;i<errore;i++)
    {
        if(strcmp(tab[i], com)==0)
            return i;
    }
    return errore;
}
