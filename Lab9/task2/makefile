virus: task2.o
	ld -melf_i386 task2.o -o virus
task2.o : task2.s
	nasm -f elf -g task2.s -o task2.o -l task2.lst
clean:
	rm task2.o task2.lst virus