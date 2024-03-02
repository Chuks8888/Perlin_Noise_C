#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h> 

struct vector
{
    float distance_vector_x;
    float distance_vector_y;

    int permutation_value;

    float gradient_vector_x;
    float gradient_vector_y;

    float dot_prouct;
};

void swap(int *table, int i, int j);
int* Permutation_maker();
void assign_vector_values(struct vector temp[], float Point_x, float Point_y, int Base_x, int Base_y);
float Perlin(float x, float y);
float Fade(float t);
float Lerp(float a, float b, float t);
int generate(int octaves, int i, int j);

int *permutation; // declare a global permutation array

#define Height  500
#define Width   500
int main()
{
    srand(time(NULL));

    permutation = Permutation_maker(); // define of the global array

    int Numbers[Height][Width];
    int number_of_iterations = 8;

    FILE *save = fopen("Perlin_noise.txt", "w");

    for(int i = 0; i < Height; i++)
    {
        for(int j = 0; j < Width; j++)
        {
            Numbers[i][j] = generate(number_of_iterations, i, j);
            fprintf(save, "%d ", Numbers[i][j]);
        }
        fprintf(save, "\n");
    }

    free(permutation);
}

int generate(int octaves, int i, int j)
{
    float result = 0.0;

    float frequency = 0.01;
    float amplitude = 0.7;

    for (int o = 0 ; o < octaves; o++)
    {
        result += amplitude * Perlin(frequency * i, frequency * j);

        frequency *=2.0;
        amplitude /=2.0;
    }

    return round(result * 255);
}

float Perlin(float x, float y)
{
    int Base_x = floor(x);
    int Base_y = floor(y);

    float Point_x = x - (float)Base_x;
    float Point_y = y - (float)Base_y;

    Base_x = Base_x & 255;
    Base_y = Base_y & 255;
    
    struct vector Cell[4];

    assign_vector_values(Cell, Point_x, Point_y, Base_x, Base_y);

    float Fade_x = Fade(Point_x);
    float Fade_y = Fade(Point_y);


    // 0 - top right   1 - top left    2 - bottom right    3 - bottom left
    float a = Lerp(Cell[3].dot_prouct, Cell[1].dot_prouct, Fade_y);
    float b = Lerp(Cell[2].dot_prouct, Cell[0].dot_prouct, Fade_y);

    return (Lerp(a, b, Fade_x) + 1)/2;
}

void assign_vector_values(struct vector temp[], float Point_x, float Point_y, int Base_x, int Base_y)
{
    // Iteration:   0 - top right   1 - top left    2 - bottom right    3 - bottom left
    for(int i = 0; i < 4; i++)
    {
        temp[i].distance_vector_x = Point_x - ((i+1) & 1); // minus one for i = 0 and 2
        temp[i].distance_vector_y = Point_y - (i > 1 ? 0:1); // minus one for i = 0 and 1 

        temp[i].permutation_value = permutation[permutation[Base_x + ((i+1) & 1)] + Base_y + (i > 1 ? 0:1)]; // the hash function

        int rest= temp[i].permutation_value % 4;

        temp[i].gradient_vector_x = (rest > 1)? -1.0:1.0; // -1 for rest = 2 and 3
        temp[i].gradient_vector_y = (rest % 2 == 0)? -1.0:1.0; // -1 for rest = 0 and 2

        temp[i].dot_prouct = temp[i].distance_vector_x * temp[i].gradient_vector_x + temp[i].distance_vector_y * temp[i].gradient_vector_y;
        
        //printf("%f, %f, %d, %f\n", temp[i].distance_vector_x, temp[i].distance_vector_y, temp[i].permutation_value, temp[i].dot_prouct);
    }
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

void swap(int *table, int i, int j)
{
    int temp = table[i];
    table[i] = table[j];
    table[j] = temp;
}

float Fade(float t)
{
    return t * t * t *(t * (t * 5.0 - 15.0) + 10.0);
}

float Lerp(float a, float b, float t)
{
    return a + t*(b-a);
}
