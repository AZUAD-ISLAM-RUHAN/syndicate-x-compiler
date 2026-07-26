#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "ast.h"

ASTNode *new_leaf(NodeType type, char *value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->children = NULL;
    node->num_children = 0;
    return node;
}

ASTNode *new_node(NodeType type, char *value, int num_children, ...) {
    ASTNode *node = new_leaf(type, value);
    if (num_children > 0) {
        node->children = malloc(sizeof(ASTNode *) * num_children);
        va_list args;
        va_start(args, num_children);
        for (int i = 0; i < num_children; i++) {
            node->children[i] = va_arg(args, ASTNode *);
        }
        va_end(args);
        node->num_children = num_children;
    }
    return node;
}

void add_child(ASTNode *parent, ASTNode *child) {
    parent->num_children++;
    parent->children = realloc(parent->children,
                                sizeof(ASTNode *) * parent->num_children);
    parent->children[parent->num_children - 1] = child;
}

const char *node_type_name(NodeType type) {
    switch (type) {
        case NODE_PROGRAM: return "Program";
        case NODE_DECL:    return "Decl";
        case NODE_ASSIGN:  return "Assign";
        case NODE_IF:      return "If";
        case NODE_WHILE:   return "While";
        case NODE_PRINT:   return "Print";
        case NODE_BLOCK:   return "Block";
        case NODE_BINOP:   return "BinOp";
        case NODE_UNOP:    return "UnOp";
        case NODE_ID:      return "ID";
        case NODE_INT:     return "IntConst";
        case NODE_FLOAT:   return "FloatConst";
        case NODE_BOOL:    return "BoolConst";
        case NODE_TYPE:    return "Type";
        default:           return "Unknown";
    }
}

void print_ast(ASTNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    if (node->value) {
        printf("%s: %s\n", node_type_name(node->type), node->value);
    } else {
        printf("%s\n", node_type_name(node->type));
    }
    for (int i = 0; i < node->num_children; i++) {
        print_ast(node->children[i], depth + 1);
    }
}
