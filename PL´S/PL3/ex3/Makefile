Pipes: Pipes.o
	gcc Pipes.o -o Pipes

Pipes.o: Pipes.c
	gcc -g -Wall -c Pipes.c -o Pipes.o

SharedMemory: SharedMemory.o
	gcc -Wall -g SharedMemory.o -o SharedMemory.bin -lrt

SharedMemory.o: SharedMemory.c
	gcc -Wall -g -c SharedMemory.c

run: Pipes SharedMemory
	./SharedMemory.bin
	./Pipes


clean:
	rm *.o *.bin

