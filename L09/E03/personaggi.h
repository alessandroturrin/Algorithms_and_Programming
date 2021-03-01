//
//  personaggi.h
//  E03
//
//  Created by Alessandro Turrin on 05/12/20.
//

#ifndef personaggi_h
#define personaggi_h

#include "inventario.h"

//struct giocatore
typedef struct
{
    char *codice;
    char *nome;
    char *classe;
    tabEquip_t equip;
    stat_t stat;
}pg_t;
//lista
typedef struct nodo *link_pg;
typedef struct nodo
{
    pg_t val;
    link_pg next;
}nodoPg_t;
//struct tabella giocatori
typedef struct
{
    link_pg head;
    link_pg tail;
    int nPg;
}tabPg_t;

void inserimentoPersonaggiFile(tabPg_t **tabPg);
void listInsHead(tabPg_t** tabPg, pg_t val);
pg_t creaPersonaggio(void);
void rimozionePersonaggio(tabPg_t** tabPg);
void ricercaPersonaggio(tabPg_t* tabPg);
void stampaPersonaggioEquipaggiamento(tabPg_t* tabPg);
void aggiuntaEquipaggiamento(tabPg_t* tabPg, tabInv_t* tabInv);
void rimozioneEquipaggiamento(tabPg_t* tabPg, tabInv_t* tabInv);
void calcolaStatistiche(tabPg_t* tabPg);;
void freeLinkPg(link_pg x);

#endif /* personaggi_h */
