/*
 *
 *  Written by : Prashant Kumar Prajapati
 *  Time complexity : O(n)
 *  Space complexity : O(n)
 *
 */

 /*
  *
  *  This script generates a biased dataset having the numbers "2" and "133452" each in more than 20% of the dataset.
  *  All the other numbers are generated randomly and the entire dataset is then shuffled;
  *  All outputs are written to the input.txt file and number of random numbers to generate is taken from the console.
  *  Number of outputs should be in the limit [ 1 , 10^6 ] to avoid segmentation fault.
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Swaps the numbers a and b */
void swap (int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/* Shuffles an array randomly */
void randomize ( int arr[], int n )
{
    int i;
    srand(time(0));
    for (i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}

int main(){
    int n;
    printf( "Enter the number of numbers you want to generate\n" );
    scanf("%d", &n);
    int a[n];

    FILE *f;
    f = fopen( "input.txt", "w" );
    fprintf( f, "%d\n", n );

    srand( time( 0 ) );

    int i;
    for( i = 0; i <= n/5; i++)
        a[i] = 2;

    for(; i < 2 * n/5; i++)
        a[i] = rand();

    for(;i <= ((3 * n/5) + 1); i++)
        a[i] = 133452;

    for(;i<n;i++)
        a[i] = rand();

    randomize (a, n);

    for( i = 0; i < n; i++ )
        fprintf (f, "%d\n", a[i]);

    fclose(f);
    printf( "Numbers have been generated for testing\n" );
    return 0;
}
