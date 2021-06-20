# A simple Makefile
passenger: passenger.o
	gcc -Wall -g passenger.o -o passenger -lpthread -lrt

passenger.o: passenger.c
	gcc -Wall -g -c passenger.c

train: train.o
	gcc -Wall -g train.o -o train -lpthread -lrt

train.o: train.c
	gcc -Wall -g -c train.c

run: train
	./train && ./passenger.bin

clean:
	rm *.o *.bin
	rm /dev/shm/*
