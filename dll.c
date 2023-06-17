#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// types
struct Node
{
    char file_name[256]; // max file name length is 255
    struct Node* prev;
    struct Node* next;
};

typedef struct Node* dll;

// GLOBALS
unsigned int LIST_LENGTH = 0;
// struct Node* LAST_REF = NULL;

// functions
struct Node* alloc_node(char* loc, struct Node* prev, struct Node* next)
{
    struct Node* new_node_ref = (struct Node*)malloc(sizeof(struct Node));

    strcpy(new_node_ref->file_name, loc);
    new_node_ref->prev = prev;
    new_node_ref->next = next;
    
    return new_node_ref;
}

void append_node(struct Node* base_ref, char* value)
{
    struct Node* new_end_ref = alloc_node(value, base_ref, NULL);

    if (base_ref != NULL)
        base_ref->next = new_end_ref;
    
    LIST_LENGTH++;
    // LAST_REF = new_end_ref;
}
