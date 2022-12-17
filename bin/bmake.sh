#!/bin/sh

if [ $# -ne 1  ]; then
    echo "Usage: ./bmake [file]"
    exit 1
fi

FILE=$1

if ./bminor -codegen $FILE out.s ; then
    if gcc -g out.s src/library.c -o prog ; then 
        ./prog
        echo "output: $?"
        exit 0
    else 
        echo "failure on gcc"
    fi
else 
    echo "failure on bminor"
fi
exit 1

