#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"

void stat_read(FILE *fp, stat_t *statp)
{
    fscanf(fp, "%d%d%d%d%d%d", &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr);
}

void stat_print(FILE *fp, stat_t *statp, int soglia)
{
    fprintf(fp, " Hp:%d ", statp->hp>soglia ? statp->hp : soglia);
    fprintf(fp, "Mp:%d ", statp->mp>soglia ? statp->mp : soglia);
    fprintf(fp, "Atk:%d ", statp->atk>soglia ? statp->atk : soglia);
    fprintf(fp, "Def:%d ", statp->def>soglia ? statp->def : soglia);
    fprintf(fp, "Mag:%d ", statp->mag>soglia ? statp->mag : soglia);
    fprintf(fp, "Spr:%d\n", statp->spr>soglia ? statp->spr : soglia);
}

void inv_read(FILE *fp, inv_t *invp)
{
    fscanf(fp, "%s%s", invp->nome, invp->tipo);
    stat_read(fp, &invp->stat);
}

void inv_print(FILE *fp, inv_t *invp)
{
    fprintf(fp, "%s %s", invp->nome, invp->tipo);
    stat_print(fp, &invp->stat, -10000);
}

stat_t inv_getStat(inv_t *invp)
{
    return invp->stat;
}
