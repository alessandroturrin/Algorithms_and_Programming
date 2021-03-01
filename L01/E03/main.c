#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 30 //dimensione massima matrice
#define lMax 21 //massimo numero caratteri nome del file di input
#define strL 100 //massima lunghezza dei quattro comandi

typedef struct
{
    int righe, colonne;
}inputMat;

inputMat inputMatrice(int matrix[][max]);
void showMatrix(int matrix[][max], int r, int c); //stampa matrice
int continua(int matrix[][max], char selettore[lMax], int indice, char direzione[lMax], int posizioni, int r, int c); //verifica validità input
char direzChar(char direzione[lMax]); //converte valore direzione in char
void ruotaMatrix(int matrix[][max], char rc, int indice, char dir, int posizioni, int r, int c); //generica per rotazione
void ruotaVerticale(int matrix[][max], int indice, int dir, int posizioni, int r); //rotazione verticale(su|giu)
void ruotaOrizzontale(int matrix[][max], int indice, int dir, int posizioni, int c); //rotazione orizzontale(destra|sinistra)


int main()
{
    int matrix[max][max]; //matrice
    inputMat inputM; //input nr righe e colonne
    int righe, colonne; //numero righe e colonne
    
    int keep = 1; // true/false per ciclo do/while
    char comando[strL]; //input comando con fgets
    char selettore[lMax], direzione[lMax]; //due dei quattro comandi
    int indice, posizioni; //ultimi due comandi
    
    inputM = inputMatrice(matrix); //input matrice
    righe = inputM.righe;
    colonne = inputM.colonne;

    
    do //ciclo principale
    {
        showMatrix(matrix, righe, colonne); //stampa matrice a ogni ciclo
        printf("Comandi: ");
        fgets(comando, strL, stdin);
        sscanf(comando, "%s %d %s %d", selettore, &indice, direzione, &posizioni);
        keep = continua(matrix, selettore, indice, direzione, posizioni, righe, colonne); //per terminare -> selettore deve essere "fine"
        puts("");
    }while(keep);
    
}

inputMat inputMatrice(int matrix[][max])
{
    inputMat input; //numero righe e colonne
    int i=0,j=0; //indici ausiliari
    char nomeFile[lMax];
    FILE* file;
    
    printf("Nome file: ");
    scanf("%s", nomeFile);
    puts("");
    
    file=fopen(nomeFile, "rt");
    
    if(file==NULL)
    {
        puts("Errore nell'apertura di file!");
        exit(-1);
    }
    
    fscanf(file, "%d %d", &input.righe, &input.colonne); //input nr. righe e nr. colonne matrice
    
    for(i=0;i<input.righe;i++)
        for(j=0;j<input.colonne;j++)
            fscanf(file, "%d", &matrix[i][j]);

    fclose(file);
    
    return input;
}

void showMatrix(int matrix[][max], int r, int c)
{
    for(int i=0;i<r;i++)
    {
        for(int j=0;j<c;j++)
        {
            printf("%d ", matrix[i][j]);
        }
        puts("");
    }
}

int continua(int matrix[][max], char selettore[lMax], int indice, char direzione[lMax], int posizioni, int r, int c)
{
    char dir, rc; //direzione, riga o colonna

    if(strcmp(selettore,"fine")==0) //termine esecuzione ciclo
        return 0;
    
    if((dir = direzChar(direzione))=='q') //ossia valore non valido
        return 1;
    //controlli dei vari input
    if(posizioni>0 && dir!='0')
    {
        if(strcmp(selettore, "riga")==0)
        {
            if(indice<=0 || indice>r)
            {
                puts("Errore indice riga!");
            }
            
            rc = 'r';
            ruotaMatrix(matrix, rc, indice, dir, posizioni, r, c); //avviene rotazione
        }
        else if(strcmp(selettore, "colonna")==0)
        {
            if(indice<=0 || indice>c)
            {
                puts("Errore indice colonna!");
                return 1;
            }
            
            rc = 'c';
            ruotaMatrix(matrix, rc, indice, dir, posizioni, r, c); //avviene rotazione
        }
        else
        {
            puts("Comandi validi: 'riga'-'colonna'-'fine'");
        }
    }
    else
    {
        puts("Posizioni valide: 0<posizione<=Colonna|Riga");
    }
    
    return 1;
    
}

char direzChar(char direzione[lMax])
{
    if(strcmp(direzione, "su")==0)
        return 'a'; //alto
    else if(strcmp(direzione, "giu")==0)
        return 'b'; //basso
    else if(strcmp(direzione, "destra")==0)
        return 'd'; //destra
    else if(strcmp(direzione, "sinistra")==0)
        return 's'; //sinistra
    else
        puts("Comandi validi: 'su'|'giu'|'destra'|'sinistra'"); //errore
    
    return 'q';
}

void ruotaMatrix(int matrix[][max], char rc, int indice, char dir, int posizioni, int r, int c)
{
    if(rc=='r') //per righe si puo' andare solo verso dx, sx
    {
        switch (dir)
        {
            case 'd': //dx
                ruotaOrizzontale(matrix, indice, 1, posizioni, c);
                break;
            case 's': //sx
                ruotaOrizzontale(matrix, indice, -1, posizioni, c);
            default:
                break;
        }
    }
    
    if(rc=='c') //per colonne si può andare solo verso su, giù
    {
        switch (dir)
        {
            case 'a': //su
                ruotaVerticale(matrix, indice, 1, posizioni, r);
                break;
            case 'b': //giù
                ruotaVerticale(matrix, indice, -1, posizioni, r);
            default:
                break;
        }
    }
}

void ruotaVerticale(int matrix[][max], int indice, int dir, int posizioni, int r)
{
    int save; //salvo ultimo|primo elemento
    int convIndice = indice-1; //indice adattato
    
    if(dir==1) //verso su
    {
        for(int k=0;k<posizioni;k++)
        {
            save=matrix[0][convIndice];
            for(int i=0;i<r-1;i++)
            {
                matrix[i][convIndice] = matrix[i+1][convIndice];
            }
            matrix[r-1][convIndice]=save;
        }
    }
    
    if(dir==-1) //verso giu
    {
        for(int k=0;k<posizioni;k++)
        {
            save=matrix[r-1][convIndice];
            for(int i=r-1;i>0;i--)
            {
                matrix[i][convIndice] = matrix[i-1][convIndice];
            }
            matrix[0][convIndice]=save;
        }
    }
}

void ruotaOrizzontale(int matrix[][max], int indice, int dir, int posizioni, int c)
{
    int save; //salvo ultimo|primo elemento
    int convIndice = indice-1; //indice adattato
    
    if(dir==1) //verso destra
    {
        for(int k=0;k<posizioni;k++)
        {
            save=matrix[convIndice][c-1];
            for(int j=c-1;j>0;j--)
            {
                matrix[convIndice][j] = matrix[convIndice][j-1];
            }
            matrix[convIndice][0]=save;
        }
    }
    
    if(dir==-1) //verso sinistra
    {
        for(int k=0;k<posizioni;k++)
        {
            save=matrix[convIndice][0];
            for(int j=0;j<c-1;j++)
            {
                matrix[convIndice][j] = matrix[convIndice][j+1];
            }
            matrix[convIndice][c-1]=save;
        }
    }

}
