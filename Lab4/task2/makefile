flame2: start.o flame2.o util.o
	ld -melf_i386 start.o flame2.o util.o -o flame2
start.o : start.s
	nasm -f elf start.s -o start.o -l start.lst
util.o flame2.o : util.c flame2.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector flame2.c -o flame2.o
clean:
	rm start.o flame2.o util.o flame2 start.lst