#include <stdio.h>
#include <stdlib.h>

#define max 50 //dimensioni massime matrice

typedef struct
{
    int nr, nc; //nr. righe, nr. colonne
}dimensioni;

dimensioni inputFile(int matrix[][max], char fileName[]); //input di dati da file

void base(int matrix[][max], dimensioni numeroRC); //calcolo base maggiore
void altezza(int matrix[][max], dimensioni numeroRC); //calcolo altezza maggiore
void area(int matrix[][max], dimensioni numeroRC); //calcolo area maggiore
int main()
{
    char fileName[] = "mappa.txt"; //nome file di input
    int matrix[max][max]; //matrice contenente dati mappa
    int nr, nc; //numero di righe e colonne della matrice
    
    dimensioni numeroRC; //numero di nr e nc

    numeroRC = inputFile(matrix, fileName);

    
    base(matrix, numeroRC);
    altezza(matrix, numeroRC);
    area(matrix, numeroRC);
}


dimensioni inputFile(int matrix[][max], char fileName[])
{
    FILE* file; //file di input
    dimensioni inputrc; //input nr. righe e colonne
    
    if((file=fopen(fileName, "rt"))==NULL) //apertura e controllo file di input(mappa.txt)
    {
        printf("Errore nell'apertura di '%s'\n!", fileName);
        exit(-1);
    }
    
    fscanf(file, "%d %d", &inputrc.nr, &inputrc.nc); //input nr e nc
    
    for(int i=0;i<inputrc.nr;i++)
        for(int j=0;j<inputrc.nc;j++)
            fscanf(file, "%d", &matrix[i][j]);
    
    return inputrc;
}

void base(int matrix[][max], dimensioni numeroRC)
{
    dimensioni sxMax, sxAttuale; //angolo alto a sx di base massima e angolo alto a sx di base in esame
    int lMax=0, lAttuale=0; //base massima e lunghezza in esame
    int altezza = 1; //altezza per calcolo area
    int continua = 1; //per ciclo while
    
    for(int i=0;i<=numeroRC.nr;i++)
    {
        for(int j=0;j<=numeroRC.nc;j++)
        {
            if(matrix[i][j]==1)
            {
                if(lAttuale==0)//caso in cui inizio a contare una nuova base
                {
                    sxAttuale.nr=i;
                    sxAttuale.nc=j;
                }
                lAttuale++;
            }
            
            else
            {
                if(lAttuale>lMax) //nuovo massimo
                {
                    lMax = lAttuale;
                    sxMax = sxAttuale;
                }
                lAttuale=0;
            }
        }
    }
    
    
    while(continua)
    {
        lAttuale=0; //riutilizzo lAttuale per verificare l'altezza del rettangolo avente base massima lMax
        
        for(int j=0;j<=lMax;j++)
            if(matrix[altezza][j]==1)
                lAttuale++;
        if(lAttuale==lMax)
            altezza++;
        else
            continua=0;
    }
    
    printf("Max Base: estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n", sxMax.nr, sxMax.nc, lMax, altezza, lMax*altezza);
}

void altezza(int matrix[][max], dimensioni numeroRC)
{
    dimensioni sxMax, sxAttuale; //angolo alto a sx di altezza massima e angolo alto a sx di altezza in esame
    int hMax=0, hAttuale=0; //altezza massima e altezza in esame
    int base = 1; //base per calcolo area
    int continua = 1; //per ciclo while
    
    for(int j=0;j<=numeroRC.nc;j++)
    {
        for(int i=0;i<=numeroRC.nr;i++)
        {
            if(matrix[i][j]==1)
            {
                if(hAttuale==0)
                {
                    sxAttuale.nr=i;
                    sxAttuale.nc=j;
                }
                hAttuale++;
            }
            
            else
            {
                if(hAttuale>hMax)
                {
                    hMax=hAttuale;
                    sxMax=sxAttuale;
                }
                hAttuale=0;
            }
        }
        hAttuale=0;
    }
    
    while(continua)
    {
        hAttuale=0; //riutilizzo hAttuale per verificare la base del rettangolo avente altezza massima hMax
        
        for(int i=0;i<=hMax;i++)
            if(matrix[i][base]==1)
                hAttuale++;
        if(hAttuale==hMax)
            base++;
        else
            continua=0;
    }
    
    printf("Max Altezza: estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n", sxMax.nr, sxMax.nc, base, hMax, hMax*base);
}

void area(int matrix[][max], dimensioni numeroRC)
{
    dimensioni sxMax, sxAttuale; //angolo alto a sx di altezza massima e angolo alto a sx di altezza in esame
    int lMax=0, lAttuale=0, hMax=0, hAttuale=0; //rispettivamente lunghezze e altezze massime e attuali
    int aMax=0, aAttuale=0; //area massima e area in esame
    int continua=1;
    
    for(int i=0;i<=numeroRC.nr;i++)
    {
        for(int j=0;j<=numeroRC.nc;j++)
        {
            if(matrix[i][j]==1)
            {
                if(lAttuale==0)
                {
                    sxAttuale.nr=i;
                    sxAttuale.nc=j;
                }
                lAttuale++;
            }
            
            else
            {
                if(lAttuale>0)
                {
                    int x=sxAttuale.nr;
                    int contaUno;
                    continua=1;
                    hAttuale=0;
                    
                    while(continua==1)
                    {
                        contaUno=0;
                        
                        for(int y=sxAttuale.nc;y<(sxAttuale.nc+lAttuale);y++)
                        {
                            if(matrix[x][y]==1)
                                contaUno++;
                        }
                        
                        if(lAttuale==contaUno)
                            hAttuale++;
                        else
                            continua=0;
                        
                        x++;
                    }
                    
                    aAttuale=lAttuale*hAttuale;
                    
                    //printf("<%d,%d> l: %d   h: %d contauno: %d\n", sxAttuale.nr, sxAttuale.nc, lAttuale, hAttuale, contaUno);
                    if(aAttuale>aMax)
                    {
                        aMax=aAttuale;
                        lMax=lAttuale;
                        hMax=hAttuale;
                        sxMax=sxAttuale;
                    }
                }
                
                lAttuale=0;
            }
        }
    }
    
    printf("Max Area: estr. sup. SX=<%d,%d> b=%d, h=%d, Area=%d\n", sxMax.nr, sxMax.nc, lMax, hMax, aMax);
}
