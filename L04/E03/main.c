#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define lSrc 100 //lunghezza src

typedef enum {false, true} bool;
char *cercaRegexp(char *src, char *regexp);
int isEnd(char currentChar);
char *metaCarattere(char *word, char *reg);
int main()
{
    char *src = "fotoerartEpocacicletta"; //src
    char *regexp = ".oto\\aera\\Apoc[^r]icl\0"; //regexp
    char *occorrenza;
    
    if(strlen(regexp)>strlen(src))
        exit(1);
    
    occorrenza = cercaRegexp(src, regexp);
    
    occorrenza==NULL ? puts("Nessuna occorrenza!") : printf("\nPrima occorrenza della sequenza completa di metacaratteri '%s': %p\n", regexp, &occorrenza);
}

char *cercaRegexp(char *src, char *regexp)
{
    char *first, *next; //prima occorrenza, n-esima occorrenza
    char *word;
    bool keep = true, inizio = true;
    int counter = 0;
    
    while(keep) //suddivisione della stringa di metacaratteri in sottostringhe
    {
        switch (isEnd(*regexp))
        {
            case 0:
                regexp++;
                counter++;
                break;
            case 1:
                strncpy(word, regexp-counter, counter);
                if(inizio)
                {
                    if(counter==0) //caso in cui la *regexp sia un metacarattere
                    {
                        counter++;
                        regexp++;
                        break;
                    }
                    printf("Sottosequenza in analisi: %s", word);
                    first = metaCarattere(src, word);
                    if(first==NULL) return first;
                    inizio=false;
                    printf("\t->\t%p\n", first);
                }
                else
                {
                    printf("Sottosequenza in analisi: %s", word);
                    next = metaCarattere(src, word);
                    printf("\t->\t%p\n", next);
                    if(next==NULL) return next;
                }
                counter = 1;
                regexp++;
                break;
            case 2:
                strncpy(word, regexp-counter, counter);
                printf("Sottosequenza in analisi: %s", word);
                if(inizio)
                {
                    first = metaCarattere(src, word);
                    first==NULL ? printf("\t->\tNULL\n", first) : printf("\t->\t%p\n", first);
                    return first;
                }
                else
                {
                    next = metaCarattere(src, word);
                    printf("\t->\t%p\n", next);
                    if(next!=NULL) return first;
                    return next;
                }
                break;
            default:
                keep = false;
                break;
        }
    }
    
    return NULL;
}
int isEnd(char currentChar)
{
    if(currentChar=='.' || currentChar=='[' || currentChar=='\\') return 1; //caso inizio nuovo metacarattere
    if(currentChar=='\0' || currentChar=='\n') return 2; //caso fine parola
    else return 0; //caso carattere normale
}
char *metaCarattere(char *word, char *reg)
{
    int counter = 0;
    char *index = NULL;
    

    if(isalpha(*reg))
        return strstr(word, reg);
    
    switch (*reg)
    {
        case '.':
            reg++;
            return strstr(word, reg);
        case '[':
        {
            char letter;
            reg++;
            if((*reg)=='^')
            {
                reg++;
                letter = *reg; //salvo lettera da escludere
                reg+=2; //esco dalla quadra
                index = strstr(word, reg);
                if(index!=NULL) //presente all'interno della parola
                {
                    word = index;
                    if((*(word-1))==letter) return NULL;
                    else return word-1;
                }
                return NULL;
            }
            else
            {
                char *save = reg; //salvo indirizzo di reg
                char subStr[lSrc]; //sottostringhe
                while((*reg)!=']')
                    reg++;
                strcpy(subStr+1, reg+1);
                reg = save;
                for(counter=0;(*reg)!=']';reg++)
                {
                    subStr[counter] = (*reg);
                    index = strstr(word, subStr);
                    if(index!=NULL) return index;
                }
                return NULL;
            }
        }
            break;
        case '\\':
        {
            reg++;
            if(*reg=='a')
            {
                reg++;
                index = strstr(word, reg);
                if(index!=NULL)
                {
                    word = index;
                    if(islower(*(word-1))) return word-1;
                    return NULL;
                }
            }
            if(*reg=='A')
            {
                reg++;
                index = strstr(word, reg);
                if(index!=NULL)
                {
                    word = index;
                    if(isupper(*(word-1))) return word-1;
                    return NULL;
                }
            }
        }
            break;
        default:
            break;
    }
    
    return NULL;
}
