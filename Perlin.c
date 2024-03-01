#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

void swap(int *table, int i, int j)
{
    int temp = table[i];
    table[i] = table[j];
    table[j] = temp;
}

int* Permutation_maker() // function for creating permutation array
{
    // declaring the array and initialize memory
    int *Permutation = malloc(2 * 256 * sizeof(int)); 

    // filling array
    for(int i = 0; i < 256; i++)
    {
        Permutation[i] = i;
    }

    // shuffling of array
    int random;
    for(int i = 0; i < 256; i++)
    {
        random = rand()%256;
        swap(Permutation, i, random);
    }

    // fillig the second half of the array
    for(int i = 0; i < 256; i++)
    {
        Permutation[i+256] = Permutation[i];
    }

    return Permutation;
}

float TDPerlin(float x, float y)
{

}

int main()
{
    srand(time(NULL));

    int *ptr = Permutation_maker();
    free(ptr);
}