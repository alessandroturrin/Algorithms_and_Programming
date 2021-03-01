#include <stdio.h>
#include <stdlib.h>

#define nomeFile "hard_test_set.txt"
#define numeroPietre 4

typedef enum {Z,R,T,S_t} gemme_t;
typedef enum {empty, full} check_t;

void printInizio(int nGemme[], int nGemmeTot, int caso);
int wrapperSol(int z, int r, int t, int s, int nGemmeTot);
int *****mallocS(int z, int r, int t, int s);
int fz(int *****S, int z, int r, int t, int s);
int fr(int *****S, int z, int r, int t, int s);
int ft(int *****S, int z, int r, int t, int s);
int fs(int *****S, int z, int r, int t, int s);
int max(int a, int b);
void freeS(int *****S, int z, int r, int t, int s);
int main()
{
    FILE* file = fopen(nomeFile, "rt");
    int dimFile, nGemme[numeroPietre], nGemmeTot;
    
    if(file==NULL)
    {
        printf("Errore nell'apertura di '%s'\n", nomeFile);
        exit(EXIT_FAILURE);
    }
    fscanf(file, "%d", &dimFile);
    for(int i=0;i<dimFile;i++)
    {
        nGemmeTot = 0;
        for(int j=0;j<numeroPietre;j++)
        {
            fscanf(file, "%d", &nGemme[j]);
            nGemmeTot+=nGemme[j];
        }
        printInizio(nGemme, nGemmeTot, i+1);
        nGemmeTot = wrapperSol(nGemme[Z], nGemme[R], nGemme[T], nGemme[S_t], nGemmeTot);
        printf("Lunghezza massima: %d\n\n", nGemmeTot);
    }
    
}

void printInizio(int nGemme[], int nGemmeTot, int caso)
{
    printf("TEST #%d\nZaffiro=%d Rubino=%d Topazio=%d Smeraldo=%d\tTotale=%d\n", caso, nGemme[Z], nGemme[R], nGemme[T], nGemme[S_t], nGemmeTot);
}

int wrapperSol(int z, int r, int t, int s, int nGemmeTot)
{
    int *****S;
    int solZ, solR, solT, solS;
    
    S = mallocS(z, r, t, s);
    
    solZ = fz(S, z, r, t, s);
    solR = fr(S, z, r, t, s);
    solT = ft(S, z, r, t, s);
    solS = fs(S, z, r, t, s);
    
    free(S);
    return max(solZ, max(solR, max(solT, solS)));
}

int *****mallocS(int z, int r, int t, int s)
{
    int *****S;
    int h, i, j, k, l;
    
    S = (int*****)malloc((z+1)*sizeof(int****));
    
    for(h=0;h<=z;h++)
    {
        S[h] = (int****)malloc((r+1)*sizeof(int***));
        for(i=0;i<=r;i++)
        {
            S[h][i] = (int***)malloc((t+1)*sizeof(int**));
            for(j=0;j<=t;j++)
            {
                S[h][i][j] = (int**)malloc((s+1)*sizeof(int*));
                for(k=0;k<=s;k++)
                {
                    S[h][i][j][k] = (int*)malloc(numeroPietre*sizeof(int));
                    for(l=0;l<numeroPietre;l++)
                    S[h][i][j][k][l] = empty;
                }
            }
        }
    }
    return S;
}

int fz(int *****S, int z, int r, int t, int s)
{
    if(z<=0) return 0;
    
    if(S[z][r][t][s][Z]!=empty) return S[z][r][t][s][Z];
    
    S[z][r][t][s][Z]=1+max(fz(S,z-1,r,t,s),fr(S,z-1,r,t,s));
    
    return  S[z][r][t][s][Z];
}

int fr(int *****S, int z, int r, int t, int s)
{
    if(r<=0) return 0;
   
    if(S[z][r][t][s][R]!=empty) return S[z][r][t][s][R];
    
    S[z][r][t][s][R]=1+max(ft(S,z,r-1,t,s),fs(S,z,r-1,t,s));
    
    return  S[z][r][t][s][R];
}

int ft(int *****S, int z, int r, int t, int s)
{
    if(t<=0) return 0;
   
    if(S[z][r][t][s][T]!=empty) return S[z][r][t][s][T];
      
    S[z][r][t][s][T]=1+max(fz(S,z,r,t-1,s),fr(S,z,r,t-1,s));

    return  S[z][r][t][s][T];
}

int fs(int *****S, int z, int r, int t, int s)
{
    if(s<=0) return 0;
   
    if(S[z][r][t][s][S_t]!=empty) return S[z][r][t][s][S_t];

    S[z][r][t][s][S_t]=1+max(ft(S,z,r,t,s-1),fs(S,z,r,t,s-1));

    return  S[z][r][t][s][S_t];
}

int max(int a, int b)
{
    if(a>b) return a;
    else return b;
}

void freeS(int *****S, int z, int r, int t, int s)
{
    int h, i, j, k;
    
    for(h=0;h<=z;h++)
    {
        for(i=0;i<=r;i++)
        {
            for(j=0;j<=t;j++)
            {
                for(k=0;k<=s;k++)
                    free(S[i][i][j][k]);
                
                free(S[h][i][j]);
            }
            free(S[h][i]);
        }
        free(S[h]);
    }
    free(S);
}

