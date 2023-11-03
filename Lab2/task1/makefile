all: addresses t1c t1d

addresses: addresses.o
	gcc -g -m32 -Wall -o addresses addresses.o
addresses.o: addresses.c
	gcc -g -m32 -ansi -Wall -c addresses.c
t1c: t1c.o
	gcc -g -m32 -Wall -o t1c t1c.o
t1c.o: t1c.c
	gcc -g -m32 -ansi -Wall -c t1c.c
t1d: t1d.o
	gcc -g -m32 -Wall -o t1d t1d.o
t1d.o: t1d.c
	gcc -g -m32 -ansi -Wall -c t1d.c
clean:
	rm addresses addresses.o t1c t1c.o t1d t1d.o