/* ast.h - FINAL WORKING VERSION */
#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
    VAL_TYPE_INT,
    VAL_TYPE_STR
} ValType;
struct Value
{
    ValType type;
    union
    {
        int ival;
        char *sval;
    };
};

typedef enum
{
    NODE_TYPE_LIST,
    NODE_TYPE_VAL,
    NODE_TYPE_ID,
    NODE_TYPE_ASSIGN,
    NODE_TYPE_SPILL,
    NODE_TYPE_IF,
    NODE_TYPE_WHILE,
    NODE_TYPE_OP
} NodeType;
struct ASTNode
{
    NodeType type;
    union
    {
        struct
        {
            struct ASTNode *left;
            struct ASTNode *right;
        } list;
        struct Value val;
        char *name;
        struct
        {
            char *name;
            struct ASTNode *expr;
        } assign;
        struct
        {
            struct ASTNode *expr;
        } spill;
        struct
        {
            int op;
            struct ASTNode *left;
            struct ASTNode *right;
        } op;
        struct
        {
            struct ASTNode *cond;
            struct ASTNode *if_branch;
            struct ASTNode *else_branch;
        } branch;
    } data;
};

struct ASTNode *create_node_list(struct ASTNode *stmt, struct ASTNode *next);
struct ASTNode *create_node_val(struct Value val);
struct ASTNode *create_node_id(char *name);
struct ASTNode *create_node_op(int op, struct ASTNode *left, struct ASTNode *right);
struct ASTNode *create_node_assign(char *name, struct ASTNode *expr);
struct ASTNode *create_node_spill(struct ASTNode *expr);
struct ASTNode *create_node_if(struct ASTNode *cond, struct ASTNode *if_branch, struct ASTNode *else_branch);
struct ASTNode *create_node_while(struct ASTNode *cond, struct ASTNode *loop_body);
struct Value execute_ast(struct ASTNode *node);
#endif