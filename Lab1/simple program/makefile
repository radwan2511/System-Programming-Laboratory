my_echo : my_echo.o
	gcc -Wall -m32 -g -o my_echo my_echo.o
my_echo.o : my_echo.c
	gcc -Wall -m32 -g -c my_echo.c
clean : 
	rm my_echo.o my_echo