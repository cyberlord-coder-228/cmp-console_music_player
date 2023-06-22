#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
extern int errno;

#include "dll.h"

// data type
typedef struct
{
    char** array;
    unsigned int length;
} arr_cl;

// functions
char** _alloc_arr_(unsigned int length)
{
    // note: max file name in linux is 255 chars
    char** result = (char**)malloc(length*256*sizeof(char));
    for (int i = 0; i < length; i++) result[i] = (char*)malloc(256*sizeof(char));
    return result;
}

arr_cl alloc_arr_cl(unsigned int length)
{
    arr_cl result;
    result.array = _alloc_arr_(length);
    result.length = length;
    return result;
}

arr_cl arrayify(struct Node* start_ref)
{
    unsigned int length = get_length(start_ref);
    if (length == 0)
    {
        perror("Trying to arrayify NULL");
        return alloc_arr_cl(0);
    }

    arr_cl result = alloc_arr_cl(length);

    struct Node* local_node = start_ref;
    int i = 0;
    while (local_node != NULL)
    {
        strcpy(result.array[i], local_node->file_name);
        
        local_node = local_node->next;
        i++;
    }

    return result;
}

void _print_arr_(char** arr, unsigned int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("[%d]:\t%s\n", i, arr[i]);
    }
}

void print_arr_cl(arr_cl arr)
{
    _print_arr_(arr.array, arr.length);
}
