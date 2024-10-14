main: compile run

compile:
	gcc -std=c17 -Wall -Wl,--stack,16777216 -pedantic Perlin.c -o Generate -lm
run:
	./Generate.exe
