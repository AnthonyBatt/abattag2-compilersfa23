CC=		gcc
CFLAGS=	-Wall -g -std=c99

bminor: encoder.o main.o scanner.o scanner_io.o
	$(CC) $(CFLAGS) encoder.o main.o scanner.o scanner_io.o -o bminor

encoder.o: encoder.c
	$(CC) -c $(CFLAGS) encoder.c -o encoder.o

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

scanner.c: scanner.flex
	flex -oscanner.c scanner.flex

scanner.o: scanner.c
	$(CC) -c $(CFLAGS) scanner.c -o scanner.o

scanner_io.o: scanner_io.c
	$(CC) -c $(CFLAGS) scanner_io.c -o scanner_io.o

test: bminor runtest_encoder.sh
	@echo "Encoder Tests:"
	./runtest_encoder.sh
	@echo "Scanner Tests:"
	./runtest_scanner.sh

clean:
	rm *.o
	rm bminor
	rm tests/encode/*.out
