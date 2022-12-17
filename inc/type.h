#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"
#include "expr.h"

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
    TYPE_AUTO,
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
    struct expr *arr_expr;
	struct type *subtype;
    int num_elements; // number of array elements
};

struct type * type_create( type_t kind, struct type *subtype, struct expr *arr_expr, struct param_list *params );
void          type_print( struct type *t );

struct type * type_copy( struct type *t );
int type_compare( struct type **a1, struct type **b1, int change );


#endif
