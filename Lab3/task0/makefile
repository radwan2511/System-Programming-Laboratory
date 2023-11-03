all: bubblesort hexprint

hexprint: hexprint.o
	gcc -ansi -Wall -m32 -o hexprint hexprint.o
hexprint.o: hexprint.c
	gcc -ansi -Wall -c -g -m32 -o hexprint.o hexprint.c

bubblesort: bubblesort.o
	gcc -m32 -o bubblesort bubblesort.o
bubblesort.o: bubblesort.c
	gcc -c -g -m32 -o bubblesort.o bubblesort.c
	
clean:
	rm bubblesort bubblesort.o hexprint hexprint.o