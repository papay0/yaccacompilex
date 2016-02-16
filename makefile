CC=gcc
LD=gcc
INCLUDE=-Iinclude
CCFLAGS=-g -std=c99 
LDFLAGS=-g -ll

all: bin/parser

bin/parser: build/y.tab.o build/lex.yy.o build/compiler.o build/stable.o
	$(LD) build/y.tab.o build/lex.yy.o build/compiler.o build/stable.o $(LDFLAGS) -o bin/parser

build/lex.yy.c: src/source.lex
	flex src/source.lex
	mv lex.yy.c build/lex.yy.c

build/y.tab.c: src/source.yacc
	yacc -d src/source.yacc
	mv y.tab.c build/y.tab.c
	mv y.tab.h include/y.tab.h
	echo "int getMode() { return 1; }" >> build/y.tab.c	

build/y.tab.o: build/y.tab.c
	$(CC) build/y.tab.c $(CCFLAGS) $(INCLUDE) -c -o build/y.tab.o

build/lex.yy.o: build/lex.yy.c
	$(CC) build/lex.yy.c $(CCFLAGS) $(INCLUDE) -c -o build/lex.yy.o

build/compiler.o: src/compiler.c
	$(CC) src/compiler.c $(CCFLAGS) $(INCLUDE) -c -o build/compiler.o

build/stable.o: src/stable.c
	$(CC) src/stable.c $(CCFLAGS) $(INCLUDE) -c -o build/stable.o
	
clean:
	rm build/*
