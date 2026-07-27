%code requires {
    #include "ast.h"
}

%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "symbol_table.h"

int yylex(void);
void yyerror(const char *s);
extern int line_num;
ASTNode *root;
int semantic_errors = 0;
%}

%define api.value.type {ASTNode *}

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
    {
        root = new_node(NODE_PROGRAM, NULL, 1, $1);
        printf("Parsing successful. No syntax errors.\n");
        printf("\n--- Abstract Syntax Tree ---\n");
        print_ast(root, 0);
    }
    ;

statement_list:
      /* empty */ { $$ = new_node(NODE_BLOCK, NULL, 0); }
    | statement_list statement { add_child($1, $2); $$ = $1; }
    ;

statement:
      declaration { $$ = $1; }
    | assignment { $$ = $1; }
    | if_stmt { $$ = $1; }
    | while_stmt { $$ = $1; }
    | print_stmt { $$ = $1; }
    | block { $$ = $1; }
    ;

declaration:
    type ID SEMI
    {
        $$ = new_node(NODE_DECL, NULL, 2, $1, $2);
        if (insert_symbol($2->value, $1->value, line_num) == -1) {
            printf("Semantic Error at line %d: Variable '%s' already declared in this scope\n", line_num, $2->value);
            semantic_errors++;
        }
    }
    ;

type:
      INT_KW { $$ = new_leaf(NODE_TYPE, "int"); }
    | FLOAT_KW { $$ = new_leaf(NODE_TYPE, "float"); }
    | BOOL_KW { $$ = new_leaf(NODE_TYPE, "bool"); }
    ;

assignment:
    ID ASSIGN expr SEMI
    { $$ = new_node(NODE_ASSIGN, NULL, 2, $1, $3); }
    ;

if_stmt:
      IF LPAREN expr RPAREN block
      { $$ = new_node(NODE_IF, NULL, 2, $3, $5); }
    | IF LPAREN expr RPAREN block ELSE block
      { $$ = new_node(NODE_IF, "else", 3, $3, $5, $7); }
    ;

while_stmt:
    WHILE LPAREN expr RPAREN block
    { $$ = new_node(NODE_WHILE, NULL, 2, $3, $5); }
    ;

print_stmt:
    PRINT expr SEMI
    { $$ = new_node(NODE_PRINT, NULL, 1, $2); }
    ;

block:
    LBRACE { enter_scope(); } statement_list RBRACE
    {
        exit_scope();
        $$ = $3;
    }
    ;

expr:
      expr PLUS expr   { $$ = new_node(NODE_BINOP, "+", 2, $1, $3); }
    | expr MINUS expr  { $$ = new_node(NODE_BINOP, "-", 2, $1, $3); }
    | expr TIMES expr  { $$ = new_node(NODE_BINOP, "*", 2, $1, $3); }
    | expr DIVIDE expr { $$ = new_node(NODE_BINOP, "/", 2, $1, $3); }
    | expr MOD expr    { $$ = new_node(NODE_BINOP, "%", 2, $1, $3); }
    | expr LT expr     { $$ = new_node(NODE_BINOP, "<", 2, $1, $3); }
    | expr GT expr     { $$ = new_node(NODE_BINOP, ">", 2, $1, $3); }
    | expr LE expr     { $$ = new_node(NODE_BINOP, "<=", 2, $1, $3); }
    | expr GE expr     { $$ = new_node(NODE_BINOP, ">=", 2, $1, $3); }
    | expr EQ expr     { $$ = new_node(NODE_BINOP, "==", 2, $1, $3); }
    | expr NEQ expr    { $$ = new_node(NODE_BINOP, "!=", 2, $1, $3); }
    | expr AND expr    { $$ = new_node(NODE_BINOP, "&&", 2, $1, $3); }
    | expr OR expr     { $$ = new_node(NODE_BINOP, "||", 2, $1, $3); }
    | NOT expr         { $$ = new_node(NODE_UNOP, "!", 1, $2); }
    | LPAREN expr RPAREN { $$ = $2; }
    | ID    { $$ = $1; }
    | INT_CONST { $$ = $1; }
    | FLOAT_CONST { $$ = $1; }
    | TRUE_KW { $$ = new_leaf(NODE_BOOL, "true"); }
    | FALSE_KW { $$ = new_leaf(NODE_BOOL, "false"); }
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
    enter_scope();
    yyparse();
    exit_scope();
    return 0;
}
