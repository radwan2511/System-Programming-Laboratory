virus: task1.o
	ld -melf_i386 task1.o -o virus
task1.o : task1.s
	nasm -f elf -g task1.s -o task1.o -l task1.lst
clean:
	rm task1.o task1.lst virus