#include <stdlib.h>
#include <stdio.h>

#define fileTiles "tiles.txt"
#define fileBoard "board.txt"

typedef enum {false, true} bool;

typedef struct
{
    char coloreT1, coloreT2, usato;
    int valoreT1, valoreT2;
}tessera;

typedef struct
{
    int i, r;
}scacchiera;

void dispRipWrapper(tessera *t, scacchiera *s, int *o, int T, int R, int C);
void disposizioni(tessera *t, scacchiera *s, scacchiera *bestSol, int *o, int *bestSum, int pos, int T, int R, int C);
int calc_punt(scacchiera *s, tessera *t, int nr, int nc);
void stampa(int bestSum, scacchiera *bestSol, int R, int C);
int main()
{
    tessera *t; //vettore di struct per le coppie di tessere
    scacchiera *s; //vettore di struct contenente valori di scacchiera
    int *iniz; //vettore per verificare se usato o meno
    int T, R, C; //numero di tessere, righe, colonne
    
    FILE* fTiles = fopen(fileTiles, "rt");
    FILE* fBoard = fopen(fileBoard, "rt");
    
    if(fTiles==NULL || fBoard==NULL)
    {
        fTiles==NULL ? printf("Errore nell'apertura di '%s'!\n", fileTiles) : printf("Errore nell'apertura di '%s'!\n", fileBoard);
        exit(1);
    }
    
    fscanf(fTiles, "%d", &T);
    //allocazione e input di t da fTiles
    t = (tessera*)malloc(T*sizeof(tessera));
    for(int i=0;i<T;i++)
        fscanf(fTiles, " %c %d %c %d", &t[i].coloreT1, &t[i].valoreT1, &t[i].coloreT2, &t[i].valoreT2);
    
    fscanf(fBoard, "%d %d", &R, &C);
    //allocazione di mark
    iniz = (int*)malloc(R*C*sizeof(int));
    //allocazione e input di scacchiera da fBoard
    s = (scacchiera*)malloc(R*C*sizeof(scacchiera*));
    for(int i=0;i<R*C;i++)
    {
        fscanf(fBoard,"%d/%d", &s[i].i, &s[i].r);
        if(s[i].i==-1)
            iniz[i] = 0;
        else
        {
            iniz[i] = 1;
            t[s[i].i].usato = 1;
        }
    }
    //chiusura file
    fclose(fTiles);
    fclose(fBoard);
    
    dispRipWrapper(t, s, iniz, T, R, C);
}

void dispRipWrapper(tessera *t, scacchiera *s, int *o, int T, int R, int C)
{
    scacchiera *bestSol; //soluzione migliore
    int bestSum = 0; //somma migliore
    
    bestSol = (scacchiera*)malloc(R*C*sizeof(scacchiera));
    for(int i=0;i<R*C;i++)
        if(o[i]==1)
            bestSol[i] = s[i]; //deve necessariamente rispettare schema iniziale
        else
        {
            bestSol[i].i = -1;
            bestSol[i].r = -1;
        }

    disposizioni(t, s, bestSol, o, &bestSum, 0, T, R, C);
    stampa(bestSum, bestSol, R, C);
}

void disposizioni(tessera *t, scacchiera *s, scacchiera *bestSol, int *o, int *bestSum, int pos, int T, int R, int C)
{
    int i, somma;
    
    if(pos>=R*C)
    {
        somma = calc_punt(s, t, R, C);
        //somma = calcolaSomma(s, t, R, C);
        if(somma>*bestSum)
        {
            *bestSum = somma;
            for(i=0;i<R*C;i++)
                bestSol[i] = s[i];
        }
        return;
    }
    
    if(o[pos]==1) //caso tessera gia utilizzata, passo a pos successiva
        disposizioni(t, s, bestSol, o, bestSum, pos+1, T, R, C);
    else
    {
        for(i=0;i<T;i++)
        {
            if(s[pos].i<0 && !t[i].usato)
            {
                s[pos].i = i;
                s[pos].r = 0; //non ruotata
                t[i].usato = 1;
                disposizioni(t, s, bestSol, o, bestSum, pos+1, T, R, C);
                s[pos].r = 1; //ruotata
                disposizioni(t, s, bestSol, o, bestSum, pos+1, T, R, C);
                s[pos].i = -1; //smarco i
                s[pos].r = -1;
                t[i].usato = 0;
            }
        }
    }
}

int calc_punt(scacchiera *s, tessera *t, int nr, int nc){
    int i, j, k;
    int indice, val=0, tot=0;
    char c;
    
    for(i=0;i<nr;i++)
    {
        indice=s[i*nr].i;
        if(s[i*nr].r==0)
        {
            c=t[indice].coloreT1;
            val=t[indice].valoreT1;
        }
        else
        {
            c=t[indice].coloreT2;
            val=t[indice].valoreT2;
        }
        for(j=1;j<nc;j++)
        {
            k=i*nr+j;
            indice=s[k].i;
            if(s[k].r==0)
            {
                if(t[indice].coloreT1==c)
                    val+=t[indice].valoreT1;
                else
                {
                    val=0;
                    j=nc;
                }
            }
            else
            {
                if(t[indice].coloreT2==c)
                    val+=t[indice].valoreT2;
                else
                {
                    val=0;
                    j=nc;
                }
            }
        }
        tot+=val;
        val=0;
    }

    for(j=0;j<nc;j++)
    {
        if(s[j].r==0)
        {
            indice=s[j].i;
            c=t[indice].coloreT2;
            val=t[indice].valoreT2;
        }
        else
        {
            c=t[indice].coloreT1;
            val=t[indice].valoreT1;
        }
        for(i=1;i<nr;i++)
        {
            k=i*nr+j;
            indice=s[k].i;
            if(s[k].r==0){
                if(t[indice].coloreT2==c)
                    val+=t[indice].valoreT2;
                else
                {
                    val=0;
                    i=nr;
                }
            }
            else
            {
                if(t[indice].coloreT1==c)
                    val+=t[indice].valoreT1;
                else
                {
                    val=0;
                    i=nr;
                }
            }
        }
        tot+=val;
        val=0;
    }
    return tot;
}

void stampa(int bestSum, scacchiera *bestSol, int R, int C)
{
    printf("Somma migliore: %d\n\n", bestSum);
    for(int i=0;i<R;i++)
    {
        for(int j=0;j<C;j++)
            printf("%d/%d\t", bestSol[i*R+j].i, bestSol[i*R+j].r);
        puts("");
    }
}

