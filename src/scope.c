#include "scope.h"
#include "hash_table.h"
#include <stdlib.h>

struct scope *scope_create(int level, struct hash_table *hash_table, struct scope *prev, struct scope *next) {
    struct scope *s = malloc(sizeof(struct scope));
    s->level = level;
    s->hash_table = hash_table;
    s->prev = prev;
    s->next = next;
    s->var_count = 0;
    s->param_count = 0;
    return s;
}
void scope_enter(struct scope *s) {
    if (!s) return;

    struct hash_table *h = hash_table_create(0,0);
    struct scope *new_scope = scope_create(s->level + 1, h, s, NULL);
    s->next = new_scope;
}

void scope_exit(struct scope *s) {
    //remove top hash table
    if (!s) return;

    if (s->prev != NULL) s->prev->next = NULL;
    hash_table_delete(s->hash_table);
}

int scope_level(struct scope *s) { 
    return s->level;
    // returns number of hash tables in current stack
}

int scope_bind( struct scope *s, const char *name, struct symbol *sym  ) {
    // adds entry to the top of the stack, mapping name to the symbol structure sym
    if (hash_table_insert(s->hash_table, name, sym)) return 0;
    else return 1;    
}

struct symbol *scope_lookup( struct scope *s, const char *name  ) {
    // searches stack of hash tables from top to bottom, looking for the first entry that matches name exactly
    // if no match is found, returns null
    if (!s) return NULL;
    struct symbol *sym = hash_table_lookup(s->hash_table, name);
    if (!sym) sym = scope_lookup(s->prev, name);
    return sym;
}

struct symbol *scope_lookup_current( struct scope *s, const char *name  ) {
    // only searches the topmost table 
    // used to determine whether a symbol has already been defined in the current scope
    if (!s) return NULL;
    return hash_table_lookup(s->hash_table, name);
}

int inc_var_counter( struct scope *s ) {
    s->var_count += 1;
    return s->var_count;
}

void reset_var_counter( struct scope *s ) {
    s->var_count = 0;
}
