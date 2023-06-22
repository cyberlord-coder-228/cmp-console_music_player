#ifndef MY_DLL_H
#define MY_DLL_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char file_name[PATH_MAX];
    struct Node* prev;
    struct Node* next;
};

unsigned int get_length(struct Node* start_ref);
struct Node* get_last(struct Node* start_ref);
struct Node* alloc_node(char* loc, struct Node* prev, struct Node* next);
void append_node(struct Node* base_ref, char* value);
void append_list(struct Node* base_ref, struct Node* upbuild_ref);

#endif /*MY_DLL_H*/
