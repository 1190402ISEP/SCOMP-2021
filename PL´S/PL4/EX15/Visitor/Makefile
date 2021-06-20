Visitor: visitor.o
	gcc visitor.o -o Visitor -lrt -lpthread

visitor.o: visitor.c
	gcc -g -Wall -c visitor.c -o visitor.o -lrt -lpthread

clean:
	rm -f *.o Visitor

run: Visitor
	./Visitor