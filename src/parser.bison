%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "decl.h"
#include "param_list.h"
extern char* yytext;
extern int yylex();
extern int yyerror(char *str);
struct decl* d;
%}
%token TOKEN_MULTI_COMMENT
%token TOKEN_SINGLE_COMMENT
%token TOKEN_STRING
%token TOKEN_ERROR_CHAR
%token TOKEN_CHAR
%token TOKEN_ARRAY
%token TOKEN_AUTO
%token TOKEN_BOOLEAN
%token TOKEN_KEYWORD_CHAR
%token TOKEN_ELSE
%token TOKEN_FOR
%token TOKEN_FUNCTION
%token TOKEN_IF
%token TOKEN_INTEGER
%token TOKEN_PRINT
%token TOKEN_RETURN
%token TOKEN_KEYWORD_STRING
%token TOKEN_VOID
%token TOKEN_WHILE
%token TOKEN_TRUE
%token TOKEN_FALSE
%token TOKEN_AND
%token TOKEN_OR
%token TOKEN_TERNARY
%token TOKEN_COLON
%token TOKEN_SEMICOLON
%token TOKEN_COMMA
%token TOKEN_LBRACKET
%token TOKEN_RBRACKET
%token TOKEN_LBRACE
%token TOKEN_RBRACE
%token TOKEN_LPAREN
%token TOKEN_RPAREN
%token TOKEN_ASSIGNMENT
%token TOKEN_EXPONENT
%token TOKEN_INCREMENT
%token TOKEN_DECREMENT
%token TOKEN_ADD
%token TOKEN_HYPHEN
%token TOKEN_ASTERISK
%token TOKEN_FORWARD_SLASH
%token TOKEN_PERCENT
%token TOKEN_EQ
%token TOKEN_LE
%token TOKEN_GE
%token TOKEN_LT
%token TOKEN_GT
%token TOKEN_NE
%token TOKEN_NOT
%token TOKEN_IDENT
%token TOKEN_NUMBER
%token TOKEN_ERROR_INVALID

%union {
    struct decl *decl;
    struct stmt *stmt;
    struct expr *expr;
    struct type *type;
    struct param_list *param_list;
    char * name;
}

%type <decl> program decls decl
%type <stmt> open_stmt closed_stmt simple_stmt stmts_or_nothing stmts stmt
%type <expr> expr expr1 expr2 expr3 expr4 expr5 expr6 expr7 expr8 expr9 expr_list expr_or_nothing expr_list_not_empty atomic array_init array_index cond
%type <type> type
%type <param_list> param_list param_list_not_empty param
%type <name> ident
%%
program : decls                                         { $$ = $1; d = $$; }
        ;  

expr    : expr1 TOKEN_ASSIGNMENT expr                   { $$ = expr_create(EXPR_ASSIGNMENT, $1, $3); }
        | expr1 TOKEN_TERNARY expr TOKEN_COLON expr     { $$ = expr_create_ternary($1, $3, $5); }
        | expr1                                         { $$ = $1; }
        ;

expr1   : expr1 TOKEN_OR expr2                          { $$ = expr_create(EXPR_OR, $1, $3); }
        | expr2                                         { $$ = $1; }
        ;

expr2   : expr2 TOKEN_AND expr3                         { $$ = expr_create(EXPR_AND, $1, $3); }
        | expr3                                         { $$ = $1; }

expr3   : expr3 TOKEN_EQ expr4                          { $$ = expr_create(EXPR_EQ, $1, $3); }
        | expr3 TOKEN_LE expr4                          { $$ = expr_create(EXPR_LE, $1, $3); }
        | expr3 TOKEN_GE expr4                          { $$ = expr_create(EXPR_GE, $1, $3); }
        | expr3 TOKEN_LT expr4                          { $$ = expr_create(EXPR_LT, $1, $3); }
        | expr3 TOKEN_GT expr4                          { $$ = expr_create(EXPR_GT, $1, $3); }
        | expr3 TOKEN_NE expr4                          { $$ = expr_create(EXPR_NE, $1, $3); }

        | expr4                                         { $$ = $1; }
        ;

expr4   : expr4 TOKEN_ADD expr5                         { $$ = expr_create(EXPR_ADD, $1, $3); }
        | expr4 TOKEN_HYPHEN expr5                      { $$ = expr_create(EXPR_SUB, $1, $3); }
        | expr5                                         { $$ = $1; }
        ;

expr5   : expr5 TOKEN_ASTERISK expr6                    { $$ = expr_create(EXPR_MUL, $1, $3); }
        | expr5 TOKEN_FORWARD_SLASH expr6               { $$ = expr_create(EXPR_DIV, $1, $3); }
        | expr5 TOKEN_PERCENT expr6                     { $$ = expr_create(EXPR_MOD, $1, $3); }
        | expr6                                         { $$ = $1; }
        ;

expr6   : expr7 TOKEN_EXPONENT expr6                    { $$ = expr_create(EXPR_EXP, $1, $3); }
        | expr7                                         { $$ = $1; } 
        ;

expr7   : TOKEN_NOT expr7                               { $$ = expr_create(EXPR_NOT, $2, NULL); }
        | TOKEN_HYPHEN expr7                            { $$ = expr_create(EXPR_NEGATE, $2, NULL); }
        | expr8                                         { $$ = $1; }
        ;

expr8   : expr8 TOKEN_DECREMENT                         { $$ = expr_create(EXPR_DECREMENT, $1, NULL); }
        | expr8 TOKEN_INCREMENT                         { $$ = expr_create(EXPR_INCREMENT, $1, NULL); }
        | expr9                                         { $$ = $1; }
        ;

expr9   : TOKEN_LPAREN expr TOKEN_RPAREN                { $$ = $2; }
        | ident TOKEN_LPAREN expr_list TOKEN_RPAREN  /* function call */ { $$ = expr_create_func_call($1, $3); }
        | array_index                                                   { $$ = $1; }
        | atomic                                                        { $$ = $1; } 
        ;

expr_list: expr_list_not_empty                                          { $$ = $1; }
        |                                                               { $$ = NULL; }
        ;

expr_or_nothing: expr                                                   { $$ = $1; }
        |                                                               { $$ = NULL; }
        ; 

expr_list_not_empty: expr TOKEN_COMMA expr_list_not_empty               { $$ = expr_create(EXPR_LIST, $1, $3); }
        | expr                                                          { $$ = $1; }
        ;

atomic  : TOKEN_NUMBER          { $$ = expr_create_integer_literal(atoi(yytext)); }
        | TOKEN_STRING          { $$ = expr_create_string_literal(strdup(yytext)); }
        | TOKEN_CHAR            { $$ = expr_create_char_literal(strdup(yytext)); }
        | TOKEN_TRUE            { $$ = expr_create_boolean_literal(1); }
        | TOKEN_FALSE           { $$ = expr_create_boolean_literal(0); }
        | ident                 { $$ = expr_create_name($1); }
        | array_init            { $$ = $1; }
        ;

array_init: TOKEN_LBRACE expr_list_not_empty TOKEN_RBRACE   { $$ = expr_create(EXPR_ARR_INIT, $2, NULL); }
        ;

array_index: ident TOKEN_LBRACKET expr TOKEN_RBRACKET       { $$ = expr_create(EXPR_ARR_INDEX, expr_create_name($1), $3); }
        | array_index TOKEN_LBRACKET expr TOKEN_RBRACKET    { $$ = expr_create(EXPR_ARR_INDEX, $1, $3); }
        ;

stmts_or_nothing: stmts     { $$ = $1; }
        |                   { $$ = NULL; }
        ;

stmts   : stmt stmts        { $1->next = $2; $$ = $1; }
        | stmt              { $$ = $1; }
        ;

stmt    : open_stmt         { $$ = $1; }
        | closed_stmt       { $$ = $1; }
        ;

cond    : TOKEN_LPAREN expr TOKEN_RPAREN                        { $$ = $2; }
        ;

open_stmt: TOKEN_IF cond closed_stmt TOKEN_ELSE open_stmt       { $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $2, NULL, $3, $5, NULL); }
        | TOKEN_IF cond closed_stmt                             { $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $2, NULL, $3, NULL, NULL); }
        | TOKEN_FOR TOKEN_LPAREN expr_or_nothing TOKEN_SEMICOLON expr_or_nothing TOKEN_SEMICOLON expr_or_nothing TOKEN_RPAREN open_stmt     { $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9, NULL, NULL); }
        ;

closed_stmt: TOKEN_IF cond closed_stmt TOKEN_ELSE closed_stmt   { $$ = stmt_create(STMT_IF_ELSE, NULL, NULL, $2, NULL, $3, $5, NULL); }
        | TOKEN_FOR TOKEN_LPAREN expr_or_nothing TOKEN_SEMICOLON expr_or_nothing TOKEN_SEMICOLON expr_or_nothing TOKEN_RPAREN closed_stmt   { $$ = stmt_create(STMT_FOR, NULL, $3, $5, $7, $9, NULL, NULL); }
        | simple_stmt                                           { $$ = $1; }
        ;

simple_stmt: TOKEN_RETURN expr_or_nothing TOKEN_SEMICOLON       { $$ = stmt_create(STMT_RETURN, NULL, NULL, $2, NULL, NULL, NULL, NULL); }             
        | TOKEN_PRINT expr_list TOKEN_SEMICOLON                 { $$ = stmt_create(STMT_PRINT, NULL, NULL, $2, NULL, NULL, NULL, NULL); }
        | expr TOKEN_SEMICOLON                                  { $$ = stmt_create(STMT_EXPR, NULL, NULL, $1, NULL, NULL, NULL, NULL); }
        | decl                                                  { $$ = stmt_create(STMT_DECL, $1, NULL, NULL, NULL, NULL, NULL, NULL); }
        | TOKEN_LBRACE stmts_or_nothing TOKEN_RBRACE            { $$ = stmt_create(STMT_BLOCK, NULL, NULL, NULL, NULL, $2, NULL, NULL); }
        ; 

ident   : TOKEN_IDENT                                           { $$ = strdup(yytext); }
        ;

decl    : ident TOKEN_COLON type TOKEN_ASSIGNMENT expr TOKEN_SEMICOLON                          { $$ = decl_create($1, $3, $5, NULL, NULL); }
        | ident TOKEN_COLON type TOKEN_ASSIGNMENT TOKEN_LBRACE stmts_or_nothing TOKEN_RBRACE    { $$ = decl_func_def_create($1, $3, NULL, $6, NULL); }
        | ident TOKEN_COLON type TOKEN_SEMICOLON                                                { $$ = decl_create($1, $3, NULL, NULL, NULL); }
        ;

decls   : decl decls                                            { $1->next = $2; $$ = $1; }
        |                                                       { $$ = NULL; }
        ;

type    : TOKEN_KEYWORD_CHAR                                    { $$ = type_create(TYPE_CHARACTER, NULL, NULL, NULL); }
        | TOKEN_KEYWORD_STRING                                  { $$ = type_create(TYPE_STRING, NULL, NULL, NULL); }
        | TOKEN_INTEGER                                         { $$ = type_create(TYPE_INTEGER, NULL, NULL, NULL); }
        | TOKEN_BOOLEAN                                         { $$ = type_create(TYPE_BOOLEAN, NULL, NULL, NULL); }
        | TOKEN_AUTO                                            { $$ = type_create(TYPE_AUTO, NULL, NULL, NULL); }
        | TOKEN_VOID                                            { $$ = type_create(TYPE_VOID, NULL, NULL, NULL); }
        | TOKEN_ARRAY TOKEN_LBRACKET expr_or_nothing TOKEN_RBRACKET type        { $$ = type_create(TYPE_ARRAY, $5, $3, NULL); } 
        | TOKEN_FUNCTION type TOKEN_LPAREN param_list TOKEN_RPAREN              { $$ = type_create(TYPE_FUNCTION, $2, NULL, $4); }
        ;

param_list: param_list_not_empty                                { $$ = $1; }
        |                                                       { $$ = NULL; }
        ;

param_list_not_empty: param TOKEN_COMMA param_list_not_empty    { $1->next = $3; $$ = $1; }
        | param                                                 { $$ = $1; }
        ;

param   : ident TOKEN_COLON type                                { $$ = param_list_create($1, $3, NULL); }
        ;

%%
int yyerror(char* str) {
    printf("parse error: %s\n", str);
    return 0;
}
