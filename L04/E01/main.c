#include <stdlib.h>
#include <stdio.h>

void swap(int* a, int* b);
int gcd(int a, int b);
int main()
{
    int n1, n2;
    printf("Inserire due numeri interi: ");
    scanf("%d %d", &n1, &n2);
    printf("Massimo comune divisore calcolato ricorsivamente: %d\n", gcd(n1,n2));
    return 0;
}

void swap(int *a, int *b)
{
    int tmp;
    if((*a) < (*b))
    {
        tmp = (*a);
        (*a) = (*b);
        (*b) = tmp;
    }
}
int gcd(int a, int b)
{
    if(a==b) return a;
    else if(a==0) return 1;
    
    if(b>a) swap(&a, &b);
    
    if(a%2==0 && b%2==0) return (2*gcd(a%2, b/2)); //a e b E
    else if(a%2!=0 && b%2==0) return gcd(a, b/2); //aO bE
    else if(a%2==0 && b%2!=0) return gcd(a/2, b);
    else return gcd((a-b)/2, b);//a e b O
}
