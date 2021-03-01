#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "graph.h"

typedef enum {carica_grafo, svolgi, stampa, fine, errore} comando_t;
char *tabellaComandi[] = {"Carica_grafo", "Svolgi", "Stampa", "Fine"};

comando_t leggiComando();
int main()
{
    FILE *fp;
    Graph G;
    comando_t comando;
    int esegui = 1;
    char nomeFile[30];
    
    printf("Nome file: ");
    scanf("%s", nomeFile);
    if((fp = fopen(nomeFile, "rt"))==NULL)
    {
        printf("Errore nell'apertura di '%s'\n", nomeFile);
        exit(EXIT_FAILURE);
    }
    G = GRAPHload(fp);
    fclose(fp);
    
    while(esegui)
    {
        comando = leggiComando();
        
        switch(comando)
        {
            case carica_grafo:
                free(G);
                printf("[Grafo precedente deallocato correttamente]\nNome file: ");
                scanf("%s", nomeFile);
                if((fp = fopen(nomeFile, "rt"))==NULL)
                {
                    printf("Errore nell'apertura di '%s'\n", nomeFile);
                    exit(EXIT_FAILURE);
                }
                G = GRAPHload(fp);
                fclose(fp);
                break;
            case svolgi:
                DAG(G);
                break;
            case stampa:
                GRAPHdisplay(G);
                break;
            case fine:
                esegui = 0;
                break;
            default:
                break;
        }
    }
}

comando_t leggiComando()
{
    int comando;
    for(int i=carica_grafo;i<errore;i++)
        printf("[%d] - %s\n", i+1, tabellaComandi[i]);
    printf("\nComando: ");
    scanf("%d", &comando);
    comando--;
    if(comando>=0 && comando<errore)
        return comando;
    else
        return errore;
}
