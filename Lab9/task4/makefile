virus: task4.o
	ld -melf_i386 task4.o -o virus
task4.o : task4.s
	nasm -f elf -g task4.s -o task4.o -l task4.lst
clean:
	rm task4.o task4.lst virus