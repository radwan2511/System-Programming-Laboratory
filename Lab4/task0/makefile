task0: start.o main.o util.o
	ld -melf_i386 start.o main.o util.o -o task0
start.o : start.s
	nasm -f elf start.s -o start.o
util.o main.o : util.c main.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o
clean:
	rm start.o main.o util.o task0