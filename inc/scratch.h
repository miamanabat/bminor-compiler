
#ifndef SCRATCH_H
#define SCRATCH_H

int scratch_alloc();

void scratch_free( int r );

const char * scratch_name( int r );

const char * arg_name ( int r );

/* label functions */
int label_create();
const char * label_name( int label );

#endif
