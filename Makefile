CC=		gcc
CFLAGS=	-Wall -g -std=c99


bminor: encoder.o main.o
	$(CC) $(CFLAGS) encoder.o main.o -o bminor

encoder.o: encoder.c
	$(CC) -c $(CFLAGS) encoder.c -o encoder.o

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

clean:
	rm *.o
	rm bminor
	rm tests/encode/*.out
