SHELL = /bin/sh
INC_DIR = ../inc
CC := gcc
CFLAGS := -std=gnu99 -g -O2 -Iinc

all: dirs bminor

dirs: 
	mkdir -p objs

objs/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

src/parser.c inc/token.h: src/parser.bison 
	bison --defines=inc/token.h --output=src/parser.c src/parser.bison

src/scanner.c: src/scanner.flex inc/token.h
	flex -o src/scanner.c src/scanner.flex

test_scanner: bminor
	./bin/run_all_tests.sh scanner

test_parser: bminor
	./bin/run_all_tests.sh parser

test_printer: bminor 
	./bin/run_all_tests.sh printer

test_typecheck: bminor
	./bin/run_all_tests.sh typecheck

test_codegen: bminor
	./bin/run_all_tests.sh codegen

test_all: bminor
	./bin/run_all_tests.sh all

bminor: objs/parser.o objs/scanner.o objs/main.o objs/type.o objs/expr.o objs/decl.o objs/stmt.o objs/param_list.o objs/symbol.o objs/scope.o objs/hash_table.o objs/scratch.o
	$(CC) $(CFLAGS) $^ -o $@
	
clean:
	@echo "Removing objects"
	rm -rf bminor objs src/scanner.c inc/token.h src/parser.c src/parser.output out.s prog output1 output2

