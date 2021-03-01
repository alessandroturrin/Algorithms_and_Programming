#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define maxR 200 //massimo lunghezza riga
#define maxT 30 //dimensione fisica massima lunghezza termini in file dizionario

void codifica(char riga[maxR], char noCodificate[maxT][maxR], char siCodificate[maxT][maxR], unsigned short int s); //prototipo della procedura

int main()
{
    char riga[maxR]; //input riga
    unsigned short int s; //numero codifiche nel file dizionario
    
    char noCodificate[maxT][maxR]; //matrice di caratteri contenente le stringhe non codificate del file dizionario
    char siCodificate[maxT][maxR]; //matrice di caratteri contenente le stringhe codificate del file dizionario
    char contatoreInput = -1; //contatore per copia dei valori in fase di input
    
    FILE* sorgente; //file sorgente
    FILE* dizionario; //file dizionario
    
    if((sorgente=fopen("sorgente.txt", "rt"))==NULL)
    {
        puts("Errore nell'apertura del file 'sorgente.txt'!");
        exit(-1);
    }
    
    if((dizionario=fopen("dizionario.txt", "rt"))==NULL)
    {
        puts("Errore nell'apertura del file 'dizionario.txt'!");
        exit(-1);
    }

    
    /* Prendo in inpput dal file 'dizionario.txt' le S coppie <ricodifica><originale> e le salvo rispettivamente in siCodificta e noCodificata, al fine di poter operare più agevolmente con il
       file 'sorgente.txt', senza dover ogni volta effettuare la fscanf del file 'dizionario.txt' */
    
    while(!feof(dizionario))
    {
        if(contatoreInput==-1) //prima riga, input del numero di codifiche s
        {
            fscanf(dizionario, "%hu", &s);
            contatoreInput++;
        }
        else //input dalla seconda riga fino al termine del file
        {
            fscanf(dizionario, "%s", riga); //input di una stringa per volta per poter implementare i due array in parallelo
            
            if(riga[0]=='$') //caso nuova riga, non implemento il contatore per poter inizializzare le due matrici in parallelo
            {
                strcpy(siCodificate[contatoreInput],riga); //array contenente stringhe codificate
            }
            
            else
            {
                strcpy(noCodificate[contatoreInput], riga); //array contenente stringhe non codificate
                contatoreInput++;
            }
        }
    }
    
    //input da file sorgente
    while(fgets(riga, 200, sorgente)!=NULL) //lettura riga di massimo 200 caratteri con funzione fgets da file 'sorgente'
    {
        codifica(riga, noCodificate, siCodificate, s); //richiamo procedura 'codifica'
    }
    
    //chiusura file 'dizionario' e 'sorgente'
    fclose(dizionario);
    fclose(sorgente);
    
}

//procedura 'codifica' avente come parametri la riga presa in input dal file 'sorgente' con la funzione fgets, le matrici di caratteri contenenti rispettivamente, nell'ordine originale, le stringhe non codificate e le stringhe codificate, la variabile s rappresentante la dimensione logica+1 degli array precedenti(dimensione fisica al massimo uguale a 30)
void codifica(char riga[maxR], char noCodificate[maxT][maxR], char siCodificate[maxT][maxR], unsigned short int s)
{
    FILE* ricodificato; //file ricodificato
    int contatore = 0; //contatore per ciclo
    int uguali; //verifica se le due stringhe sono uguali o meno
    int i = 0, j = 0; //indici ausiliari per le due matrici di caratteri
    int indexRiga = 0; //indice riga per confrontare i caratteri all'interno del ciclo while
    
    if((ricodificato=fopen("ricodificato.txt", "at"))==NULL)
    {
        puts("Errore nell'apertura del file 'ricodificato.txt'");
        exit(-1);
    }
    
    //ciclo confronto sottostringhe di riga con matrice di caratteri 'noCodificate
    for(contatore=0;contatore<strlen(riga);contatore++)
    {
        //ripristino per ogni esecuzione del ciclo
        i=0;
        uguali=0;
        
        while(i<s) //scorrimento di tutto l'array contenente le stringhe non codificate
        {
            indexRiga=contatore;
            
            if(riga[contatore]==noCodificate[i][0]) //uguaglianza primo carattere
            {
                for(j=0;j<strlen(noCodificate[i]);j++)
                {
                    if(noCodificate[i][j]==riga[indexRiga])
                    {
                        indexRiga++;
                        uguali++;
                    }
                }
                    
                if(uguali==(strlen(noCodificate[i]))) //caso in cui tutti i caratteri di un intervallo coincidente con strlen(noCodificate[i])-1 sono uguali
                {
                    fprintf(ricodificato, "%s", siCodificate[i]); //stampa su 'ricodificato' la corrispondente stringa codificata
                    contatore = contatore + uguali - 1; //spostamento del cursore per non ripetere più di una volta l'analisi dei caratteri già convertiti
                    i=s; //break del ciclo while
                }
                
                else //caso non uguali
                {
                    i++; //elemento successivo di noCodificate
                    uguali=0;
                }
            }
            else
            {
                i++; //elemento successivo di noCodificate
                uguali=0;
            }
        }
        
        if(uguali==0) //in caso non uguali, stampa del carattere riga[contatore]
            fprintf(ricodificato, "%c", riga[contatore]);
    }
    
    fclose(ricodificato);
}
