#ifndef MY_DLL_H
#define MY_DLL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct Node
{
    char file_name[256]; // max file name length is 255
    struct Node* prev;
    struct Node* next;
};

// extern unsigned int LIST_LENGTH;

unsigned int get_length(struct Node* start_ref);
struct Node* get_last(struct Node* start_ref);
struct Node* alloc_node(char* loc, struct Node* prev, struct Node* next);
void append_node(struct Node* base_ref, char* value);
void append_list(struct Node* base_ref, struct Node* upbuild_ref);

#endif /*MY_DLL_H*/
