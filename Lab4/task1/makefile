filter: start.o filter.o util.o
	ld -melf_i386 start.o filter.o util.o -o filter
start.o : start.s
	nasm -f elf start.s -o start.o
util.o filter.o : util.c filter.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector filter.c -o filter.o
clean:
	rm start.o filter.o util.o filter