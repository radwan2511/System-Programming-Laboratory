all: task2

task2: task2.o
	gcc -g -m32 -Wall -o myElf task2.o
task2.o: task2.c
	gcc -g -m32 -ansi -Wall -c task2.c
clean:
	rm myElf task2.o
