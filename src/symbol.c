#include "symbol.h"
#include "string.h"
#include <stdlib.h>

struct symbol * symbol_create( symbol_t kind, struct type *type, char *name ) {
    struct symbol *s = malloc(sizeof(struct symbol));
    s->kind = kind;
    s->type = type;
    s->name = name;
    s->which = 0;
    s->defined = 0;
    return s;
}

void print_resolve( struct symbol *symbol ) {
    printf("%s resolves to ", symbol->name);
    if (symbol->kind == SYMBOL_GLOBAL) printf("global %s\n", symbol->name);
    else if (symbol->kind == SYMBOL_LOCAL) printf("local %d\n", symbol->which);
    else printf("param %d\n", symbol->which);
}

const char * symbol_codegen( struct symbol *s ) {
    // global: name is same as in source language
    // local and func parameters: address computation
    //  -8(%rbp)
    if (!s) return NULL;
    switch (s->kind) {
        case SYMBOL_GLOBAL:
            return s->name;
        case SYMBOL_LOCAL:
        case SYMBOL_PARAM:
            {
                char str[100];
                sprintf(str, "%d(%%rbp)", -8*(s->which));
                return strdup(str);
            }
    }
}
