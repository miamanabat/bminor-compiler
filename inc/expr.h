#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include "scratch.h"
#include "scope.h"

struct scope;
typedef enum {
    EXPR_ASSIGNMENT=0,
    EXPR_TERNARY,

    EXPR_OR=20,

    EXPR_AND= 40,

    EXPR_EQ=60,
    EXPR_LE,
    EXPR_GE,
    EXPR_LT,
    EXPR_GT,
    EXPR_NE,

	EXPR_ADD=80,
	EXPR_SUB,

	EXPR_MUL=100,
	EXPR_DIV,
    EXPR_MOD,

    EXPR_EXP=120,

    EXPR_NOT=140,
    EXPR_NEGATE,

    EXPR_DECREMENT=160,
    EXPR_INCREMENT,

    EXPR_FUNC_CALL=180,
    EXPR_ARR_INDEX,

    EXPR_NUMBER=200,
    EXPR_STRING,
    EXPR_CHAR,
    EXPR_BOOL,
    EXPR_IDENT,
    EXPR_ARR_INIT,
    EXPR_LIST,
	/* many more kinds of exprs to add here */
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
    struct expr *mid;
	const char *name;
	int literal_value;
	const char * string_literal;
    const char * char_literal;
	struct symbol *symbol;
    int reg;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );
struct expr * expr_create_ternary( struct expr *left, struct expr *mid, struct expr *right );
struct expr * expr_create_func_call( char *n, struct expr * expr );

struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( const char *c );
struct expr * expr_create_string_literal( const char *str );

void compare_precedence( struct expr* expr, expr_t kind, int eq_not_needed );

void expr_print( struct expr *e );
void expr_resolve( struct scope *s, struct expr *e );
struct type * expr_typecheck( struct expr *e );
void expr_type_print( struct expr *e, struct type *t );
int check_arr_types(struct expr *e);
int count_expr_list( struct expr *e);

struct expr * expr_copy( struct expr *e );

void expr_codegen( struct expr *e );

void expr_codegen_comp( struct expr *e, char *comp );

void expr_codegen_funccall(struct expr *e, char *name );

void expr_codegen_incdec(struct expr *e, char * command);

void expr_codegen_comp_str(struct expr *e, char *name);
#endif
