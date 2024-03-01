#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

struct Corners
{
    struct vector *TopRight;
    struct vector *TopLeft;
    struct vector *BottomRight;
    struct vector *BottomLeft;
};

struct vector
{
    float x1;
    float y1;

    int permutation_value;
};  

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
    for(int i = 255; i > 0  ; i--)
    {
        random = rand()%i;
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
    int Base_x = floor(x);
    int Base_y = floor(y);

    int Point_x = x - Base_x;
    int Point_y = y - Base_y;

    Base_x = Base_x & 256;
    Base_y = Base_y & 256;
}

int main()
{
    srand(time(NULL));

    int *ptr = Permutation_maker();
    free(ptr);
}