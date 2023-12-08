CC=		gcc
CFLAGS=	-Wall -g -lm

bminor: parse.o encoder.o main.o scanner.o scanner_io.o decl.o expr.o param_list.o stmt.o type.o symbol.o hash_table.o scope.o scratch.o label.o
	$(CC) $(CFLAGS) encoder.o main.o scanner.o scanner_io.o parse.o decl.o expr.o param_list.o stmt.o type.o symbol.o hash_table.o scope.o scratch.o label.o -o bminor

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

encoder.o: encoder.c
	$(CC) -c $(CFLAGS) encoder.c -o encoder.o

parse.o:	parse.c
	$(CC) -c $(CFLAGS) parse.c -o parse.o

parse.c:	parse.bison
	bison --defines=parse.h --output=parse.c -v parse.bison

scanner_io.o: scanner_io.c
	$(CC) -c $(CFLAGS) scanner_io.c -o scanner_io.o

scanner.o: scanner.c
	$(CC) -c $(CFLAGS) scanner.c -o scanner.o

scanner.c: scanner.flex
	flex -oscanner.c scanner.flex

decl.o: decl.c
	$(CC) -c $(CFLAGS) decl.c -o decl.o

stmt.o: stmt.c
	$(CC) -c $(CFLAGS) stmt.c -o stmt.o

expr.o: expr.c
	$(CC) -c $(CFLAGS) expr.c -o expr.o

type.o: type.c
	$(CC) -c $(CFLAGS) type.c -o type.o

param_list.o: param_list.c
	$(CC) -c $(CFLAGS) param_list.c -o param_list.o

symbol.o: symbol.c
	$(CC) -c $(CFLAGS) symbol.c -o symbol.o

hash_table.o: hash_table.c
	$(CC) -c $(CFLAGS) hash_table.c -o hash_table.o

scope.o: scope.c
	$(CC) -c $(CFLAGS) scope.c -o scope.o

scratch.o: scratch.c
	$(CC) -c $(CFLAGS) scratch.c -o scratch.o

label.o: label.c
	$(CC) -c $(CFLAGS) label.c -o label.o

test: bminor runtest_encoder.sh runtest_scanner.sh
	@echo "Encoder Tests:"
	./runtest_encoder.sh
	@echo ""
	@echo "Scanner Tests:"
	./runtest_scanner.sh
	@echo ""
	@echo "Parser Tests:"
	./runtest_parser.sh
	@echo ""
	@echo "Printer Tests:"
	./runtest_printer.sh
	@echo ""
	@echo "Resolver Tests:"
	./runtest_resolver.sh
	@echo ""
	@echo "Typechecker Tests:"
	./runtest_typechecker.sh
	@echo ""
	@echo "Code Generator Tests:"
	./runtest_codegenerator.sh

clean: 
	rm -rf *.o
	rm -rf bminor
	rm -rf ./tests/encode/*.out
	rm -rf ./tests/scan/*.out
	rm -rf ./tests/parse/*.out
	rm -rf ./tests/printer/*.out
	rm -rf ./tests/resolve/*.out
	rm -rf ./tests/typecheck/*.out
	rm -rf ./tests/codegen/*.out
	rm -rf ./tests/codegen/*.s
