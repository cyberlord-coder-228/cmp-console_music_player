#ifndef MY_ARRAY_H
#define MY_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dll.h"

typedef struct
{
    char** array;
    unsigned int length;
} arr_cl;

arr_cl alloc_arr_cl(unsigned int length);
arr_cl arrayify(struct Node* start_ref);
void print_arr_cl(arr_cl arr);

#endif /*MY_ARRAY_H*/
