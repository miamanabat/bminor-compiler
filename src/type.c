#include "type.h"
#include "expr.h"
#include <stdlib.h>
#include <stdio.h>

struct type * type_create( type_t kind, struct type *subtype, struct expr *arr_expr, struct param_list *params ) {
    struct type* t = malloc(sizeof(struct type));
    t->kind = kind;
    t->subtype = subtype;
    t->arr_expr = arr_expr;
    t->params = params;
    return t;
}

void type_print( struct type *t ) {
    if (!t) return;

    switch (t->kind) {
        case TYPE_VOID:
            printf("void");
            break;
        case TYPE_BOOLEAN:
            printf("boolean");
            break;
        case TYPE_CHARACTER:
            printf("char");
            break;
        case TYPE_INTEGER:
            printf("integer");
            break;
        case TYPE_STRING:
            printf("string");
            break;
        case TYPE_ARRAY:
            printf("array[");
            expr_print(t->arr_expr);
            printf("] ");
            type_print(t->subtype);
            break;
        case TYPE_FUNCTION:
            printf("function ");
            type_print(t->subtype);
            printf("(");
            param_list_print(t->params);
            printf(")");
            break;
        case TYPE_AUTO:
            printf("auto");
            break;
    }
}

struct type * type_copy( struct type *t ) {
    if (!t) return NULL;
    // return a duplicate copy of t, making sure to duplicate subtype and params recursively
    struct type * copy = type_create(t->kind, type_copy(t->subtype), expr_copy(t->arr_expr), param_list_copy(t->params));
    copy->num_elements = t->num_elements;
    return copy;
}
int type_compare( struct type **a1, struct type **b1, int change ) {

    if (!(*a1) && !(*b1)) return 0;
    if (!(*a1) || !(*b1)) return 1;
    
    if (change && (*a1)->kind == TYPE_AUTO) {
        *a1 = type_copy((*b1));
        printf("notice: type of auto is now ");
        type_print((*b1));
        printf("\n");   
    }
    if (change && (*a1)->subtype && (*b1)->subtype && (*a1)->subtype->kind == TYPE_AUTO) {
        (*a1)->subtype = type_copy((*b1)->subtype);
        printf("notice: type of auto is now ");
        type_print((*b1)->subtype);
        printf("\n");   
    }

    if ((*a1)->kind == TYPE_ARRAY && (*b1)->kind == TYPE_ARRAY) {
        if ((*a1)->arr_expr->literal_value != (*b1)->num_elements) return 1;
        return type_compare(&((*a1)->subtype), &((*b1)->subtype), change);
    } else if ((*a1)->kind == TYPE_FUNCTION && (*b1)->kind == TYPE_FUNCTION) {
        return type_compare(&((*a1)->subtype), &((*b1)->subtype), change) && param_list_compare((*a1)->params, (*b1)->params, 0);
    } else if ((*a1)->kind == (*b1)->kind){
        return 0;
    } else {
        return 1;
    }
}

