
#ifndef STMT_H
#define STMT_H

#include "decl.h"
#include "scope.h"
#include "scratch.h"

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
    int else_if;
    char *name;
    int in_func;    // true if stmt block is in function
};

struct scope;

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr * next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next );
void stmt_print( struct stmt *s, int indent );
void stmt_resolve( struct scope *s, struct stmt *stmt );
void stmt_typecheck( struct stmt *s, struct decl *decl );

void stmt_codegen( struct stmt *s );
void stmt_codegen_print( struct expr *e );
#endif
