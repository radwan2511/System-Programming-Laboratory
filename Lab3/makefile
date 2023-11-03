
# Tool invocations
# Executable "hello" depends on the files hello.o and run.o.
virusDetector: virusDetector.o
	gcc -g -m32 -Wall -o virusDetector virusDetector.o

# Depends on the source and header files
virusDetector.o: virusDetector.c
	gcc -m32 -g -Wall -ansi -c -o virusDetector.o virusDetector.c
 

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean: 
	rm -f *.o virusDetector