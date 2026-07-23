%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_num = 1;
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID          {LETTER}({LETTER}|{DIGIT})*
INT_CONST   {DIGIT}+
FLOAT_CONST {DIGIT}+"."{DIGIT}+

%%

[ \t\r]+        { }
\n              { line_num++; }

"//".*                          { }
"/*"([^*]|\*+[^*/])*\*+"/"      { }

"int"       { printf("<KEYWORD, int>\n");    }
"float"     { printf("<KEYWORD, float>\n");  }
"bool"      { printf("<KEYWORD, bool>\n");   }
"if"        { printf("<KEYWORD, if>\n");     }
"else"      { printf("<KEYWORD, else>\n");   }
"while"     { printf("<KEYWORD, while>\n");  }
"print"     { printf("<KEYWORD, print>\n");  }
"true"      { printf("<BOOL_CONST, true>\n");  }
"false"     { printf("<BOOL_CONST, false>\n"); }

"+"     { printf("<OP, +>\n");  }
"-"     { printf("<OP, ->\n");  }
"*"     { printf("<OP, *>\n");  }
"/"     { printf("<OP, />\n");  }
"%"     { printf("<OP, %%>\n"); }
"<="    { printf("<OP, <=>\n"); }
">="    { printf("<OP, >=>\n"); }
"=="    { printf("<OP, ==>\n"); }
"!="    { printf("<OP, !=>\n"); }
"<"     { printf("<OP, <>\n");  }
">"     { printf("<OP, >>\n");  }
"&&"    { printf("<OP, &&>\n"); }
"||"    { printf("<OP, ||>\n"); }
"!"     { printf("<OP, !>\n");  }
"="     { printf("<ASSIGN, =>\n"); }

"{"     { printf("<LBRACE, {>\n"); }
"}"     { printf("<RBRACE, }>\n"); }
"("     { printf("<LPAREN, (>\n"); }
")"     { printf("<RPAREN, )>\n"); }
";"     { printf("<SEMI, ;>\n");   }

{FLOAT_CONST}   { printf("<FLOAT_CONST, %s>\n", yytext); }
{INT_CONST}     { printf("<INT_CONST, %s>\n", yytext);   }

{ID}            { printf("<ID, %s>\n", yytext); }

.               { printf("Lexical Error at line %d: unexpected character '%s'\n", line_num, yytext); }

%%

int yywrap(void) {
    return 1;
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            printf("Error: cannot open file %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }

    yylex();

    return 0;
}