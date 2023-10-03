CC=		gcc
CFLAGS=	-Wall -g -std=c99

decl:	main.o ../scanner.o decl.o
	gcc main.o ../scanner.o decl.o -o decl -lm

main.o: main_test_bison.c
	gcc -c -o main.o main_test_bison.c

decl.o: decl.c
	gcc -c -o decl.o decl.c

decl.c: decl.bison
	bison --defines=decl.h --output=decl.c -v decl.bison
