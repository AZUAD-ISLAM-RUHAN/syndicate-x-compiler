#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "semantic.h"

extern int semantic_errors;
extern int line_num;

static char *check_arithmetic(char *op, char *lt, char *rt) {
    if (strcmp(lt, "bool") == 0 || strcmp(rt, "bool") == 0) {
        printf("Semantic Error at line %d: Invalid expression - arithmetic operator '%s' cannot be applied to bool operand\n", line_num, op);
        semantic_errors++;
        return "error";
    }
    if (strcmp(lt, "float") == 0 || strcmp(rt, "float") == 0) {
        return "float";
    }
    return "int";
}

static char *check_relational(char *op, char *lt, char *rt) {
    int left_is_bool = (strcmp(lt, "bool") == 0);
    int right_is_bool = (strcmp(rt, "bool") == 0);

    if (left_is_bool != right_is_bool) {
        printf("Semantic Error at line %d: Type mismatch - cannot compare '%s' with '%s' using '%s'\n", line_num, lt, rt, op);
        semantic_errors++;
        return "error";
    }
    return "bool";
}

static char *check_logical(char *op, char *lt, char *rt) {
    if (strcmp(lt, "bool") != 0 || strcmp(rt, "bool") != 0) {
        printf("Semantic Error at line %d: Invalid expression - logical operator '%s' requires bool operands, got '%s' and '%s'\n", line_num, op, lt, rt);
        semantic_errors++;
        return "error";
    }
    return "bool";
}

char *get_expr_type(ASTNode *node) {
    if (!node) return "error";

    switch (node->type) {

        case NODE_ID: {
            Symbol *s = lookup_symbol(node->value);
            if (!s) {
                printf("Semantic Error at line %d: Undeclared variable '%s'\n", line_num, node->value);
                semantic_errors++;
                return "error";
            }
            return s->type;
        }

        case NODE_INT:
            return "int";

        case NODE_FLOAT:
            return "float";

        case NODE_BOOL:
            return "bool";

        case NODE_UNOP: {
            char *operand_type = get_expr_type(node->children[0]);
            if (strcmp(operand_type, "bool") != 0) {
                printf("Semantic Error at line %d: Invalid expression - '!' requires a bool operand, got '%s'\n", line_num, operand_type);
                semantic_errors++;
                return "error";
            }
            return "bool";
        }

        case NODE_BINOP: {
            char *lt = get_expr_type(node->children[0]);
            char *rt = get_expr_type(node->children[1]);
            char *op = node->value;

            if (strcmp(lt, "error") == 0 || strcmp(rt, "error") == 0) {
                return "error";
            }

            if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
                strcmp(op, "*") == 0 || strcmp(op, "/") == 0 || strcmp(op, "%") == 0) {
                return check_arithmetic(op, lt, rt);
            }

            if (strcmp(op, "<") == 0 || strcmp(op, ">") == 0 ||
                strcmp(op, "<=") == 0 || strcmp(op, ">=") == 0 ||
                strcmp(op, "==") == 0 || strcmp(op, "!=") == 0) {
                return check_relational(op, lt, rt);
            }

            if (strcmp(op, "&&") == 0 || strcmp(op, "||") == 0) {
                return check_logical(op, lt, rt);
            }

            return "error";
        }

        default:
            return "error";
    }
}
