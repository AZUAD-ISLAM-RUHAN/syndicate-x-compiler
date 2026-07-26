#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_DECL,
    NODE_ASSIGN,
    NODE_IF,
    NODE_WHILE,
    NODE_PRINT,
    NODE_BLOCK,
    NODE_BINOP,
    NODE_UNOP,
    NODE_ID,
    NODE_INT,
    NODE_FLOAT,
    NODE_BOOL,
    NODE_TYPE
} NodeType;

typedef struct ASTNode {
    NodeType type;
    char *value;
    struct ASTNode **children;
    int num_children;
} ASTNode;

ASTNode *new_leaf(NodeType type, char *value);
ASTNode *new_node(NodeType type, char *value, int num_children, ...);
void add_child(ASTNode *parent, ASTNode *child);
void print_ast(ASTNode *node, int depth);
const char *node_type_name(NodeType type);

#endif
