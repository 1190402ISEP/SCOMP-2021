ShowRoom: showRoom.o
	gcc showRoom.o -o ShowRoom -lrt -lpthread

showRoom.o: showRoom.c
	gcc -g -Wall -c showRoom.c -o showRoom.o -lrt -lpthread

clean:
	rm -f *.o ShowRoom

run: ShowRoom
	./ShowRoom