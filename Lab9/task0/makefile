virus: task0.o
	ld -melf_i386 task0.o -o virus
task0.o : task0.s
	nasm -f elf -g task0.s -o task0.o -l task0.lst
clean:
	rm task0.o task0.lst virus