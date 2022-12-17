#include "decl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern int typecheck_error;
extern int resolve_error;
extern int codegen_error;
extern FILE *fp;

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next ) {
    struct decl* d = malloc(sizeof(struct decl));
    d->name = name;
    d->type = type;
    d->value = value;
    d->code = code;
    d->symbol = NULL;
    d->next = next;
    d->func_def = 0;
    d->param_count = 0;
    d->local_count = 0;
    return d;
}

struct decl * decl_func_def_create(char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next) {
    struct decl *d = decl_create(name, type, value, code, next);
    d->func_def = 1;
    return d;
}
void decl_print( struct decl *d, int indent ) {
    if (!d) return;

    indent_print(indent);
    printf("%s: ", d->name);
    type_print(d->type);
    if (d->value) {
        printf(" = ");
        expr_print(d->value);
        printf(";\n");
    } else if (d->type->kind == TYPE_FUNCTION) {
        if (d->func_def == 0){
            printf(";\n");
        } else {
            printf(" = \n");
            indent_print(indent);
            printf("{\n");
            stmt_print(d->code, indent + 1);
            indent_print(indent);
            printf("}\n");  
        }
    } else {
        printf(";\n");
    }
    decl_print(d->next, indent);
}

void indent_print(int indent) {
    if (indent < 0) return;
    for (int i = 0; i < indent * 4; i++) {
        printf(" ");
    }
}

// add return ints and stuff
void decl_resolve(struct scope *s, struct decl *d) {
    if(!d) return;
    printf("resolving\n");
    symbol_t kind = scope_level(s) > 1 ?
        SYMBOL_LOCAL : SYMBOL_GLOBAL;

    d->symbol = symbol_create(kind,d->type,d->name); 
    d->symbol->which = inc_var_counter(s);
    
    expr_resolve(s, d->value);

    // if already in the symbol table
    if (scope_bind(s, d->name,d->symbol) != 0) {
        // if function, compare parameters- must be consistent
        if (d->type->kind == TYPE_FUNCTION) {
            // check return type
            struct symbol *symbol = scope_lookup(s, d->name);
            if (type_compare(&(d->type->subtype), &(symbol->type->subtype), 0)) {
                printf("%s is already defined with a different return type of ", d->name);
                type_print(symbol->type->subtype);
                printf("\n");
                resolve_error = 1;
            }
            // check param list
            if (param_list_compare(d->type->params, symbol->type->params, 1)) {
                printf("%s is already defined, conflicting param types\n", d->name);
                resolve_error = 1;    
            } 
            // check if function is being redefined --> error
            if (symbol->defined && d->code) {
                printf("%s is already defined\n", d->name);
                resolve_error = 1;
            }

        } else {
            printf("%s is already defined\n", d->name); 
            resolve_error = 1;
        }
    } 
    // function: enter scope, resolve things
    if(d->code) {
        d->symbol->defined = 1;
        d->code->in_func = 1;
        scope_enter(s);
        param_list_resolve(s->next, d->type->params, d);
        d->param_count = s->next->var_count;
        stmt_resolve(s->next, d->code);
        d->local_count = s->next->var_count - d->param_count;
        scope_exit(s->next);
    }
    decl_resolve(s, d->next);
}

void decl_typecheck( struct decl *d ) {
    if (!d) return;

    if (d->symbol->type->kind == TYPE_ARRAY) {
        if (d->symbol->type->subtype->kind == TYPE_FUNCTION || d->symbol->type->subtype->kind == TYPE_VOID) {
            printf("type error: cannot declare an array of type ");
            type_print(d->symbol->type->subtype);
            printf("\n");
            typecheck_error = 1;
        }
        // make sure that the array size is a literal int value or null
        if (d->symbol->type->arr_expr) {
           if (d->symbol->type->arr_expr->kind != EXPR_NUMBER) {
                printf("type error: cannot declare an array with a non-int literal value");
                expr_type_print(d->symbol->type->arr_expr, expr_typecheck(d->symbol->type->arr_expr));
                printf("\n");
                typecheck_error = 1;
            }
            d->symbol->type->num_elements = d->symbol->type->arr_expr->literal_value;

        } else {
            d->symbol->type->num_elements = 0;
        }
        if (d->symbol->kind == SYMBOL_LOCAL) {
            printf("type error: cannot declare array (%s) in local scope\n", d->symbol->name);
            typecheck_error = 1;
        }
    } else if (d->symbol->type->kind == TYPE_FUNCTION) {
        // if function, check param_list for any autos
        param_list_auto(d->symbol->type->params);
    }

    if (d->value) {
        struct type *t = expr_typecheck(d->value);
        // check that globals are literal values only
        if (d->symbol->kind == SYMBOL_GLOBAL) {
            switch (d->value->kind) {
                case EXPR_NUMBER:
                case EXPR_STRING:
                case EXPR_CHAR:
                case EXPR_BOOL:
                case EXPR_ARR_INIT:
                    break;
                default:
                    printf("type error: cannot declare non-literal value");
                    expr_type_print(d->value, t);
                    printf(" in global scope\n");
                    typecheck_error = 1;
                    break;
            }
            if (d->symbol->type->kind == TYPE_ARRAY) {
                if (!d->symbol->type->arr_expr) {
                    printf("type error: array (%s) must be declared with integer length outside of params\n", d->symbol->name);
                    typecheck_error = 1;
                }
            }
        } 
        // check types of d and d value
        if (type_compare(&(d->symbol->type), &t, 1)) {
            printf("type error: cannot declare a type ");
            type_print(d->symbol->type);
            printf(" to be");
            expr_type_print(d->value, t);
            printf("\n");
            typecheck_error = 1;
        }
        d->symbol->defined = 1; 
    }
    if (d->code) {
        // check return type
        struct type *return_type = d->type->subtype;
        if (return_type->kind == TYPE_FUNCTION || return_type->kind == TYPE_ARRAY) {
            printf("type error: return type of function canot be ");
            type_print(return_type);
            printf("\n");  
            typecheck_error = 1;
        }
        stmt_typecheck(d->code, d);
        if (return_type->kind != TYPE_VOID && d->return_exists == 0 && strcmp(d->name, "main") != 0) {
            printf("type error: no return statement for a non-void function\n");
            typecheck_error = 1;
        }
    d->symbol->defined = 1;
    }
    decl_typecheck(d->next);
    
}


void decl_codegen( struct decl *d ) {
    if (!d) return;
    switch (d->symbol->kind) {
        case SYMBOL_GLOBAL:
            switch (d->symbol->type->kind) {
                case TYPE_STRING:
                    fprintf(fp, ".data\n");
                    fprintf(fp, ".global %s\n", symbol_codegen(d->symbol));
                    fprintf(fp, "%s:\n", symbol_codegen(d->symbol));
                    if (!d->value) fprintf(fp, ".quad 0\n");
                    else {
                        int label = label_create();
                        fprintf(fp, ".quad %s\n",
                                label_name(label));

                        fprintf(fp, ".data\n");
                        fprintf(fp, ".global %s\n", label_name(label));
                        fprintf(fp, "%s:\n", label_name(label));

                        fprintf(fp, ".string %s\n", d->value->string_literal); 
                    }
                    break;
                case TYPE_BOOLEAN:
                    global_label(d);
                    fprintf(fp, ".quad %d\n", (d->value ? d->value->literal_value : 0));
                    break;
                case TYPE_CHARACTER:
                    global_label(d);
                    {
                        if (d->value) {
                            char c = d->value->char_literal[1];
                            fprintf(fp, ".quad %d\n", c);
                        } else {
                            fprintf(fp, ".quad 0\n");
                        }
                    }
                    break;
                case TYPE_INTEGER:
                    global_label(d);
                    fprintf(fp, ".quad %d\n", (d->value ? d->value->literal_value : 0));
                    break;
                case TYPE_ARRAY:
                    global_label(d);
                    decl_codegen_arr(d, d->value);
                    break;
                case TYPE_FUNCTION:
                    if (d->code) {
                        global_label(d);
                        func_prologue(d);
                        stmt_codegen(d->code);
                        func_epilogue(d);
                    }
                    break;
                case TYPE_AUTO: 
                    break;
            }
            break;
        case SYMBOL_PARAM:
        case SYMBOL_LOCAL:
            switch (d->symbol->type->kind) {
                case TYPE_STRING:
                case TYPE_BOOLEAN:
                case TYPE_CHARACTER:
                case TYPE_INTEGER:
                    if (d->value) {
                        expr_codegen(d->value);
                        fprintf(fp, "\tmovq %s, %s\n", 
                                scratch_name(d->value->reg), 
                                symbol_codegen(d->symbol));
                        scratch_free(d->value->reg);
                    } 
                    break;
                case TYPE_ARRAY:
                    printf("error: local array declaration not allowed\n");
                    break;

                case TYPE_FUNCTION:
                    printf("error: local function declaration not allowed\n");
                    break;
                case TYPE_AUTO:
                    break;
            } 
            break;
    }
   decl_codegen(d->next); 
}

void global_label( struct decl *d ) {
    if (!d) return;
    if (d->symbol->type->kind == TYPE_FUNCTION) {
        fprintf(fp, ".text\n");
        fprintf(fp, ".global %s\n", d->name);
        fprintf(fp, "%s:\n", d->name);
    } else {
        fprintf(fp, ".data\n");
        fprintf(fp, ".global %s\n", d->name);
        fprintf(fp, "%s:\n", d->name);
    }
}

void func_prologue(struct decl *d) {
    if (!d) return;
    fprintf(fp, "\tpushq %%rbp\n");
    fprintf(fp, "\tmovq %%rsp, %%rbp\n\n");

    // save the arguments
    for (int i = 0; i < d->param_count; i++) {
        fprintf(fp, "\tpushq %s\n", arg_name(i));
    }

    // allocate space for local variables
    fprintf(fp, "\tsubq $%d, %%rsp\n", d->local_count * 8 );

    // save callee saved registers
    fprintf(fp, "\tpushq %%rbx\n");
    fprintf(fp, "\tpushq %%r12\n");
    fprintf(fp, "\tpushq %%r13\n");
    fprintf(fp, "\tpushq %%r14\n");
    fprintf(fp, "\tpushq %%r15\n");
}

void func_epilogue( struct decl *d ) {
    fprintf(fp, ".%s_epilogue:\n", d->name);

    // restore callee-saved registers
    fprintf(fp, "\tpopq %%r15\n");
    fprintf(fp, "\tpopq %%r14\n");
    fprintf(fp, "\tpopq %%r13\n");
    fprintf(fp, "\tpopq %%r12\n");
    fprintf(fp, "\tpopq %%rbx\n");
    
    // reset stack to base pointer
    fprintf(fp, "\tmovq %%rbp, %%rsp\n");

    // restore the old base pointer
    fprintf(fp, "\tpopq %%rbp\n");

    // return to caller
    fprintf(fp, "\tret\n");
}

void decl_codegen_arr( struct decl *d, struct expr *e ) {
    if (!d->value) {
        for (int i = 0; i < d->symbol->type->arr_expr->literal_value; i++) {
            fprintf(fp, ".quad 0\n");
        }
    }
    if (!e) return;

    struct expr * curr = e->left;
    while (curr) {
        if (curr->kind == EXPR_LIST) {
            fprintf(fp, ".quad %d\n", curr->left->literal_value);
        } else {
            fprintf(fp, ".quad %d\n", curr->literal_value);
        }
        curr = curr->right;
    }
}
