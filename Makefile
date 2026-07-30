CC = gcc
CFLAGS = -I src/parser -I src/ast -I src/symbol_table -I src/semantic -I src/tac

all: compiler

compiler: src/parser/y.tab.c src/lexer/lex.yy.c src/ast/ast.c src/symbol_table/symbol_table.c src/semantic/semantic.c src/tac/tac.c
	$(CC) $^ $(CFLAGS) -o compiler

src/parser/y.tab.c: src/parser/parser.y
	bison -d -o src/parser/y.tab.c src/parser/parser.y

src/lexer/lex.yy.c: src/lexer/lexer.l
	flex -o src/lexer/lex.yy.c src/lexer/lexer.l

clean:
	rm -f src/parser/y.tab.c src/parser/y.tab.h src/lexer/lex.yy.c compiler

.PHONY: all clean
