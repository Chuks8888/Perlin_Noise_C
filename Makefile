main: compile run

compile:
	gcc -std=c17 -Wall -pedantic Perlin.c -o Generate -lm
run:
	./Generate
