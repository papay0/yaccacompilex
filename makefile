CC=gcc
LD=gcc
INCLUDE=-Iinclude
CCFLAGS=-g -Werror=incompatible-pointer-types -std=c99
LDFLAGS=-g -ll

all: bin/parser

tests: bin/parser 
	bin/parser < test/table.c 
	pr -m -t bin/yaccacompilex test/table.c

dtests: bin/parser
	gdb bin/parser

bin/parser: build/y.tab.o build/lex.yy.o build/compiler.o build/gtable.o build/stable.o build/idbuffer.o build/types.o build/warning.o build/instruction_stream.o build/ltable.o
	$(LD) build/y.tab.o build/lex.yy.o build/compiler.o build/gtable.o build/stable.o build/idbuffer.o build/types.o build/warning.o build/instruction_stream.o build/ltable.o $(LDFLAGS) -o bin/parser

build/lex.yy.c: src/source.lex
	flex src/source.lex
	mv lex.yy.c build/lex.yy.c

build/y.tab.c: src/source.yacc
	yacc -v -d src/source.yacc
	mv y.tab.c build/y.tab.c
	mv y.tab.h include/y.tab.h
	echo "int getMode() { return 1; }" >> build/y.tab.c

build/y.tab.o: build/y.tab.c
	$(CC) build/y.tab.c $(CCFLAGS) $(INCLUDE) -c -o build/y.tab.o

build/lex.yy.o: build/lex.yy.c
	$(CC) build/lex.yy.c $(CCFLAGS) $(INCLUDE) -c -o build/lex.yy.o

build/compiler.o: src/compiler.c include/compiler.h
	$(CC) src/compiler.c $(CCFLAGS) $(INCLUDE) -c -o build/compiler.o

build/stable.o: src/stable.c include/stable.h
	$(CC) src/stable.c $(CCFLAGS) $(INCLUDE) -c -o build/stable.o

build/gtable.o: src/gtable.c include/gtable.h
	$(CC) src/gtable.c $(CCFLAGS) $(INCLUDE) -c -o build/gtable.o

build/idbuffer.o: src/idbuffer.c include/idbuffer.h
	$(CC) src/idbuffer.c $(CCFLAGS) $(INCLUDE) -c -o build/idbuffer.o

build/types.o: src/types.c include/types.h
	$(CC) src/types.c $(CCFLAGS) $(INCLUDE) -c -o build/types.o

build/warning.o: src/warning.c include/warning.h
	$(CC) src/warning.c $(CCFLAGS) $(INCLUDE) -c -o build/warning.o

build/instruction_stream.o: src/instruction_stream.c
		$(CC) src/instruction_stream.c $(CCFLAGS) $(INCLUDE) -c -o build/instruction_stream.o

build/ltable.o: src/ltable.c
		$(CC) src/ltable.c $(CCFLAGS) $(INCLUDE) -c -o build/ltable.o

clean:
	rm build/*
	rm bin/yaccacompilex
