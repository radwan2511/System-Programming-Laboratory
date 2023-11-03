all: task0

task0: task0.o
	gcc -g -m32 -Wall -o myElf task0.o
task0.o: task0.c
	gcc -g -m32 -ansi -Wall -c task0.c
clean:
	rm myElf task0.o
