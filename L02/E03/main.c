//ho cambiato gli apostrofi del file 'testo.txt' perchè il progrmamma non me li riconosceva

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define lSeq 6 //lunghezza massima sequenza
#define maxSeq 20 //numero massimo sequenze
#define maxParola 26 //lunghezza massima parola


int inputSequenze(char seq[][lSeq], char fileSequenze[]);
void inputTesto(char seqSing[], char fileTesto[]);
int printParola(char parola[], int contaParole, int index);

int main()
{
    char fileSequenze[] = "sequenze.txt";
    char fileTesto[] = "testo.txt";
    int dim; //numero sequenze totali
    char seq[maxSeq][lSeq]; //vettore di struct contenente le sequenze prese in input da file
    
    dim = inputSequenze(seq, fileSequenze);
    
    for(int i=0;i<dim;i++)
    {
        inputTesto(seq[i], fileTesto);
        puts("");
    }
    
}

int inputSequenze(char seq[][lSeq], char fileSequenze[])
{
    FILE* fileSeq;
    int dim; //numero totale di sequenze
    
    if((fileSeq=fopen(fileSequenze, "rt"))==NULL)
    {
        printf("Errore nell'apertura di '%s'!\n", fileSequenze);
        exit(-1);
    }
    
    fscanf(fileSeq, "%d", &dim); //input dim
    for(int i=0;i<dim;i++)
        fscanf(fileSeq, "%s", seq[i]);
    
    return dim;
    
}

void inputTesto(char seqSing[], char fileTesto[])
{
    FILE* fileText;
    char parola[maxParola];
    int contaParole = 0;
    int contaOccorrenze = 0;
    int uguali;
    
    int apostrofo=0;
    
    if((fileText=fopen(fileTesto, "rt"))==NULL)
    {
        printf("Errore nell'apertura di '%s'!\n", fileTesto);
        exit(-1);
    }
    
    while(!feof(fileText))
    {
        fscanf(fileText, "%s", parola);
        contaParole++;
        apostrofo = 0;
        for(int i=0;i<strlen(parola);i++)
        {
            uguali = 0;
            
            for(int j=0;j<strlen(seqSing);j++)
            {
                if((int)parola[i]!=39)
                {
                    if((i+j)<strlen(parola))
                    {
                        if(tolower(seqSing[j])==tolower(parola[i+j]))
                            uguali++;
                    }
                    else
                    {
                        if(parola[i]==',')
                            contaParole--;
                        
                        uguali = 0;
                    }
                }
                else
                {
                    apostrofo = i+j;
                    contaParole++;
                    uguali=0;
                }
               
            }
            
            if(uguali==(strlen(seqSing)) && contaOccorrenze<10)
            {
                if(contaOccorrenze==0)
                    printf("La sequeza '%s' e' contenuta in:\n", seqSing);
                
                contaOccorrenze = contaOccorrenze + printParola(parola, contaParole, apostrofo);
                            
                i=strlen(parola); //esco dal ciclo for
            }
            else if(contaOccorrenze==10) //caso in cui siano già state stampate 10 occorrenze
            {
                puts("Stampa interrotta: 10 occorrenze di questa sequenza stampate!");
                contaOccorrenze++;
            }
            

        }
    }
    
    if(contaOccorrenze==0)
        printf("La sequenza '%s' non e' contenuta nel testo!\n", seqSing);
    
    fclose(fileText);

    
}

int printParola(char parola[], int contaParole, int index)
{
    int keep;
    
    printf("-'");
    
    if(index==0) //stampo parola prima dell'apostrofo(se presente)
        keep = 0;
    
    else //stampo parola dopo apostrofo
        keep = index + 1;
    
    while(keep>=0 && parola[keep]!='\0')
    {
        if((int)parola[keep]!=39)
            if(!ispunct(parola[keep]))
            {
                printf("%c", parola[keep]);
                keep++;
            }
            else
                keep=-1;
        else
            keep=-1;
    }

    printf("' (parola in posizione %d nel testo)\n", contaParole);
    
    return 1;
}
