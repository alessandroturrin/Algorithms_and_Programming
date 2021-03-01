#include <stdio.h>
#include <stdlib.h>

#define max 50 //dimensioni massime matrice

void leggiMatrice(int matrix[][max], char fileName[], int* nr, int* nc); //input di dati da file
int riconosciRegione(int matrix[][max], int nr, int nc, int r, int c, int *b, int *h);
int main()
{
    char fileName[] = "mat.txt"; //nome file di input
    int matrix[max][max]; //matrice contenente dati mappa
    int nr, nc; //numero di righe e colonne della matrice
    int r, c; //riga e colonna di interesse
    int b, h; //base e altezza
    
    leggiMatrice(matrix,fileName,&nr,&nc);
    
    for(r=0;r<nr;r++)
        for(c=0;c<nc;c++)
        {
            b=0;
            h=0;
            if(riconosciRegione(matrix, nr, nc, r, c, &b, &h)==1)
                printf("Estremo sup. SX=<%d,%d>\tb=%d h=%d A=%d\n", r, c, b, h, b*h);
        }
}

void leggiMatrice(int matrix[][max], char fileName[], int* nr, int* nc)
{
    FILE* file; //file di input
    
    if((file=fopen(fileName, "rt"))==NULL) //apertura e controllo file di input(mappa.txt)
    {
        printf("Errore nell'apertura di '%s'\n!", fileName);
        exit(-1);
    }
    
    fscanf(file, "%d %d", &(*nr), &(*nc)); //input nr e nc
    
    for(int i=0;i<*nr;i++)
        for(int j=0;j<*nc;j++)
            fscanf(file, "%d", &matrix[i][j]);
}

int riconosciRegione(int matrix[][max], int nr, int nc, int r, int c, int *b, int *h)
{
    int calcola = 0;
    //possibili casi di r e c
    if(r>0 && c>0)
    {
        if(matrix[r][c]==1 && matrix[r-1][c]==0 && matrix[r][c-1]==0)
            calcola = 1;
    }
    else
        if(matrix[r][c]==1 && r==0)
        {
            if(matrix[r][c-1]==0)
                calcola = 1;
        }
        else if(matrix[r][c]==1 && c==0)
            if(matrix[r-1][c]==0)
                calcola = 1;
    
    if(calcola)
    {
        (*h) = 1;
        int lungB; //ausilio per trovare altezza
        
        while(matrix[r][c]!=0 && r<nr && c<nc) //primo while per lunghezza base
        {
            if(matrix[r][c]==1)
            {
                *b = *b + 1;
                c++;
            }
        }
        
        c = c-(*b); //ripristino posizione colonna
        
        while(matrix[r][c]!=0 && r<nr && c<nc)
        {
            r+=1;
            lungB=0;
            for(int j=c;j<(c+(*b));j++)
                if(matrix[r][c]==1)
                    lungB++;
            
            lungB==(*b) ? ((*h) = (*h) + 1) : (r = nr); //incremento altezza
        }
        return 1;
    }
    else
        return 0;
}
