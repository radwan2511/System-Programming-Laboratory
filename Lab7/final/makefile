all: task1 task4

task1: task1.o
	gcc -g -m32 -Wall -o hexeditplus task1.o
task1.o: task1.c
	gcc -g -m32 -ansi -Wall -c task1.c
task4: task4.o
	gcc -g -m32 -Wall -o task4 task4.o
task4.o: task4.c
	gcc -g -m32 -ansi -Wall -c task4.c
clean:
	rm hexeditplus task1.o
	rm task4 task4.o