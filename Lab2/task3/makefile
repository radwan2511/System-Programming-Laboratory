all: menu

menu: menu.o
	gcc -g -m32 -Wall -o menu menu.o
menu.o: menu.c
	gcc -g -m32 -ansi -Wall -c menu.c
clean:
	rm menu menu.o