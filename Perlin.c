#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

// HERE ARE THE CONTROLS
// MAX HEIGHT AND WIDTH IS 
// 1080 x 1920
#define Height  1080
#define Width   1920
#define freq    0.005 // frequency
#define octav   8    // octaves
#define ampli   0.7  // amplitude
/*
    The number of octaves defines
    how many time the function Perlin is 
    used (lets call it smoothness)

    The frequency will influence how often 
    the grid points change

    The amplitude will only change the 
    result that we need in this case 
    its 1.0 because we need values
    between 0 and 255 because of RGB
	if we were to lower it, the noise
	would be a lot darker
*/

// Structure for a single vector
struct vector
{
    // vectors to the current 
    // point in a cell
    float distance_vector_x;
    float distance_vector_y;

    // the permutation value of the
    // grid point
    int permutation_value;

    // dot product of distance vector
    // and the constant vector
    float dot_prouct;
};

// Decrlaration of functions
int* Permutation_maker();
void assign_vector_values(struct vector temp[], float Point_x, float Point_y, int Base_x, int Base_y);
float Perlin(float x, float y);
float Fade(float t);
float Lerp(float a, float b, float t);
int generate(int i, int j);
// End of decrlaration of functions

int *permutation; // declare a global permutation array

int main()
{
    permutation = Permutation_maker(); // define of the global array

    int Numbers[Height][Width]; 
	// Numbers got from
	// the algorithm

    FILE *save = fopen("Perlin_noise.txt", "w");
	// File for saving the numbers 

    for(int i = 0; i < Height; i++)
        for(int j = 0; j < Width; j++)
            Numbers[i][j] = generate(j, i);
	// Loop for calculating each number

    for(int i = 0; i < Height; i++)
    {
        for(int j = 0; j < Width; j++)
        {
            if(Numbers[i][j] > 255) Numbers[i][j] = 255;
            fprintf(save, "%d ", Numbers[i][j]);
        }
        fprintf(save, "\n");
    }
	// Loop for writing to file and correcting numbers higher than 255
	// which is the maximal value for RGB
	
    free(permutation);
    fclose(save);
	// freeing memory

	// geting data from a blank jpg, this is used
	// so that we manipulate the pixels and create
	// a new picture. That is our Perlin noise
	int width, height, channels;
	unsigned char *img = stbi_load("DO_NOT_REMOVE.jpg", &width, &height, &channels, 0);

	if(img == NULL)
	{
		printf("error reading files");
		exit(1);
	}

	// calculating image sizes
	size_t img_size = Height * Width * channels;
	int new_channels = channels == 4 ? 2:1;
	size_t new_size = Width * Height * new_channels;

	// allocating memory for new picture
	unsigned char *newimg = malloc(new_size);
	if(newimg == NULL)
	{
		printf("error allocating memory");
		exit(1);
	}

	// Loop for manipulating pixel data that is stored
	// in a pointer to the new image
	int x = 0, y = 0;
	for(unsigned char *p = img, *n = newimg; p!= img + img_size; p+=channels, n+=new_channels)
	{
		if(x == Width)
		{
			x = 0;
			y++;
		}

		*(n) = (uint8_t)(*(p)*0.0 + Numbers[y][x]);
		if(new_channels == 2)
		{
			*(n+1) = (uint8_t)(*(p+3));
		}

		x++;
	}

	// writing the image and then freeing the memory
	stbi_write_jpg("Perlin_noise.jpg", Width, Height, new_channels, newimg, 100);

	stbi_image_free(img);
	stbi_image_free(newimg);
}

/*
    Function generate is the one 
    responsible for all the controls
    that are defined above
*/
int generate(int i, int j)
{
    float result = 0.0;
    int octaves = octav;
    float frequency = freq;
    float amplitude = ampli;

    for (int o = 0 ; o < octaves; o++)
    {
        result += amplitude * Perlin(frequency * i, frequency * j);

        frequency *=2.0;
        amplitude /=2.0;
    }

    return round(result * 255);
}

// Function for calculating a single pixel
float Perlin(float x, float y)
{
	// Base - means grid point
    int Base_x = floor(x);
    int Base_y = floor(y);

	// The point within a cell
    float Point_x = x - Base_x;
    float Point_y = y - Base_y;

    Base_x = Base_x & 255;
    Base_y = Base_y & 255;

	// declaring a structure of 4 vectors
	// that point to the point that is being calculated
    struct vector Cell[4];

    assign_vector_values(Cell, Point_x, Point_y, Base_x, Base_y);

	// calculating fade of x and y
    float Fade_x = Fade(Point_x);
    float Fade_y = Fade(Point_y);

    // 0 - top right   1 - top left    2 - bottom right    3 - bottom left
	// Another one of Perlin's shenanigans
    float a = Lerp(Cell[3].dot_prouct, Cell[1].dot_prouct,  Fade_y);
    float b = Lerp(Cell[2].dot_prouct, Cell[0].dot_prouct, Fade_y);

	// The returned value is the Lerp of Lerp of left two grid points
	// Lerp of two right grid points and Fade of x
	// we add 1 so that we get a positive value
	// and divide by two so the values are between 0 and 1
    return (Lerp(a, b, Fade_x) + 1.0)/2.0;
}

void assign_vector_values(struct vector temp[], float Point_x, float Point_y, int Base_x, int Base_y)
{
    // Iteration:   0 - top right   1 - top left    2 - bottom right    3 - bottom left

    for(int i = 0; i < 4; i++)
    {
        temp[i].distance_vector_x = Point_x - ((i+1) & 1); // for 0 and 2
        temp[i].distance_vector_y = Point_y - (1.0 - ((float)(i&2)*0.5)); // for 0 and 1
        temp[i].permutation_value = permutation[permutation[Base_x + ((i+1) & 1)] + Base_y + (1 - (i&2)/2)];// same here +1 for X where i is 0 and 2, +1 for Y where 0 and 1

        int rest = temp[i].permutation_value & 3;
        temp[i].dot_prouct = (-1.0 * (-1.0 + (rest&2)))*temp[i].distance_vector_x + (-1.0 + 2*((rest+1) & 1))*temp[i].distance_vector_y;
        // 1, 1 for i = 0;  1, -1 for i = 1;    -1, 1 for i = 2;    -1, -1 for i = 3
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

// The Fade function for the smoothnes
// Simply put it's the function Perlin 
// used
float Fade(float t){
    return t * t * t *(t * (t * 6 - 15) + 10);
}

// Another part of the Perlin's algorithm
float Lerp(float a, float b, float t){
    return a + t*(b-a);
}
