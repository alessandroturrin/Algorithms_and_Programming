#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DD_global 10
#define DP_global 20

#define maxChar 100
#define nomeFile "elementi.txt"
#define nDiagonali 5
#define nDiagSol 3

typedef struct
{
    char nome[maxChar+1];
    int tipo, dIngresso, dUscita, precedenza, finale, difficolta;
    float valore;
} elemento_t;

typedef struct
{
    elemento_t diag[nDiagonali];
    float valore;
    int difficolta, length;
} diagonale_t;

elemento_t *inputFile(int *dim);
void wrapper(elemento_t e[], int dim);
void powerSet(elemento_t e[], diagonale_t **d, elemento_t sol[], int n, int N, int *maxN, int *count, int pos, int start, int DD, int dd);
int check(elemento_t e, elemento_t sol[], int pos, int n);
int elementoAcrobatico(elemento_t sol[], int pos);
void wrapperDue(diagonale_t *d, int N,  int DD, int DP);
void disp(diagonale_t *val, diagonale_t sol[], diagonale_t bestSol[], int pos, int N, int DP, float dp, float pt, float *bdp, int *bonus);
int checkProgramma(diagonale_t *sol, int *bonus);
void stampaProgramma(diagonale_t *sol, int DD, int DP, float dp, int bonus);
int main()
{
    elemento_t *e;
    int dim;
    
    e = inputFile(&dim);
    wrapper(e, dim);
}

elemento_t *inputFile(int *dim)
{
    FILE *file = fopen(nomeFile, "r");
    elemento_t *e;
    
    if(file==NULL)
    {
        printf("Errore nell'apertura di '%s'!\n", nomeFile);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", dim);
    e = (elemento_t*)malloc((*dim)*sizeof(elemento_t));
    for(int i=0;i<*dim;i++)
        fscanf(file, "%s%d%d%d%d%d%f%d\n", e[i].nome, &e[i].tipo, &e[i].dIngresso, &e[i].dUscita, &e[i].precedenza, &e[i].finale, &e[i].valore, &e[i].difficolta);
    fclose(file);
    return e;
}

void wrapper(elemento_t e[], int dim)
{
    elemento_t sol[nDiagonali];
    diagonale_t *d = (diagonale_t*)malloc(nDiagonali*sizeof(diagonale_t));
    int maxN = nDiagonali, count=0, DD=DD_global, DP=DP_global, dd=0;

    for(int n=1;n<=nDiagonali;n++)
        powerSet(e, &d, sol, n, dim, &maxN, &count, 0, 0, DD, dd);
    wrapperDue(d, count, DD, DP);
}

void powerSet(elemento_t e[], diagonale_t **d, elemento_t sol[], int n, int N, int *maxN, int *count, int pos, int start, int DD, int dd)
{
    if(pos>=n)
    {
        if(elementoAcrobatico(sol, pos))
        {
            if(*count==*maxN)
            {
                *maxN = 2*(*maxN);
                *d =realloc(*d, (*maxN)*sizeof(diagonale_t));
            }
            (*d)[*count].valore = 0.0;
            (*d)[*count].difficolta = dd;
            (*d)[*count].length = pos;
            for(int k=0;k<n;k++)
            {
                (*d)[*count].diag[k] = sol[k];
                (*d)[*count].valore += sol[k].valore;
            }
            *count = *count + 1;
        }
        return;
    }
    
    for(int i=start;i<N;i++)
    {
        if(check(e[i], sol, pos, n-1) && (dd+e[i].difficolta)<=DD) //sostituire con check
        {
            sol[pos] = e[i];
            dd += e[i].difficolta;
            powerSet(e, d, sol, n, N, maxN, count, pos+1, start, DD, dd);
            dd -= e[i].difficolta;
        }
    }
    return;
}

int check(elemento_t e, elemento_t sol[], int pos, int n)
{
    
    if(pos==0)
    {
        if(e.precedenza==0 && e.dIngresso==1) return 1;
        return 0;
    }

    if(e.finale==1 && pos!=n) return 0;
    if(e.dIngresso!=sol[pos-1].dUscita) return 0;
    return 1;
}

int elementoAcrobatico(elemento_t sol[], int pos)
{
    for(int i=0;i<pos;i++)
        if(sol[i].tipo==1 || sol[i].tipo==2) return 1;
    return 0;
}

void wrapperDue(diagonale_t *d, int N, int DD, int DP)
{
    int bonus=0;
    float dp=0, bdp=0, pt=0;
    diagonale_t sol[nDiagSol], bestSol[nDiagonali];
    
    for(int i=0;i<nDiagSol;i++) bestSol[i].valore = 0;
    disp(d, sol, bestSol, 0, N, DP, dp, pt, &bdp, &bonus);
    stampaProgramma(bestSol, DD, DP, bdp, bonus);
}

void disp(diagonale_t *val, diagonale_t sol[], diagonale_t bestSol[], int pos, int N, int DP, float dp, float pt, float *bdp, int *bonus)
{
    if(pos>=nDiagSol)
    {
        int b;
        if(checkProgramma(sol, &b))
        {
            if(b)
                pt+=sol[nDiagSol-1].diag[sol[nDiagSol-1].length-1].valore*0.5;
            if(pt>*bdp)
            {
                for(int i=0;i<nDiagSol;i++)
                {
                    bestSol[i] = sol[i];
                    *bdp = pt;
                }
                if(b) *bonus=1;
                else *bonus=0;
            }
        }
        return;
    }
    
    for(int i=0;i<N;i++)
    {
        if(dp+val[i].difficolta<=DP)
        {
            sol[pos]=val[i];
            dp+=(float)val[i].difficolta;
            pt+=val[i].valore;
            disp(val, sol, bestSol, pos+1, N, DP, dp, pt, bdp, bonus);
            dp-=(float)val[i].difficolta;
            pt-=val[i].valore;
        }
    }
}

int checkProgramma(diagonale_t *sol, int *bonus)
{
    int avanti=0, indietro=0, acrobatico=0, index;
    
    
    for(int i=0;i<nDiagSol;i++)
    {
        for(int j=0;j<sol[i].length;j++)
        {
            if(j>0 && sol[i].diag[j].tipo>0 && sol[i].diag[j-1].tipo>0)
                acrobatico=1;
            if(sol[i].diag[j].tipo==1) indietro=1;
            if(sol[i].diag[j].tipo==2) avanti=2;
        }
    }
    if(avanti==0 || indietro==0 || acrobatico==0)
        return 0;
    index = sol[nDiagSol-1].length-1;
    if(sol[nDiagSol-1].diag[index].valore>=8)
        *bonus = 1;
    else
        *bonus = 0;
    return 1;
}

void stampaProgramma(diagonale_t *sol, int DD, int DP, float dp, int bonus)
{
    float bns=sol[nDiagSol-1].valore*0.5;
 
    dp=0.0;
    for(int i=0;i<nDiagSol;i++)
        dp+=sol[i].valore;
    if(bonus)
        dp+=bns;
    
    printf("---Test Case---\n");
    printf("DD = %d DP = %d\n", DD, DP);
    printf("TOT = %.3f\n", dp);
    for(int i=0;i<nDiagSol;i++)
    {
        printf("DIAGO #%d > %.3f", i+1, sol[i].valore);
        if(i==nDiagSol-1 && bonus)
            printf(" * 1.5 (BONUS)\n");
        else
            printf("\n");
        for(int j=0;j<sol[i].length;j++)
        printf("%s ", sol[i].diag[j].nome);
        printf("\n");
    }
    printf("\n\n");
}
