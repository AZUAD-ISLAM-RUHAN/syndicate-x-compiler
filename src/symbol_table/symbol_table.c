#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* Global variable - shobshomoy amra jani ekhon kon scope-e achi */
static Scope *current_scope = NULL;

void enter_scope(void) {
    Scope *new_scope = malloc(sizeof(Scope));
    new_scope->symbols = NULL;
    new_scope->parent = current_scope;
    new_scope->level = current_scope ? current_scope->level + 1 : 0;
    current_scope = new_scope;
}

/* Ei scope er shob variable print kore dekhay - report/viva te
   dekhanor jonno khub kaje lagbe, ei table tai proof je
   nested scope thik moto kaj korche */
static void print_current_scope(void) {
    printf("\n--- Symbol Table (Scope Level %d) ---\n", current_scope->level);

    if (current_scope->symbols == NULL) {
        printf("  (no variables declared in this scope)\n");
        return;
    }

    printf("  %-15s %-10s %-6s\n", "Name", "Type", "Line");
    printf("  --------------------------------\n");

    Symbol *s = current_scope->symbols;
    while (s != NULL) {
        printf("  %-15s %-10s %-6d\n", s->name, s->type, s->line);
        s = s->next;
    }
}

void exit_scope(void) {
    print_current_scope();

    Scope *old_scope = current_scope;
    current_scope = current_scope->parent;

    /* Purono scope er memory free kore dicchi */
    Symbol *s = old_scope->symbols;
    while (s != NULL) {
        Symbol *next = s->next;
        free(s->name);
        free(s->type);
        free(s);
        s = next;
    }
    free(old_scope);
}

int insert_symbol(char *name, char *type, int line) {
    /* Age check kori - EI scope-e (bairer scope na) same naam e
       age theke kono variable ache kina. Thakle - redeclaration error */
    Symbol *s = current_scope->symbols;
    while (s != NULL) {
        if (strcmp(s->name, name) == 0) {
            return -1;  /* Already declared - error */
        }
        s = s->next;
    }

    /* Notun symbol banie current scope-er list er shamne jog kori */
    Symbol *new_symbol = malloc(sizeof(Symbol));
    new_symbol->name = strdup(name);
    new_symbol->type = strdup(type);
    new_symbol->line = line;
    new_symbol->next = current_scope->symbols;
    current_scope->symbols = new_symbol;

    return 0;
}

Symbol *lookup_symbol(char *name) {
    /* Current scope theke shuru kore, dhire dhire parent
       (bairer) scope porjonto khuji */
    Scope *scope = current_scope;
    while (scope != NULL) {
        Symbol *s = scope->symbols;
        while (s != NULL) {
            if (strcmp(s->name, name) == 0) {
                return s;  /* Pawa geche */
            }
            s = s->next;
        }
        scope = scope->parent;
    }
    return NULL;  /* Kothao pawa jayni - undeclared */
}
