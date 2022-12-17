#include <stdio.h>
#include "string.h"

int regs_scratch [7];
    //"%%rbx", "%%r10", "%%r11", "%%r12", "%%r13", "%%r14", "%%r15"

int regs_arg [6];
    //"%%rdi", "%%rsi", "%%rdx", "%%rcx", "%%r8", "%%r9"


int label_count = 0;

/* scratch functions */
int scratch_alloc() {
    for (int i = 0; i < 7; i++) {
        if (regs_scratch[i] == 0) {
            regs_scratch[i] = 1;
            return i;
        }
    }
    printf("eep no more registers!\n\n");
    return -1;
}

void scratch_free( int r ) {
    regs_scratch[r] = 0;
}

const char * scratch_name( int r ) {
    if (r >= 7) {
        return NULL;
    }
    const char * reg_names[7] = {
        "%rbx", "%r10", "%r11",
        "%r12", "%r13", "%r14", "%r15"
    };
    return reg_names[r];
}

const char * arg_name ( int r ) {
    if (r >= 6) {
        fprintf(stdout, "cannot have more than 6 registers!\n");
    }
    const char * reg_names[6] = {
        "%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"
    };
    return reg_names[r];

}


/* label functions */

int label_create() {
    int label = label_count;
    label_count += 1;
    return label;
}
const char * label_name( int label ) {
    char s[10];
    snprintf(s, 10, ".L%d", label);
    return strdup(s);
}


