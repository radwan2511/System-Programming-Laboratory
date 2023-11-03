all: hexeditplus

hexeditplus: hexeditplus.o
	gcc -g -m32 -Wall -o hexeditplus hexeditplus.o
hexeditplus.o: hexeditplus.c
	gcc -g -m32 -ansi -Wall -c hexeditplus.c
clean:
	rm hexeditplus hexeditplus.o