#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define titleLength 255

typedef struct
{
    int numeroCanzoni;
    int *index;
    char **titoloCanzone;
}blocco;

int generaPlaylistWrapper(blocco *bloccoCanzoni, int numeroAmici);
int generaPlaylist(blocco *bloccoCanzoni, int *sol, int pos, int numeroAmici, int count);
void print(blocco *bloccoCanzoni, int *sol, int numeroAmici, int count);
int main()
{
    FILE* file;
    int numeroAmici;
    blocco *bloccoCanzoni;
    char inputSong[titleLength];
    
    if((file=fopen("/Users/alexturrin/Desktop/brani.txt", "rt"))==NULL)
    {
        printf("Errore nell'apertura del file!\n");
        exit(1);
    }
    fscanf(file, "%d", &numeroAmici);
    
    bloccoCanzoni = (blocco*)malloc(numeroAmici*sizeof(blocco));
    for(int i=0;i<numeroAmici;i++)
    {
        fscanf(file, "%d", &bloccoCanzoni[i].numeroCanzoni);
        bloccoCanzoni[i].index = (int*)malloc(bloccoCanzoni[i].numeroCanzoni*sizeof(int));
        bloccoCanzoni[i].titoloCanzone = (char**)malloc(bloccoCanzoni[i].numeroCanzoni*sizeof(char*));
        for(int j=0;j<bloccoCanzoni[i].numeroCanzoni;j++)
        {
            fscanf(file, "%s", inputSong);
            bloccoCanzoni[i].titoloCanzone[j] = (char*)malloc((strlen(inputSong)+1)*sizeof(char));
            bloccoCanzoni[i].index[j] = j;
            strcpy(bloccoCanzoni[i].titoloCanzone[j], inputSong);
        }
    }
    printf("\nSono state generate %d playlist!\n\n", generaPlaylistWrapper(bloccoCanzoni, numeroAmici));
    fclose(file);
    free(bloccoCanzoni);
}

int generaPlaylistWrapper(blocco *bloccoCanzoni, int numeroAmici)
{
    int *sol, pos=0, count=0;
    sol = (int*)malloc(numeroAmici*sizeof(int));
    return generaPlaylist(bloccoCanzoni, sol, pos, numeroAmici, count);
}

int generaPlaylist(blocco *bloccoCanzoni, int *sol, int pos, int numeroAmici, int count)
{
    if(pos>=numeroAmici)
    {
        print(bloccoCanzoni, sol, numeroAmici, count);
        return count+1;
    }
    for(int i=0;i<bloccoCanzoni[pos].numeroCanzoni;i++)
    {
        sol[pos] = bloccoCanzoni[pos].index[i];
        count = generaPlaylist(bloccoCanzoni, sol, pos+1, numeroAmici, count);
    }
    return count;
}

void print(blocco *bloccoCanzoni, int *sol, int numeroAmici, int count)
{
    printf("Playlist-%d: ", count+1);
    for(int i=0;i<numeroAmici;i++) printf("%s ", bloccoCanzoni[i].titoloCanzone[sol[i]]);
    puts("");
}
