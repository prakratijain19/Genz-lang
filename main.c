/* main.c - FINAL ERROR-FREE VERSION */
#include "ast.h"
#include "genz.tab.h"

extern FILE *yyin;

/* --- SYMBOL TABLE (and other functions from before) --- */
#define MAX_SYMBOLS 100
struct Symbol
{
    char *name;
    struct Value value;
};
struct Symbol symbol_table[MAX_SYMBOLS];
int symbol_count = 0;

void store_variable(char *name, struct Value value)
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            symbol_table[i].value = value;
            return;
        }
    }
    if (symbol_count < MAX_SYMBOLS)
    {
        symbol_table[symbol_count].name = name;
        symbol_table[symbol_count].value = value;
        symbol_count++;
    }
}

struct Value lookup_variable(char *name)
{
    for (int i = 0; i < symbol_count; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            return symbol_table[i].value;
        }
    }
    fprintf(stderr, "Error: Undefined variable %s\n", name);
    exit(1);
}

struct ASTNode *create_node_list(struct ASTNode *stmt, struct ASTNode *next)
{
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    node->type = NODE_TYPE_LIST;
    node->data.list.left = stmt;
    node->data.list.right = next;
    return node;
}
struct ASTNode *create_node_val(struct Value val)
{
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    node->type = NODE_TYPE_VAL;
    node->data.val = val;
    return node;
}
struct ASTNode *create_node_id(char *name)
{
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    node->type = NODE_TYPE_ID;
    node->data.name = name;
    return node;
}
struct ASTNode *create_node_op(int op, struct ASTNode *left, struct ASTNode *right)
{
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    node->type = NODE_TYPE_OP;
    node->data.op.op = op;
    node->data.op.left = left;
    node->data.op.right = right;
    return node;
}
struct ASTNode *create_node_assign(char *name, struct ASTNode *expr)
{
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    node->type = NODE_TYPE_ASSIGN;
    node->data.assign.name = name;
    node->data.assign.expr = expr;
    return node;
}
struct ASTNode *create_node_spill(struct ASTNode *expr)
{
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    node->type = NODE_TYPE_SPILL;
    node->data.spill.expr = expr;
    return node;
}
struct ASTNode *create_node_if(struct ASTNode *cond, struct ASTNode *if_branch, struct ASTNode *else_branch)
{
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    node->type = NODE_TYPE_IF;
    node->data.branch.cond = cond;
    node->data.branch.if_branch = if_branch;
    node->data.branch.else_branch = else_branch;
    return node;
}
struct ASTNode *create_node_while(struct ASTNode *cond, struct ASTNode *loop_body)
{
    struct ASTNode *node = malloc(sizeof(struct ASTNode));
    node->type = NODE_TYPE_WHILE;
    node->data.branch.cond = cond;
    node->data.branch.if_branch = loop_body;
    node->data.branch.else_branch = NULL;
    return node;
}

struct Value execute_ast(struct ASTNode *node)
{
    struct Value result = {.type = VAL_TYPE_INT, .ival = 0};
    if (!node)
        return result;

    switch (node->type)
    {
    case NODE_TYPE_LIST:
        execute_ast(node->data.list.left);
        execute_ast(node->data.list.right);
        break;
    case NODE_TYPE_VAL:
        return node->data.val;
    case NODE_TYPE_ID:
        return lookup_variable(node->data.name);
    case NODE_TYPE_ASSIGN:
        store_variable(node->data.assign.name, execute_ast(node->data.assign.expr));
        break;
    case NODE_TYPE_SPILL:
    {
        struct Value spill_val = execute_ast(node->data.spill.expr);
        if (spill_val.type == VAL_TYPE_INT)
            printf("%d\n", spill_val.ival);
        else if (spill_val.type == VAL_TYPE_STR)
            printf("%s\n", spill_val.sval);
        break;
    }
    case NODE_TYPE_IF:
        if (execute_ast(node->data.branch.cond).ival)
            execute_ast(node->data.branch.if_branch);
        else if (node->data.branch.else_branch)
            execute_ast(node->data.branch.else_branch);
        break;
    case NODE_TYPE_WHILE:
        while (execute_ast(node->data.branch.cond).ival)
            execute_ast(node->data.branch.if_branch);
        break;
    case NODE_TYPE_OP:
    {
        int left = execute_ast(node->data.op.left).ival;
        int right = execute_ast(node->data.op.right).ival;
        result.type = VAL_TYPE_INT;
        switch (node->data.op.op)
        {
        case '+':
            result.ival = left + right;
            break;
        case '-':
            result.ival = left - right;
            break;
        case '*':
            result.ival = left * right;
            break;
        case '/':
            if (right == 0)
            {
                fprintf(stderr, "Runtime Error: Division by zero.\n");
                exit(1);
            }
            result.ival = left / right;
            break;
        case T_EQ:
            result.ival = left == right;
            break;
        case T_NEQ:
            result.ival = left != right;
            break;
        case T_GT:
            result.ival = left > right;
            break;
        case T_LT:
            result.ival = left < right;
            break;
        case T_GTE:
            result.ival = left >= right;
            break;
        case T_LTE:
            result.ival = left <= right;
            break;
        }
        return result;
    }
    }
    return result;
}

/* --- MAIN DRIVER (NOW SIMPLIFIED) --- */
int yyparse(void);
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename.genz>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        fprintf(stderr, "Could not open file '%s'\n", argv[1]);
        return 1;
    }
    yyin = file;
    printf("--- GenZ-Lang v6.0 (Final) ---\n");

    yyparse(); // This now does everything: parses and executes.

    fclose(file);
    return 0;
}