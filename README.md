This is a generator of Perlin noise in 2D made in C.

To generate a jpg and txt file all you need to do is type 
make in the terminal.
If you want to modify your noise you have to open Perlin.c
There in the lines 16 - 20 you have definitions which you can 
control
You can also find next to those liens some simple explanations on
what do those controls do.

maximum Width and Height
controls is 1920 and 1080 respectively, the program will most likely recieve
Segmenation fault if those controls exceed those values

Update: 
I recently tried to turn it on on Windows 11 using mingw32, there was a problem with the make file where I had to change the -std=17 to -std=11 and then it worked
