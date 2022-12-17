
#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include "scope.h"
#include "symbol.h"
#include "scratch.h"
#include <stdio.h>

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
    int func_def;
    int return_exists;
    int param_count;
    int local_count;
};

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
struct decl * decl_func_def_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next);
void decl_print( struct decl *d, int indent );
void indent_print( int indent );
void decl_resolve( struct scope *s, struct decl *d );
void decl_typecheck( struct decl *d );

void decl_codegen( struct decl *d );
void global_label( struct decl *d );
void func_prologue( struct decl *d );
void func_epilogue( struct decl *d );
void decl_codegen_arr( struct decl *d, struct expr *e );
#endif


