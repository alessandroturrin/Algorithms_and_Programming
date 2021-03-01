#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef enum {false, true} bool;

bool LittleEndianTest(void); //test little endian, true se little
void stampaCodifica (void *p, int size, bool littleEndian);
void binConverter(void *p, int size, int bitBuffer[], int binNumber[], bool littleEndian); //converte in binario
void print(int binNumber[], int size, int exp, int padding); //padding solo per long double, per float e double inizializzato e 0, così riesco ad usare la stessa funzione
int checkPadding(bool littleEndian); //funzione per trovare i bytes di padding per ogni macchina

int main()
{
    float af;
    double ad;
    long double ald;
    bool littleEndian = LittleEndianTest();
    
    littleEndian ? puts("[working on LES - Little Endian System)]") : puts("[working on BES - Big Endian System)]"); //true se little endian
    
    printf("\n(float)Value: "); //input valori
    scanf("%f", &af);
    ad = (double)af;
    ald = (long double)af;
    
    printf("------FLOAT------------\nInput: %f\n[sizeof(float)]: %lu bytes = %lu bits", af, sizeof(af), sizeof(af)*8);
    stampaCodifica((void*)&af,sizeof(af),littleEndian);
    printf("------DOUBLE-----------\nInput: %lf\n[sizeof(double)]: %lu bytes = %lu bits", ad, sizeof(ad), sizeof(ad)*8);
    stampaCodifica((void*)&ad,sizeof(ad),littleEndian);
    printf("------LONG DOUBLE------\nInput: %Lf\n[sizeof(long double)]: %lu bytes = %lu bits", ald, sizeof(ald), sizeof(ald)*8);
    stampaCodifica((void*)&ald,sizeof(ald),littleEndian);
}

bool LittleEndianTest()
{
    int n = 1;
    if(*(char * )&n==1)
        return true;
    else
        return false;
}

void stampaCodifica (void *p, int size, bool littleEndian)
{
    int bitBuffer[128], binNumber[128];
    int padding = 0; //0 per float e double, calcolato da checkPadding per long double
    
    binConverter(&(*p), size, bitBuffer, binNumber, littleEndian);
    
    switch (size)
    {
        case sizeof(float):
            print(binNumber, size, 8, padding);
            break;
        case sizeof(double):
            print(binNumber, size, 11, padding);
            break;
        case sizeof(long double):
            padding = checkPadding(littleEndian);
            print(binNumber, size, 15, padding);
            break;
        default:
            break;
    }
}

void binConverter(void *p, int size, int bitBuffer[], int binNumber[], bool littleEndian)
{
    unsigned char a;
    int counter = 0;
    
    for(int i=0;i<size;i++)
    {
        a = *(unsigned char *)p;
        
        for(int j=0;j<8;j++)
        {
            bitBuffer[counter]= a%2;
            a = (a-bitBuffer[counter])/2;
            counter = counter + 1;
        }
        p++;
    }
    counter -= 1;
    
    if(littleEndian)
    {
        for(int i=0;i<(counter);i++)
            binNumber[i]=bitBuffer[counter-i];
    }
}

void print(int binNumber[], int size, int exp, int padding) //padding usato solo per long double
{
    int counter;
    
    padding!=0 ? printf("\n[padding]: %d bytes = %d bits\n[displayed %d bytes out of %lu total bytes]\n", padding/8, padding, padding/8, sizeof(long double)) : puts("");
    
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tMSB");
    for(counter=padding;counter<size*8;counter++)
    {
        if(counter==padding || counter%8==0)
            printf("\n[memory index]: %p\t[allocated byte]: %d", &binNumber[counter], binNumber[counter]);
        else
            printf("%d", binNumber[counter]);
    }
    printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tLSB\n");
    
    printf("\n|s|\t\t|e|\t\t\t\t|m|\n|%d|", binNumber[padding]); //stampa segno
    for(counter=padding+1;counter<size*8;counter++)
    {
        if(counter==padding+exp+1)
            printf("|%d", binNumber[counter]);
        else
            printf("%d", binNumber[counter]);
    }
    printf("\n-----------------------\n\n\n");
}

int checkPadding(bool littleEndian) //trova padding del long double in base al calcolatore
{
    long double number = 1;
    long double opposite = -1;
    int counter = 0;
    bool keep = true;
    int bitN[128], bitO[128], binN[128], binO[128];
    
    binConverter((long double*)&number, sizeof(long double), bitN, binN, littleEndian);
    binConverter((long double*)&opposite, sizeof(long double), bitO, binO, littleEndian);
    
    for(int i=0;i<(8*sizeof(long double)) && keep;i++)
    {
        if(binN[i]!=binO[i] && i%8==0) //bit di segno
            keep = false;
        else
            counter++;
    }
    return counter;
}
