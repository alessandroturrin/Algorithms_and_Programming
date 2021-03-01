#include <stdio.h>
#include <stdlib.h>

int **malloc2dR(int *nr, int *nc, char *fileName);
void separa(int **mat, int nr, int nc, int **bianco, int **nero, int *dimBianco, int *dimNero);
void stampaVettori(int *vet, int dim);
void free2d(int **mat, int nr);
int main()
{
    char fileName[] = "mat.txt";
    int **mat, *bianco, *nero;
    int nr, nc, dimBianco, dimNero;
    
    mat = malloc2dR(&nr, &nc, fileName);
    separa(mat, nr, nc, &bianco, &nero, &dimBianco, &dimNero);
    
    printf("\nVettore di bianchi:\t");
    stampaVettori(bianco, dimBianco);
    printf("Vettore di neri: ");
    stampaVettori(nero, dimNero);
    free(bianco);
    free(nero);
    free2d(mat, nr);
}

int **malloc2dR(int *nr, int *nc, char *fileName)
{
    FILE* file;
    int **mat;
    
    if((file=fopen(fileName, "rt"))==NULL)
    {
        printf("Errore nell'apertura del file '%s'\nTermine dell'esecuzione del programma!\n", fileName);
        exit(-1);
    }
    
    fscanf(file, "%d %d", nr, nc);
    mat = (int**)malloc((*nr)*sizeof(int*));
    for(int i=0;i<(*nr);i++)
    {
        mat[i] = (int*)malloc((*nc)*sizeof(int));
        for(int j=0;j<(*nc);j++)
            fscanf(file, "%d", &mat[i][j]);
    }
    puts("Matrice presa in input");
    for(int i=0;i<(*nr);i++)
    {
        for(int j=0;j<(*nc);j++)
            printf("%d ", mat[i][j]);
        puts("");
    }
    fclose(file);
    return mat;
}

void separa(int **mat, int nr, int nc, int **bianco, int **nero, int *dimBianco, int *dimNero)
{
    int ib=-1, in=-1; //indice bianco, nero
    
    if((nr*nc)%2==0)
    {
        *dimBianco = nr*nc/2;
        *dimNero = *dimBianco;
    }
    else
    {
        *dimBianco = nr*nc/2+1;
        *dimNero = *dimBianco-1;
    }
    *bianco = (int*)malloc((*dimBianco)*sizeof(int));
    *nero = (int*)malloc((*dimNero)*sizeof(int));
    
    for(int i=0;i<nr;i++)
    {
        for(int j=0;j<nc;j++)
        {
            if((i+j)%2==0) (*bianco)[++ib] = mat[i][j];
            else (*nero)[++in] = mat[i][j];
        }
    }
}

void stampaVettori(int *vet, int dim)
{
    for(int i=0;i<dim;i++)
        printf("%d ", vet[i]);
    puts("");
}

void free2d(int **mat, int nr)
{
    for(int i=0;i<nr;i++)
        free(mat[i]);
    free(mat);
}
