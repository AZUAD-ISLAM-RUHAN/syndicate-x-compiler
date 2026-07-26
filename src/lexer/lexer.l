%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "y.tab.h"

int line_num = 1;
%}

DIGIT       [0-9]
LETTER      [a-zA-Z_]
ID_PATTERN  {LETTER}({LETTER}|{DIGIT})*
INT_PATTERN   {DIGIT}+
FLOAT_PATTERN {DIGIT}+"."{DIGIT}+

%%

[ \t\r]+        { }
\n              { line_num++; }

"//".*                          { }
"/*"([^*]|\*+[^*/])*\*+"/"      { }

"int"       { return INT_KW;   }
"float"     { return FLOAT_KW; }
"bool"      { return BOOL_KW;  }
"if"        { return IF;       }
"else"      { return ELSE;     }
"while"     { return WHILE;    }
"print"     { return PRINT;    }
"true"      { return TRUE_KW;  }
"false"     { return FALSE_KW; }

"+"     { return PLUS;   }
"-"     { return MINUS;  }
"*"     { return TIMES;  }
"/"     { return DIVIDE; }
"%"     { return MOD;    }
"<="    { return LE;     }
">="    { return GE;     }
"=="    { return EQ;     }
"!="    { return NEQ;    }
"<"     { return LT;     }
">"     { return GT;     }
"&&"    { return AND;    }
"||"    { return OR;     }
"!"     { return NOT;    }
"="     { return ASSIGN; }

"{"     { return LBRACE; }
"}"     { return RBRACE; }
"("     { return LPAREN; }
")"     { return RPAREN; }
";"     { return SEMI;   }

{FLOAT_PATTERN}   { yylval = new_leaf(NODE_FLOAT, yytext); return FLOAT_CONST; }
{INT_PATTERN}     { yylval = new_leaf(NODE_INT, yytext);   return INT_CONST;   }

{ID_PATTERN}      { yylval = new_leaf(NODE_ID, yytext); return ID; }

.               { printf("Lexical Error at line %d: unexpected character '%s'\n", line_num, yytext); }

%%

int yywrap(void) {
    return 1;
}
