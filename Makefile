CC=		gcc
CFLAGS=	-Wall -g -std=c99 -lm

bminor: encoder.o main.o scanner.o scanner_io.o parse.o
	$(CC) $(CFLAGS) encoder.o main.o scanner.o scanner_io.o parse.o -o bminor

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

encoder.o: encoder.c
	$(CC) -c $(CFLAGS) encoder.c -o encoder.o

scanner_io.o: scanner_io.c
	$(CC) -c $(CFLAGS) scanner_io.c -o scanner_io.o

scanner.o: scanner.c
	$(CC) -c $(CFLAGS) scanner.c -o scanner.o

scanner.c: scanner.flex
	flex -oscanner.c scanner.flex

parse.o:	parse.c
	$(CC) -c $(CFLAGS) parse.c -o parse.o

parse.c:	parse.bison
	bison --defines=parse.h --output=parse.c -v parse.bison

test: bminor runtest_encoder.sh runtest_scanner.sh
	@echo "Encoder Tests:"
	./runtest_encoder.sh
	@echo "Scanner Tests:"
	./runtest_scanner.sh
	@echo "Parser Tests:"
	./runtest_parser.sh

clean: 
	rm -rf *.o
	rm -rf bminor
	rm -rf ./tests/encode/*.out
	rm -rf ./tests/scan/*.out
	rm -rf ./tests/parse/*.out
