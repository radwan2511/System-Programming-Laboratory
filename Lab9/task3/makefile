virus: task3.o
	ld -melf_i386 task3.o -o virus
task3.o : task3.s
	nasm -f elf -g task3.s -o task3.o -l task3.lst
clean:
	rm task3.o task3.lst virus