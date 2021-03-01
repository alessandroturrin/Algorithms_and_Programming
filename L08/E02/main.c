#include <stdio.h>
#include <stdlib.h>

#define nomeFile "/Users/Ale/Desktop/test_set.txt"
#define numeroPietre 4

typedef enum {zaffiro, rubino, topazio, smeraldo} pietra;

int permRip(int pos, int *val, int *sol, int *mark, int n, int l, int *bestsol, int *bl, int *trovato, int *vg, int rip, int mr, int *bv, int value, int ns, int nz);
int checkVal(int *sol, int pos, int val, int *n);
int contrRep(int *sol, int rip, int max_rip, int val, int pos);
int leggiDaFile(FILE *fp, int *n, int *v, int *r);
void printfStart(int i, int nPietre[], int vPietre[], int maxLength, int maxRip);
void printEnd(int vMax, int lMax, int bestSol[]);

int main()
{
    int i, maxLength, nCasi, *val, *sol, *bestSol;
    int nPietre[numeroPietre], vPietre[numeroPietre];
    int maxRip, lMax, trovato, vMax;
    FILE* file = fopen(nomeFile, "rt");
    
    if(file==NULL)
    {
        printf("Errore nell'apertura del file '%s'!\n", nomeFile);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", &nCasi);
    
    val=malloc(numeroPietre*sizeof(int));
    
    for(i=0;i<numeroPietre;i++) val[i]=i;
    for(i=0;i<nCasi;i++)
    {
        lMax = trovato = vMax = 0;
        maxLength = leggiDaFile(file, nPietre, vPietre, &maxRip);
        sol=(int*)malloc(maxLength*sizeof(int));
        bestSol=(int*)malloc(maxLength*sizeof(int));
        printfStart(i, nPietre, vPietre, maxLength, maxRip);
        permRip(0, val, sol, nPietre, numeroPietre, maxLength, bestSol, &lMax, &trovato, vPietre, 0 ,maxRip, &vMax, 0, 0, 0);
        printEnd(vMax, lMax, bestSol);
        free(bestSol);
        free(sol);
    }
    return 0;
}

int leggiDaFile(FILE *file, int n[], int v[], int *r){
    int lTot=0;
    for(int i=0;i<numeroPietre;i++)
    {
        fscanf(file, "%d", &n[i]);
        lTot += n[i];
    }
    for(int i=0;i<numeroPietre;i++) fscanf(file, "%d", &v[i]);
    fscanf(file, "%d", r);
    return lTot;
}

int permRip(int pos, int *val, int *sol, int *mark, int n, int l, int *bestsol, int *bl, int *trovato, int *vg, int rip, int mr, int *bv, int value, int nSmer, int nZaff){
    int i, j, sve;
    
    if(pos>=l)
    {
        *bl=pos;
        *trovato=1;
        *bv=value;
        for(i=0;i<*bl;i++)
            bestsol[i]=sol[i];
        return 1;
    }
    
    for(i=0;i<n;i++)
    {
        if(mark[i]>0 && checkVal(sol, pos, val[i], mark) && contrRep(sol, rip, mr, val[i], pos) && nZaff<=nSmer)
        {
            mark[i]--;
            sol[pos]=val[i];
            if (pos>0)
            {
                if(sol[pos]!=sol[pos-1])
                {
                    sve=rip;
                    rip=0;
                }
            }
            if(i==0) nZaff += 1;
            if(i==3) nSmer += 1;
            permRip(pos+1, val, sol, mark, n, l, bestsol, bl, trovato, vg, rip+1, mr, bv, value+vg[i], nSmer, nZaff);
            if(*trovato)
                return 1;
            rip=sve;
            mark[i]++;
            if(i==0) nZaff -= 1;
            if(i==3) nSmer -= 1;
        }
        else
        {
            if(*bv<value && nZaff<=nSmer)
            {
                *bv=value;
                *bl=pos;
                for(j=0;j<*bl;j++)
                    bestsol[j]=sol[j];
            }
        }
    }
    return 1;
}

int checkVal(int *sol, int pos, int val, int *n){
    int fine=0;
    
    if(pos==0) return 1;
    switch(val){
        case zaffiro:
            if(sol[pos-1]==zaffiro || sol[pos-1]==topazio) fine=1;
            if((n[rubino]==0 && n[zaffiro]==1) || (n[topazio]==0 && n[rubino]==1)) return 0;
            break;
        case smeraldo:
            if(sol[pos-1]==smeraldo || sol[pos-1]==rubino) fine=1;
            if((n[smeraldo]==0 && n[topazio]==1) || (n[topazio]==0 && n[smeraldo]==1)) return 0;
            break;
        case rubino:
            if(sol[pos-1]==zaffiro || sol[pos-1]==topazio) fine=1;
            if((n[smeraldo]==0 && n[topazio]==1) || (n[topazio]==0 && n[smeraldo]==1)) return 0;
            break;
        case topazio:
            if(sol[pos-1]==smeraldo || sol[pos-1]==rubino) fine=1;
            if((n[rubino]==0 && n[zaffiro]==1) || (n[topazio]==0 && n[rubino]==1)) return 0;
            break;
        default:
            break;
    }
    return fine;
}

int contrRep(int *sol, int rip, int max_rip, int val, int pos){
    if (pos==0) return 1;
    if(sol[pos-1]!=val || rip+1<=max_rip)
        return 1;
    return 0;
}

void printfStart(int i, int nPietre[], int vPietre[], int maxLength, int maxRip)
{
    printf("TEST #%d\nzaffiro=%d[%d], rubino=%d[%d], topazio=%d[%d], smeraldo=%d[%d], TOT=%d, {max_rip=%d}\n", i+1, nPietre[zaffiro], vPietre[zaffiro], nPietre[rubino], vPietre[rubino], nPietre[topazio], vPietre[topazio], nPietre[smeraldo], vPietre[smeraldo], maxLength, maxRip);
}

void printEnd(int vMax, int lMax, int bestSol[])
{
    printf("Soluzione ottima di valore %d usando %d gemma/e\nComposizione collana: ", vMax, lMax);
    for(int i=0;i<lMax;i++)
    {
        switch(bestSol[i]){
            case 0: printf("z"); break;
            case 1: printf("r"); break;
            case 2: printf("t"); break;
            case 3: printf("s"); break;
            default: break;
        }
    }
    printf("\n\n");
}
