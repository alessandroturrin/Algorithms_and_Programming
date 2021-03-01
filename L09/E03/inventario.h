#ifndef inventario_h
#define inventario_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum {inserire_personaggi,inserire_oggetti,aggiungere_personaggio,eliminare_personaggio, ricerca_personaggio, stampa_personaggio, modificare_equipaggiamento, stampa_oggetto, calcolare_statistiche,fine,errore} comando_c;
typedef enum {false,true} bool;

#define maxLength 52
#define dimEquip 8
//struct statistiche
typedef struct
{
    int hp, mp, atk, def, mag, spr;
}stat_t;
//struct inventario
typedef struct
{
    char *nome;
    char *tipo;
    stat_t stat;
}inv_t;
//struct tabella inventario
typedef struct
{
    int nInv;
    inv_t **vettInv;
}tabInv_t;
//struct equipaggiamento
typedef struct
{
    int inUso;
    inv_t *vettEq[dimEquip];
}tabEquip_t;

void inserimentoInventarioFile(tabInv_t **tabInv);
int sceltaEquipaggiamento(void);
inv_t *ricercaEquipaggiamento(tabInv_t* tabInv, char *equipaggiamento);
void stampaEquipaggiamento(tabInv_t* tabInv);

#endif /* inventario_h */
