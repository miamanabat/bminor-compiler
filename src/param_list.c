#include "param_list.h"
#include <stdio.h>
#include <stdlib.h>

extern int typecheck_error;
extern int resolve_error;

struct param_list * param_list_create( char *name, struct type *type, struct param_list *next ) {
    struct param_list* p = malloc(sizeof(struct param_list));
    p->name = name;
    p->type = type;
    p->next = next;
    return p;
}
void param_list_print( struct param_list *a ) {
    if (!a) return;

    printf("%s", a->name);
    printf(":");
    type_print(a->type);

    if (a->next) {
        printf(",");
        param_list_print(a->next);
    }
}

void param_list_resolve(struct scope *s, struct param_list *p, struct decl *d) {
    if (!p) return;
    symbol_t kind = SYMBOL_PARAM;
    p->symbol = symbol_create(kind, p->type, p->name);
    if (scope_bind(s, p->name, p->symbol) != 0) {
        printf("%s is already defined\n", p->name);
        resolve_error = 1;
    }
    p->symbol->which = inc_var_counter(s);
    d->param_count += 1;
    param_list_resolve(s, p->next, d);
    
}

struct param_list *param_list_copy( struct param_list *param_list ) {
    if (!param_list) return NULL;
    return param_list_create(param_list->name, type_copy(param_list->type), param_list_copy(param_list->next));
}

struct param_list *param_list_from_exprlist( struct expr *e) {
    if (!e) return NULL;
    if (e->kind != EXPR_LIST) {
        return param_list_create(0, expr_typecheck(e), 0);
    }
    if (e->right && e->right->kind != EXPR_LIST) {
        return param_list_create(0,expr_typecheck(e->left), param_list_create(0, expr_typecheck(e->right), 0));
    }
    return param_list_create(0,expr_typecheck(e->left), param_list_from_exprlist(e->right));
}

int param_list_compare( struct param_list *a, struct param_list *b, int resolve) {
    if (!a && !b) return 0; // 0 = same
    if (!a || !b) return 1;
    if (type_compare(&(a->type), &(b->type), 0)) {
        if (resolve) return 1;
        printf("type error: param_list type ");
        type_print(b->type);
        printf(" does not match type ");
        type_print(a->type);
        printf("\n");
        typecheck_error = 1; 
        return 1;
    }

    // return 0 if the same and 1 if not the same
    return param_list_compare(a->next, b->next, resolve);
}

int param_list_auto( struct param_list *a ) {
    if (!a) return 0;
    if (a->type->kind == TYPE_AUTO) {
        printf("type error: param (%s) cannot be type auto\n", a->name);
        typecheck_error = 1;
        return 1;
    }
    return param_list_auto(a->next);
}
