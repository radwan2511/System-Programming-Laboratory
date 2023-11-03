myshell: myshell.o LineParser.o
	gcc -ansi -Wall -m32 -o myshell myshell.o LineParser.o
myshell.o : myshell.c
	gcc -ansi -Wall -c -g -m32 -o  myshell.o myshell.c
LineParser.o : LineParser.c
	gcc -ansi -Wall -c -g -m32 -o  LineParser.o LineParser.c
clean:
	rm LineParser.o myshell.o myshell