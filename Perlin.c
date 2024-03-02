#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

struct vector
{
    float distance_vector_x;
    float distance_vector_y;

    int permutation_value;

    //float gradient_vector_x;
    //float gradient_vector_y;

    float dot_prouct;
};

void swap(int *table, int i, int j);
int* Permutation_maker();
void assign_vector_values(struct vector temp[], float Point_x, float Point_y, int Base_x, int Base_y);
float Perlin(float x, float y);
float Fade(float t);
float Lerp(float a, float b, float t);
int generate(int i, int j);

int *permutation; // declare a global permutation array

struct vector Cell[4];

#define Height  300
#define Width   300
int main()
{
    //srand(time(NULL));

    permutation = Permutation_maker(); // define of the global array

    int Numbers[Height][Width];

    // FILE *save = fopen("Perlin_noise.txt", "w");

    for(int i = 0; i < Height; i++)
    {
        for(int j = 0; j < Width; j++)
        {
            Numbers[i][j] = generate(j, i);
            //fprintf(save, "%d ", Numbers[i][j]);
            if(Numbers[i][j] < 0) Numbers[i][j] = 0;
            if(Numbers[i][j] > 255) Numbers[i][j] = 255;
        }
        //fprintf(save, "\n");
    }
    free(permutation);
    // fclose(save);

    int width, height, channels;
    unsigned char *img = stbi_load("DO_NOT_REMOVE.jpg", &width, &height, &channels, 0);

    if(img == NULL)
    {
        printf("error reading files");
        exit(1);
    }

    size_t img_size = Height * Width * channels;
    int new_channels = channels;
    size_t new_size = Width * Height * new_channels;

    unsigned char *newimg = malloc(new_size);
    if(newimg == NULL)
    {
        printf("error allocating memory");
        exit(1);
    }

    int x = 0, y = 0;
    for(unsigned char *p = img, *n = newimg; p!= img + img_size; p+=channels, n+=new_channels)
    {
        if(x == Width)
        {
            x = 0;
            y++;
        }

        *(n) = (uint8_t)(*(p)*0.0 + Numbers[y][x]);
        *(n+1) = (uint8_t)(*(p+1)*0.0 + Numbers[y][x]);
        *(n+2) = (uint8_t)(*(p+2)*0.0 + Numbers[y][x]);

        /*if(Numbers[y][x] >= 180)
        {
            *(n) = (uint8_t)(*(p)*0.0 + Numbers[y][x]);
            *(n+1) = (uint8_t)(*(p+1)*0.0 + Numbers[y][x]);
            *(n+2) = (uint8_t)(*(p+2)*0.0 + Numbers[y][x]);
        }
        else if(Numbers[y][x] > 100)
            *(n+1) = (uint8_t)(*(p+1)*0.0 + Numbers[y][x] - 30.0);
        else if(Numbers[y][x] <= 100)
            *(n+2) = (uint8_t)(*(p+2)*0.0 + Numbers[y][x] + 30.0);*/
        if(channels == 4) {
            *(n+1) = *(p+3);
        }
        x++;
    }

    stbi_write_jpg("test.jpg", Width, Height, new_channels, newimg, 100);

    stbi_image_free(img);
    stbi_image_free(newimg);
}

int generate(int i, int j)
{
    float result = 0.0;
    int octaves = 4;
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

    float Point_x = x - floor(x);
    float Point_y = y - floor(y);

    Base_x = Base_x & 255;
    Base_y = Base_y & 255;

    assign_vector_values(Cell, Point_x, Point_y, Base_x, Base_y);

    float Fade_x = Fade(Point_x);
    float Fade_y = Fade(Point_y);

    // 0 - top right   1 - top left    2 - bottom right    3 - bottom left
    float a = Lerp(Cell[3].dot_prouct, Cell[1].dot_prouct,  Fade_y);
    float b = Lerp(Cell[2].dot_prouct, Cell[0].dot_prouct, Fade_y);

    return (Lerp(a, b, Fade_x) + 1.0)/2.0;
}

void assign_vector_values(struct vector temp[], float Point_x, float Point_y, int Base_x, int Base_y)
{
    // Iteration:   0 - top right   1 - top left    2 - bottom right    3 - bottom left
    for(int i = 0; i < 4; i++)
    {
        
        temp[i].distance_vector_x = Point_x - ((i+1) & 1); // for 0 and 2
        temp[i].distance_vector_y = Point_y - (1 - (i&2)/2); // for 0 and 1
        temp[i].permutation_value = permutation[permutation[Base_x + ((i+1) & 1)] + Base_y + (1 - (i&2)/2)];
        // same here +1 for X where i is 0 and 2, +1 for Y where 0 and 1

        int rest = temp[i].permutation_value & 4;
        temp[i].dot_prouct = (-1.0 * (-1.0 + (rest&2)))*temp[i].distance_vector_x + (-1.0 + 2*((rest+1) & 1))*temp[i].distance_vector_y;
        // -1, -1 for rest = 3, -1, 1 for rest = 2, 1,1 for rest = 1, 1,1 for rest = 0
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
    int random, temp;
    for(int i = 0; i < 255  ; i++)
    {
        random = rand()% 256;
        temp = Permutation[i];
        Permutation[i] = Permutation[random];
        Permutation[random] = temp;
    }

    // fillig the second half of the array
    for(int i = 0; i < 256; i++)
    {
        Permutation[i+256] = Permutation[i];
    }

    return Permutation;
}

float Fade(float t){
    return t * t * t *(t * (t * 6.0 - 15.0) + 10.0);
}

float Lerp(float a, float b, float t){
    return a + t*(b-a);
}