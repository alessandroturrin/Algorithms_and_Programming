#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DD_global 11
#define DP_global 33

#define maxChar 100
#define nomeFile "elementi.txt"
#define nDiagonali 5
#define nDiagSol 3

#define DBG 0 //0 no debug | 1 debug

typedef enum {false, true} bool;
typedef enum {valore, difficolta} chiave_t;
typedef enum {transizione_t, indietro_t, avanti_t} tipo_t;

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
void SelectionSort(elemento_t A[], int N, chiave_t key); //ordino da valore piu grande a valore piu piccolo
bool isBigger(elemento_t v1, elemento_t v2, chiave_t key);
void greedy(elemento_t e[], int N, int DD, int DP);
int check(elemento_t e, elemento_t sol[], int pos, int dd, int DD, int dp);
int generaVettoreIndex(elemento_t v[], elemento_t e[], int N, tipo_t key);
void stampaProgramma(diagonale_t *sol, int DD, int DP, float dp, int bonus);
int main()
{
    elemento_t *e;
    int dim;
    int DD = DD_global;
    int DP = DP_global;
    
    e = inputFile(&dim);
    SelectionSort(e, dim, difficolta);
    
    #if DBG
    for(int i=0;i<dim;i++)
        printf("%s %d %d %d %d %d %f %d\n", e[i].nome, e[i].tipo, e[i].dIngresso, e[i].dUscita, e[i].precedenza, e[i].finale, e[i].valore, e[i].difficolta);
    #endif
    
    greedy(e, dim, DD, DP);
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

void SelectionSort(elemento_t A[], int N, chiave_t key)
{
    int i, j, l=0, r=N-1, min;
    elemento_t temp;
    for(i = l; i < r; i++)
    {
        min = i;
        for (j = i+1; j <= r; j++)
            if (isBigger(A[j],A[min],key))
                min = j;
        if (min != i)
        {
            temp = A[i];
            A[i] = A[min];
            A[min] = temp;
        }
    }
    return;
}

bool isBigger(elemento_t v1, elemento_t v2, chiave_t key)
{
    if(key==valore)
        return v1.valore>v2.valore;
    else
        return v1.difficolta<v2.difficolta;
}

void greedy(elemento_t e[], int N, int DD, int DP)
{
    elemento_t s[nDiagonali];
    diagonale_t sol[nDiagSol];
    int i=0, pos, dd, dp=DP;
    int avanti=0, indietro=0, consecutive=0, selection = 0, elementoAcrobatico;
    
    elemento_t *vettoreElementi = (elemento_t*)malloc(N*sizeof(elemento_t));
    int dimVett = generaVettoreIndex(vettoreElementi, e, N, avanti_t);
    
    SelectionSort(vettoreElementi, dimVett, difficolta);
    
    for(int nDiag=0;nDiag<nDiagSol;nDiag++)
    {
        dd = pos = elementoAcrobatico = 0;
        
        if(nDiag==0)
        {
            for(i=0;i<dimVett;i++)
            {
                if(check(vettoreElementi[i], s, pos, dd, DD, dp))
                {
                    dd += vettoreElementi[i].difficolta;
                    dp -= vettoreElementi[i].difficolta;
                    s[pos] = vettoreElementi[i];
                    
                    if(s[pos].tipo==avanti_t) avanti = 1;
                    if(s[pos].tipo==indietro_t) indietro = 1;
                    if(pos>0 && (s[pos].tipo*s[pos-1].tipo)!=0) consecutive = 1;
                    pos++;
                    elementoAcrobatico = 1;
                }
                if(dd>=DD || dp<0 || pos>=nDiagonali)
                    break;
            }
        }

        if(indietro==0)
        {
            for(i=0;i<dimVett;i++)
            {
                if(vettoreElementi[i].tipo==indietro_t && check(vettoreElementi[i], s, pos, dd, DD, dp))
                {
                    dd += vettoreElementi[i].difficolta;
                    dp -= vettoreElementi[i].difficolta;
                    s[pos] = vettoreElementi[i];
                    pos++;
                    elementoAcrobatico = 1;
                    indietro = 1;
                }
            }
        }
        
        if((avanti*indietro*consecutive)!=0 && selection==0)
        {
            SelectionSort(e, N, valore);
            selection = 1;
        }
        
        i = 0;
        while (elementoAcrobatico==0)
        {
            if(check(vettoreElementi[i], s, pos, dd, DD, dp))
            {
                dd += vettoreElementi[i].difficolta;
                dp -= vettoreElementi[i].difficolta;
                s[pos] = vettoreElementi[i];
                pos++;
                elementoAcrobatico = 1;
            }
            i++;
        }
        for(i=0;i<N;i++)
        {
            if(check(e[i], s, pos, dd, DD, dp))
            {
                dd += e[i].difficolta;
                dp -= e[i].difficolta;
                s[pos] = e[i];
                
                if(s[pos].tipo==avanti_t) avanti = 1;
                if(s[pos].tipo==indietro_t) indietro = 1;
                if(pos>0 && (s[pos].tipo*s[pos-1].tipo)!=0) consecutive = 1;
                pos++;
            }
        }
        
        sol[nDiag].length = pos;
        sol[nDiag].valore = 0;
        sol[nDiag].difficolta = dd;
        for(int k=0;k<sol[nDiag].length;k++)
        {
            sol[nDiag].diag[k] = s[k];
            sol[nDiag].valore += s[k].valore;
        }
    }
    int bonus=0;
    if(sol[nDiagSol-1].diag[sol[nDiagSol-1].length-1].difficolta>=8) bonus=1;
    stampaProgramma(sol, DD, DP, dp, bonus);
    free(e);
    free(vettoreElementi);
}

int generaVettoreIndex(elemento_t v[], elemento_t e[], int N, tipo_t key)
{
    int dim = 0;
    if(key==avanti_t || key==indietro_t)
        for(int i=0;i<N;i++)
        {
            if(e[i].tipo>0)
                v[dim++] = e[i];
        }
    if(key==transizione_t) //lo uso solo per elemento all'indietro
        for(int i=0;i<N;i++)
        {
            if(e[i].tipo==0 && e[i].dUscita==0)
                v[dim++] = e[i];
        }
    return dim+1;
}

int check(elemento_t e, elemento_t sol[], int pos, int dd, int DD, int dp)
{
    
    if(pos>=nDiagonali) return 0;
    if(pos==0)
    {
        if(e.precedenza==0 && e.dIngresso==1) return 1;
        return 0;
    }

    if(e.finale==1 && pos==0) return 0; //verifica questo caso
    if(e.dIngresso!=sol[pos-1].dUscita) return 0;
    if(e.difficolta+dd>DD) return 0;
    if(e.difficolta+dd>dp) return 0;
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
        printf("DIAG #%d > %.3f", i+1, sol[i].valore);
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

