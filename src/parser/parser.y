%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror(const char *s);
extern int line_num;
%}

%token INT_KW FLOAT_KW BOOL_KW IF ELSE WHILE PRINT
%token TRUE_KW FALSE_KW
%token ID INT_CONST FLOAT_CONST
%token ASSIGN
%token PLUS MINUS TIMES DIVIDE MOD
%token LT GT LE GE EQ NEQ
%token AND OR NOT
%token LBRACE RBRACE LPAREN RPAREN SEMI

%left OR
%left AND
%right NOT
%nonassoc EQ NEQ
%nonassoc LT GT LE GE
%left PLUS MINUS
%left TIMES DIVIDE MOD

%%

program:
    statement_list
    { printf("Parsing successful. No syntax errors.\n"); }
    ;

statement_list:
    /* empty */
    | statement_list statement
    ;

statement:
    declaration
    | assignment
    | if_stmt
    | while_stmt
    | print_stmt
    | block
    ;

declaration:
    type ID SEMI
    ;

type:
    INT_KW
    | FLOAT_KW
    | BOOL_KW
    ;

assignment:
    ID ASSIGN expr SEMI
    ;

if_stmt:
    IF LPAREN expr RPAREN block
    | IF LPAREN expr RPAREN block ELSE block
    ;

while_stmt:
    WHILE LPAREN expr RPAREN block
    ;

print_stmt:
    PRINT expr SEMI
    ;

block:
    LBRACE statement_list RBRACE
    ;

expr:
    expr PLUS expr
    | expr MINUS expr
    | expr TIMES expr
    | expr DIVIDE expr
    | expr MOD expr
    | expr LT expr
    | expr GT expr
    | expr LE expr
    | expr GE expr
    | expr EQ expr
    | expr NEQ expr
    | expr AND expr
    | expr OR expr
    | NOT expr
    | LPAREN expr RPAREN
    | ID
    | INT_CONST
    | FLOAT_CONST
    | TRUE_KW
    | FALSE_KW
    ;

%%

void yyerror(const char *s) {
    printf("Syntax Error at line %d: %s\n", line_num, s);
}

int main(int argc, char **argv) {
    extern FILE *yyin;
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            printf("Error: cannot open file %s\n", argv[1]);
            return 1;
        }
    }
    yyparse();
    return 0;
}