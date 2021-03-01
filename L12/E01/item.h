#ifndef item_h
#define item_h

#define maxChar 21

typedef struct
{
    int yyyy, mm, dd; //anno, mese, giorno
    int h, m; //ora, minuto
}Data;

typedef struct item *Item;

Item *ITEMfirstScan(FILE *fp, int N);
int ITEMscan(FILE *fp, int oN, int nN, Item **item);
void ITEMshow(Item *item, int N);
Data ITEMgetData(Item item);
float ITEMgetValore(Item item);
int ITEMgetNumero(Item item);
void ITEMfree(Item *item, int dim);

int DATAcompare(Data q1, Data q2);
Data DATAinput();
void DATAprint(Data d);
Data DATAsetVoid();
#endif /* item_h */
