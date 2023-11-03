all: task3

task3: task3.o
	gcc -g -m32 -Wall -o myElf task3.o
task3.o: task3.c
	gcc -g -m32 -ansi -Wall -c task3.c
clean:
	rm myElf task3.o
