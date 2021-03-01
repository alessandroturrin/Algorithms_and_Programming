#include <stdio.h>
#include <stdlib.h>

#define max 20  //definizione dimensione massima

int capolista(int matrix[max][max], int riga, int colonna); //funzione che verifica la capolista della giornata corrente

int main()
{
    int matrix[max][max]; //matrice contenente i risultati delle giornate
    int n, m; //numero squadre/giornate
    int riga = -1, colonna = 0; //contatori iniziali per input di righe e colonne
    FILE* inputFile; //file input
    
    if((inputFile = fopen("mat.txt", "rt"))==NULL) //verifica apertura corretta al file di input
    {
        puts("Errore nell'apertura del file!");
        exit(-1); //termine esecuzione programma
    }
    
    while(!feof(inputFile))
    {
        if(riga==-1) //input di n e m, prima riga del file
        {
            fscanf(inputFile, "%d %d", &n, &m);
            riga++;
        }
        
        else //successivi input per matrice
        {
            fscanf(inputFile, "%d", &matrix[riga][colonna]);
            
            if(colonna==m-1) //caso ultima colonna della matrice
            {
                riga++;
                colonna=0;
            }
            
            else
                colonna++;
        }
    }
    
    fclose(inputFile);
    
    for(colonna=0;colonna<m;colonna++) //verifica della capolista per ogni giornata
    {
        printf("Capolista della %d giornata: %d\n", colonna, capolista(matrix, n, colonna));
    }
    
    
    
    
}

int capolista(int matrix[max][max], int n, int colonna)
{
    int indiceCapolista, sommaCapolista = 0; //indice della capolista, somma punti per determinare la capolista
    int sommaCorrente; //somma dei punteggi dell'indice in analisi all'interno del ciclo for
    
    for(int i=0;i<=n;i++)
    {
        sommaCorrente = 0;
        
        for(int j=0;j<=colonna;j++)
        {
            sommaCorrente += matrix[i][j];
        }
        
        if(sommaCorrente>sommaCapolista) 
        {
            sommaCapolista = sommaCorrente;
            indiceCapolista = i;
        }
    }
    
    return indiceCapolista;
}
