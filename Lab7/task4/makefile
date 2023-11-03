all: hexeditplus

hexeditplus: hexeditplus.o
	gcc -g -m32 -Wall -o hexeditplus hexeditplus.o
hexeditplus.o: hexeditplus.c
	gcc -g -m32 -ansi -Wall -c hexeditplus.c
clean:
	rm hexeditplus hexeditplus.o
dc: digit_counter.c
	gcc -g -m32 -ansi -Wall -c digit_counter.c
	gcc -g -m32 -Wall -o digit_counter digit_counter.o
dc-clean:
	rm digit_counter digit_counter.o