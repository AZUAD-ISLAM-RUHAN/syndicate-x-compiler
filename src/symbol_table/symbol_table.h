#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/* ============================================================
   symbol_table.h
   Phase 4: Symbol Table with nested scope support
   ============================================================

   Ekta scope k amra ekta "Scope" struct diye represent korchi.
   Protita scope er nijer symbol (variable) list ache, ar ekta
   "parent" pointer ache jeta bole dey eta kon scope er bhitore
   ache (jemon if/while block ta kon function/global scope er
   bhitore).

   Notun block ({ }) shuru hole notun Scope banie current_scope
   ke update kori (push). Block shesh hole shei scope theke
   ber hoye ashi (pop) - tokhon shei scope er variable gulo r
   dekha jabe na.
*/

typedef struct Symbol {
    char *name;
    char *type;      /* "int", "float", "bool" */
    int line;         /* Kon line e declare hoyeche */
    struct Symbol *next;  /* Ei scope er porer symbol (linked list) */
} Symbol;

typedef struct Scope {
    Symbol *symbols;      /* Ei scope er shob variable */
    int level;             /* 0 = global, 1 = first nested block, ityadi */
    struct Scope *parent;  /* Bairer scope - NULL hole eta global scope */
} Scope;

/* Notun scope shuru koro (block-e dhukle call hobe) */
void enter_scope(void);

/* Current scope shesh koro - ei scope er symbol table print kore
   dekhabe, tarpor bairer scope-e fire jabe */
void exit_scope(void);

/* Notun variable current scope-e add koro.
   Return: 0 hole success, -1 hole mane already ei scope-e
   ei naam e ekta variable declare kora ache (redeclaration) */
int insert_symbol(char *name, char *type, int line);

/* Variable ta খুঁজে বের koro - current scope theke shuru kore
   dhire dhire bairer scope porjonto khoje dekhe.
   Pele Symbol* return kore, na pele NULL */
Symbol *lookup_symbol(char *name);

#endif
