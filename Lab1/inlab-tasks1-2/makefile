task2 : task2.o
	gcc -Wall -m32 -g -o filter task2.o
task2.o : task2.c
	gcc -Wall -m32 -g -c task2.c
task1c : task1c.o
	gcc -Wall -m32 -g -o filter task1c.o
task1c.o : task1c.c
	gcc -Wall -m32 -g -c task1c.c

clean : 
	rm task1c.o task2.o filter
