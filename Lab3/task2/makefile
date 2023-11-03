all: task2 task2Bonus
	
task2Bonus: task2Bonus.o
	gcc -ansi -Wall -m32 -o virusCleaner task2Bonus.o
task2Bonus.o: task2Bonus.c
	gcc -ansi -Wall -c -g -m32 -o task2Bonus.o task2Bonus.c
	
	
task2: task2.o
	gcc -ansi -Wall -m32 -o virusDetector task2.o
task2.o: task2.c
	gcc -ansi -Wall -c -g -m32 -o task2.o task2.c
	
clean:
	rm virusDetector task2.o virusCleaner task2Bonus.o