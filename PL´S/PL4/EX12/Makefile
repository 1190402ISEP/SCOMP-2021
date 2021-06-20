main: main.o
	gcc -Wall -g main.o -o main.bin -lpthread -lrt

main.o: main.c
	gcc -Wall -g -c main.c

client: client.o
	gcc -Wall -g client.o -o client -lrt

client.o: client.c
	gcc -Wall -g -c client.c

seller: seller.o
	gcc -Wall -g seller.o -o seller -lrt

seller.o: seller.c
	gcc -Wall -g -c seller.c

run: main
	./main.bin

clean:
	rm *.o *.bin
	rm /dev/shm/*