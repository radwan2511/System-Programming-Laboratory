all: task1c task1b task1a

task1c: task1c.o
	gcc -ansi -Wall -m32 -o task1c task1c.o
task1c.o: task1c.c
	gcc -ansi -Wall -c -g -m32 -o task1c.o task1c.c

task1b: task1b.o
	gcc -ansi -Wall -m32 -o task1b task1b.o
task1b.o: task1b.c
	gcc -ansi -Wall -c -g -m32 -o task1b.o task1b.c
	
task1a: task1a.o
	gcc -ansi -Wall -m32 -o task1a task1a.o
task1a.o: task1a.c
	gcc -ansi -Wall -c -g -m32 -o task1a.o task1a.c
	
clean:
	rm task1c task1c.o task1b task1b.o task1a task1a.o