#include "token.h"
#include "string.h"
#include "scope.h"
#include "decl.h"
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
extern FILE *yyin;
extern int yylex();
extern char *yytext;
extern int yyparse();
extern struct decl* d;
typedef enum yytokentype token_t;
int typecheck_error;
int resolve_error;
int codegen_error;
FILE *fp;
#define TOKEN_EOF 0
   
/**
 * Display usage message.
 * @param       program     String containing name of program.
 **/
void usage(const char *program) {
    fprintf(stderr, "Usage: %s [options]\n\n", program);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "    -scan FILE           run scanner on FILE\n");
    fprintf(stderr, "    -parse FILE          run parser on FILE\n");
    fprintf(stderr, "    -print FILE          run printer on FILE\n");
    fprintf(stderr, "    -resolve FILE        run resolve on FILE\n");
    fprintf(stderr, "    -typecheck FILE      run typecheck on FILE\n");
}

void print_stripped_str(char * string_literal) {

    char *curr = string_literal;
    char *str_new = string_literal;
    curr++;

    while (*curr != '\0') {
        if (*curr == '\\') { 
            curr++;
            switch (*curr) {
                case 'n':
                    *str_new = '\n';
                    break;
                case '0':
                    *str_new = '\0';
                    break;
                default:
                    *str_new = *curr;
                    break;
            }
        } else {
            *str_new = *curr;
        }
        curr++;
        str_new++;
    }
    str_new--;
    *str_new = '\0';

    printf("%-20s %10s\n", "STRING LITERAL", string_literal);
}

void print_char(char * char_literal) {
    char * curr = char_literal;
    curr++;
    if (*curr == '\\') {
        curr++;
        switch(*curr) {
            case '0':
                printf("%-20s \0\n", "CHARACTER LITERAL");
                return;
            case 'n':
                printf("%-20s \n\n", "CHARACTER LITERAL");
                return;
            default:
                printf("%-20s %20c\n", "CHARACTER LITERAL", char_literal[2]);
                break;
        }
    } 
    printf("%-20s %10c\n", "CHARACTER LITERAL", char_literal[1]);
}

int scan (){
    while (1) {
        token_t t = yylex();
        if (t == TOKEN_EOF) break;
        if (t == TOKEN_STRING) {
            if (strlen(yytext) > 255) {
                printf("%-20s\n", "STRING TOO LONG");
                exit(EXIT_FAILURE);
            } else {
                print_stripped_str(yytext);
            }
        }
        else if (t == TOKEN_CHAR) print_char(yytext);
        else if (t == TOKEN_NUMBER) printf("%-20s %20s\n", "TOKEN_NUMBER", yytext);
        else if (t == TOKEN_IDENT) printf("%-20s %20s\n", "TOKEN_IDENT", yytext);
        else if (t == TOKEN_MULTI_COMMENT) printf("%-20s\n", "MULTILINE COMMENT");
        else if (t == TOKEN_SINGLE_COMMENT) printf("%-20s\n", "SINGLE COMMENT");
        else if (t == TOKEN_ARRAY) printf("%-20s\n", "ARRAY KEYWORD");
        else if (t == TOKEN_AUTO) printf("%-20s\n", "AUTO KEYWORD");
        else if (t == TOKEN_BOOLEAN) printf("%-20s\n", "BOOLEAN KEYWORD");
        else if (t == TOKEN_KEYWORD_CHAR) printf("%-20s\n", "CHAR KEYWORD");
        else if (t == TOKEN_INTEGER) printf("%-20s\n", "INTEGER KEYWORD");
        else if (t == TOKEN_ELSE) printf("%-20s\n", "ELSE KEYWORD");
        else if (t == TOKEN_FOR) printf("%-20s\n", "FOR KEYWORD");
        else if (t == TOKEN_FUNCTION) printf("%-20s\n", "FUNCTION KEYWORD");
        else if (t == TOKEN_IF) printf("%-20s\n", "IF KEYWORD");
        else if (t == TOKEN_PRINT) printf("%-20s\n", "PRINT KEYWORD");
        else if (t == TOKEN_RETURN) printf("%-20s\n", "RETURN KEYWORD");
        else if (t == TOKEN_KEYWORD_STRING) printf("%-20s\n", "STRING KEYWORD");
        else if (t == TOKEN_VOID) printf("%-20s\n", "VOID KEYWORD");
        else if (t == TOKEN_WHILE) printf("%-20s\n", "WHILE KEYWORD");
        else if (t == TOKEN_TRUE) printf("%-20s\n", "TRUE");
        else if (t == TOKEN_FALSE) printf("%-20s\n", "FALSE");
        else if (t == TOKEN_AND) printf("%-20s\n", "AND");
        else if (t == TOKEN_OR) printf("%-20s\n", "OR");
        else if (t == TOKEN_TERNARY) printf("%-20s\n", "TERNARY");
        else if (t == TOKEN_COLON) printf("%-20s\n", "COLON");
        else if (t == TOKEN_SEMICOLON) printf("%-20s\n", "SEMICOLON");
        else if (t == TOKEN_COMMA) printf("%-20s\n", "COMMA");
        else if (t == TOKEN_LBRACKET) printf("%-20s\n", "LBRACKET");
        else if (t == TOKEN_RBRACKET) printf("%-20s\n", "RBRACKET");
        else if (t == TOKEN_LBRACE) printf("%-20s\n", "LBRACE");
        else if (t == TOKEN_RBRACE) printf("%-20s\n", "RBRACE");
        else if (t == TOKEN_LPAREN) printf("%-20s\n", "LPAREN");
        else if (t == TOKEN_RPAREN) printf("%-20s\n", "RPAREN");
        else if (t == TOKEN_ASSIGNMENT) printf("%-20s\n", "ASSIGNMENT");
        else if (t == TOKEN_EXPONENT) printf("%-20s\n", "EXPONENT");
        else if (t == TOKEN_INCREMENT) printf("%-20s\n", "INCREMENT");
        else if (t == TOKEN_DECREMENT) printf("%-20s\n", "DECREMENT");
        else if (t == TOKEN_ADD) printf("%-20s\n", "ADD");
        else if (t == TOKEN_HYPHEN) printf("%-20s\n", "HYPHEN");
        else if (t == TOKEN_ASTERISK) printf("%-20s\n", "ASTERISK");
        else if (t == TOKEN_FORWARD_SLASH) printf("%-20s\n", "FORWARD SLASH");
        else if (t == TOKEN_PERCENT) printf("%-20s\n", "PERCENT");
        else if (t == TOKEN_EQ) printf("%-20s\n", "EQUALS");
        else if (t == TOKEN_LE) printf("%-20s\n", "LE");
        else if (t == TOKEN_GE) printf("%-20s\n", "GE");
        else if (t == TOKEN_LT) printf("%-20s\n", "LT");
        else if (t == TOKEN_GT) printf("%-20s\n", "GT");
        else if (t == TOKEN_NE) printf("%-20s\n", "NE");
        else if (t == TOKEN_NOT) printf("%-20s\n", "NOT");
        else if (t == TOKEN_ERROR_CHAR) {
            fprintf(stderr, "%-20s %20s\n", "TOKEN ERROR CHAR", yytext);
            exit(EXIT_FAILURE);
        } else if (t == TOKEN_ERROR_INVALID) {
            fprintf(stderr, "%-20s %20s\n", "TOKEN ERROR INVALID", yytext);
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
}

int parse() {
    int y = yyparse();
    if  (!y) {
        printf("parse successful!\n");
        return 0;
    } else {
        printf("parse failed.\n");
        return 1;
    }
}

int print() {
    int y = yyparse();
    if (y) {
        printf("parse failed.\n");
        return 1;
    }
    decl_print(d, 0);
    return 0;
}

int resolve() {
    int y = yyparse();
    if (y) {
        printf("parse failed.\n");
        return 1;
    }
    resolve_error = 0;
    struct scope *s = scope_create(1, hash_table_create(0,0), 0, 0);
    decl_resolve(s, d);
    return resolve_error;
}

int typecheck() {
    int r = resolve();
    if (r) {
        printf("resolve failed.\n");
        return 1;
    }
    typecheck_error = 0;
    decl_typecheck(d);
    return typecheck_error;
}

int codegen(char *assembly) {
    int t = typecheck();
    if (t) {
        printf("typecheck failed.\n");
        return 1;
    }
    codegen_error = 0;
    fp = fopen(assembly, "w");
    decl_codegen(d);
    return codegen_error;

}

int main(int argc, char*argv[]) {
    int argind = 1;
    char *opt;
    char *file;
    char *assembly;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-')
    {
        if (strcmp(argv[argind], "-scan") == 0) {
            argind++;
            file = argv[argind];
            yyin = fopen(file, "r");
            if (!yyin) {
                usage(argv[0]);
                return 1;
            }
            return scan();

        } else if (strcmp(argv[argind],"-parse") == 0) {
            argind++;
            file = argv[argind];
            yyin = fopen(file, "r");
            if (!yyin) {
                usage(argv[0]);
                return 1;
            }
            return parse();
            
        } else if (strcmp(argv[argind], "-print") == 0) {
            argind++;
            file = argv[argind];
            yyin = fopen(file, "r");
            if (!yyin) {
                usage(argv[0]);
                return 1;
            }
            return print();

        } else if (strcmp(argv[argind], "-resolve") == 0) {
            argind++;
            file = argv[argind];
            yyin = fopen(file, "r");
            if (!yyin) {
                usage(argv[0]);
                return 1;
            }
            return resolve();
        } else if (strcmp(argv[argind], "-typecheck") == 0) {
            argind++;
            file = argv[argind];
            yyin = fopen(file, "r");
            if (!yyin) {
                usage(argv[0]);
                return 1;
            }
            return typecheck();
        } else if (strcmp(argv[argind], "-codegen") == 0) {
            argind++;
            file = argv[argind];
            argind++;
            assembly = argv[argind];
            yyin = fopen(file, "r");
            if (!yyin) {
                usage(argv[0]);
                return 1;
            }
            return codegen(assembly);
        } else {
            usage(argv[0]);
            return 1;
        }
    }
}


/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
