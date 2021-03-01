#include "personaggi.h"

//funzioni e procedure
comando_c selezionaComando(void);
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
            case ricerca_personaggio:
                ricercaPersonaggio(tabPg);
                break;
            case stampa_personaggio:
                stampaPersonaggioEquipaggiamento(tabPg);
                break;
            case modificare_equipaggiamento:
                scelta = sceltaEquipaggiamento();
                if(scelta==1) aggiuntaEquipaggiamento(tabPg, tabInv);
                if(scelta==2) rimozioneEquipaggiamento(tabPg, tabInv);
                if(scelta==-1) printf("\nErrore nell'inserimento del comando!\n\n");
                break;
            case stampa_oggetto:
                stampaEquipaggiamento(tabInv);
                break;
            case calcolare_statistiche:
                calcolaStatistiche(tabPg);
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
    char *tab[] = {"inserire_personaggi","inserire_oggetti","aggiungere_personaggio","eliminare_personaggio","ricerca_personaggio","stampa_personaggio","modificare_equipaggiamento","stampa_oggetto","calcolare_statistiche","fine"}; //tabella comandi
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


