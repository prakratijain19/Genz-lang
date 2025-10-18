/* genz.y - FINAL ERROR-FREE VERSION */
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);

// We no longer need ast_root here, execution is done inside the parser
%}

%code requires {
    #include "ast.h"
}

%union {
    int ival;
    char *sval;
    struct ASTNode *node;
    struct Value val;
}

%token <ival> T_NUMBER
%token <sval> T_IDENTIFIER T_STRING
%token T_DECLARE T_ASSIGN T_SPILL T_TILDE T_EXIT
%token T_VIBECHECK T_IF T_THEN T_NOCAP T_ELSE T_CAP
%token T_ON_REPEAT T_WHILE T_FINNA_STOP
%token T_EQ T_NEQ T_GT T_LT T_GTE T_LTE

%type <node> program statement_list statement expr condition if_statement loop_statement
%type <val> value

%left '+' '-'
%left '*' '/'

%%

/* This rule now calls execute_ast on the completed tree */
program: statement_list { execute_ast($1); YYACCEPT; };

statement_list: { $$ = NULL; } | statement_list statement { $$ = create_node_list($1, $2); };

statement:
    T_DECLARE T_IDENTIFIER T_ASSIGN expr T_TILDE { $$ = create_node_assign($2, $4); }
    | T_SPILL expr T_TILDE                       { $$ = create_node_spill($2); }
    | if_statement
    | loop_statement
    | T_EXIT T_TILDE                              { YYACCEPT; }
;

if_statement:
    T_VIBECHECK T_IF '(' condition ')' T_THEN statement_list T_NOCAP { $$ = create_node_if($4, $7, NULL); }
    | T_VIBECHECK T_IF '(' condition ')' T_THEN statement_list T_ELSE statement_list T_CAP { $$ = create_node_if($4, $7, $9); }
;

loop_statement: T_ON_REPEAT T_WHILE '(' condition ')' statement_list T_FINNA_STOP { $$ = create_node_while($4, $6); };

condition:
    expr T_EQ expr   { $$ = create_node_op(T_EQ, $1, $3); }
    | expr T_NEQ expr  { $$ = create_node_op(T_NEQ, $1, $3); }
    | expr T_GT expr   { $$ = create_node_op(T_GT, $1, $3); }
    | expr T_LT expr   { $$ = create_node_op(T_LT, $1, $3); }
    | expr T_GTE expr  { $$ = create_node_op(T_GTE, $1, $3); }
    | expr T_LTE expr  { $$ = create_node_op(T_LTE, $1, $3); }
;

value: T_NUMBER { $$.type = VAL_TYPE_INT; $$.ival = $1; }
     | T_STRING { $$.type = VAL_TYPE_STR; $$.sval = $1; }
     ;

expr:
    value                 { $$ = create_node_val($1); }
    | T_IDENTIFIER          { $$ = create_node_id($1); }
    | expr '+' expr         { $$ = create_node_op('+', $1, $3); }
    | expr '-' expr         { $$ = create_node_op('-', $1, $3); }
    | expr '*' expr         { $$ = create_node_op('*', $1, $3); }
    | expr '/' expr         { $$ = create_node_op('/', $1, $3); }
    | '(' expr ')'          { $$ = $2; }
;

%%
void yyerror(const char *s) { fprintf(stderr, "Syntax Error\n"); }