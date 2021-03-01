#include <stdio.h>

#define n 8 //N elementi

int majority( int *a, int N); //majority
int majorityRic(int *a, int N, int l, int r); //majority wrapper
int main()
{
    int vet[n];
    int maggioritario;
    
    for(int i=0;i<n;i++)
    {
        printf("%d° elemento: ", i+1);
        scanf("%d", &vet[i]);
    }
    
    maggioritario = majority(vet,  n);
    (maggioritario==-1) ? puts("Non esiste un elemento maggioritario!") : printf("Elemento maggioritario: %d\n", maggioritario);
}

int majority(int *a, int N)
{
    int l = 0, r = N-1;
    
    return majorityRic(a, N, l, r);
}

int majorityRic(int *a, int N, int l, int r)
{
    int mid, maxSx, maxDx, sx = 0, dx = 0; //centro, massimo sx, massimo dx, sx, dx
    
    mid = (l+r)/2;
    
    if(l>=r)
        return a[l];
    
    maxSx = majorityRic(a, N, l, mid);
    maxDx = majorityRic(a, N, r, mid+1);
    if(maxDx==maxSx) return maxDx;
    
    for(int i=l;i<r+1;i++)
        if(maxSx==a[i])
            sx++;
    
    for(int i=l;i<r+1;i++)
        if(maxDx==a[i])
            dx++;
    
    if(sx>(r-l+1)/2) return maxSx;
    if(dx>(r-l+1)/2) return maxDx;
    
    return -1; //maggioritario non trovato
}
