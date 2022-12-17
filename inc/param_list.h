
#ifndef PARAM_LIST_H
#define PARAM_LIST_H

#include "type.h"
#include "scope.h"
#include "decl.h"
#include "expr.h"
#include <stdio.h>

struct expr;
struct scope;
struct decl;

struct param_list {
	char *name;
	struct type *type;
	struct symbol *symbol;
	struct param_list *next;
};

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next );
void param_list_print( struct param_list *a );
void param_list_resolve( struct scope *s, struct param_list *p, struct decl *d );
struct param_list * param_list_copy( struct param_list *param_list );
struct param_list *param_list_from_exprlist( struct expr *e );
int param_list_compare( struct param_list *a, struct param_list *b, int resolve );
int param_list_auto( struct param_list *a );
#endif
