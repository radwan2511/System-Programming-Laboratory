all: code

code: code.o
	gcc -g -m32 -Wall -o code code.o
code.o: code.c
	gcc -g -m32 -ansi -Wall -c code.c
clean:
	rm code code.o