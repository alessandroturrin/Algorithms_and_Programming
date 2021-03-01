#include <string.h>
#include <stdio.h>

#include "pg.h"


/* lettura e scrittura su file */
int pg_read(FILE *fp, pg_t *pgp)
{
    fscanf(fp, "%s %s %s", pgp->cod, pgp->nome, pgp->classe);
    if(feof(fp)) return 0;
    stat_read(fp, &pgp->b_stat);
    pgp->eq_stat=pgp->b_stat;
    return 1;
}

/* non essendo struct dinamica, pulisce chiamando il distruttire di equipArray */
void pg_clean(pg_t *pgp)
{
    equipArray_free(pgp->equip);
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray)
{
    fprintf(fp, "%s %s %s", pgp->cod, pgp->nome, pgp->classe);
    stat_print(fp, &pgp->b_stat, 0);
}

/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, invArray_t invArray)
{
    int i=equipArray_update(pgp->equip, invArray);
    stat_t update;
    if(i<0) return;
    if(i<100){ //persongaggio aggiunto
        update=inv_getStat(invArray_getByIndex(invArray, i));
        pgp->eq_stat.hp+=update.hp;
        pgp->eq_stat.mp+=update.mp;
        pgp->eq_stat.atk+=update.atk;
        pgp->eq_stat.def+=update.def;
        pgp->eq_stat.mag+=update.mag;
        pgp->eq_stat.spr+=update.spr;
    }
    else{ //personaggio rimosso
        i-=100;
        update=inv_getStat(invArray_getByIndex(invArray, i));
        pgp->eq_stat.hp-=update.hp;
        pgp->eq_stat.mp-=update.mp;
        pgp->eq_stat.atk-=update.atk;
        pgp->eq_stat.def-=update.def;
        pgp->eq_stat.mag-=update.mag;
        pgp->eq_stat.spr-=update.spr;
    }
    printf("%s %s %s", pgp->cod, pgp->nome, pgp->classe);
    stat_print(stdout, &pgp->eq_stat, 0);
}
