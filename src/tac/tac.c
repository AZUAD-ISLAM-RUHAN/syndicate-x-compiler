#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "tac.h"

static int temp_count = 0;
static char *new_temp(void) {
    char *buf = malloc(16);
    sprintf(buf, "t%d", ++temp_count);
    return buf;
}

static int label_count = 0;
static char *new_label(void) {
    char *buf = malloc(16);
    sprintf(buf, "L%d", ++label_count);
    return buf;
}

static void gen_stmt(ASTNode *node);

static char *gen_expr(ASTNode *node) {
    if (!node) return "";

    switch (node->type) {

        case NODE_ID:
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
            return node->value;

        case NODE_UNOP: {
            char *operand = gen_expr(node->children[0]);
            char *result = new_temp();
            printf("%s = %s%s\n", result, node->value, operand);
            return result;
        }

        case NODE_BINOP: {
            char *left = gen_expr(node->children[0]);
            char *right = gen_expr(node->children[1]);
            char *result = new_temp();
            printf("%s = %s %s %s\n", result, left, node->value, right);
            return result;
        }

        default:
            return "";
    }
}

static void gen_if(ASTNode *node) {
    char *cond_place = gen_expr(node->children[0]);

    if (node->value != NULL && strcmp(node->value, "else") == 0) {
        char *else_label = new_label();
        char *end_label = new_label();

        printf("ifFalse %s goto %s\n", cond_place, else_label);
        gen_stmt(node->children[1]);
        printf("goto %s\n", end_label);
        printf("%s:\n", else_label);
        gen_stmt(node->children[2]);
        printf("%s:\n", end_label);
    } else {
        char *end_label = new_label();

        printf("ifFalse %s goto %s\n", cond_place, end_label);
        gen_stmt(node->children[1]);
        printf("%s:\n", end_label);
    }
}

static void gen_while(ASTNode *node) {
    char *start_label = new_label();
    char *end_label = new_label();

    printf("%s:\n", start_label);
    char *cond_place = gen_expr(node->children[0]);
    printf("ifFalse %s goto %s\n", cond_place, end_label);
    gen_stmt(node->children[1]);
    printf("goto %s\n", start_label);
    printf("%s:\n", end_label);
}

static void gen_stmt(ASTNode *node) {
    if (!node) return;

    switch (node->type) {

        case NODE_BLOCK: {
            for (int i = 0; i < node->num_children; i++) {
                gen_stmt(node->children[i]);
            }
            break;
        }

        case NODE_DECL:
            break;

        case NODE_ASSIGN: {
            char *rhs_place = gen_expr(node->children[1]);
            printf("%s = %s\n", node->children[0]->value, rhs_place);
            break;
        }

        case NODE_PRINT: {
            char *place = gen_expr(node->children[0]);
            printf("print %s\n", place);
            break;
        }

        case NODE_IF:
            gen_if(node);
            break;

        case NODE_WHILE:
            gen_while(node);
            break;

        default:
            break;
    }
}

void generate_tac(ASTNode *root) {
    if (!root) return;
    gen_stmt(root->children[0]);
}
