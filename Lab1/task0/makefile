 
summy : main.o numbers.o add.o
	gcc -g -m32 -Wall -o summy main.o numbers.o add.o
main.o : main.c numbers.o add.o
	gcc -g -m32 -ansi -Wall -c main.c
numbers.o : numbers.c add.o
	gcc -g -m32 -ansi -Wall -c numbers.c
add.o : add.s
	nasm -g -f elf -w+all add.s

clean : 
	rm summy main.o numbers.o add.o