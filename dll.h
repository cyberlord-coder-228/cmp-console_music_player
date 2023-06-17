#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node
{
    char file_name[256]; // max file name length is 255
    struct Node* prev;
    struct Node* next;
};

extern unsigned int LIST_LENGTH;

struct Node* alloc_node(char* loc, struct Node* prev, struct Node* next);
void append_node(struct Node* base_ref, char* value);
