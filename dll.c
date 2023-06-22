#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// type
struct Node
{
    char file_name[PATH_MAX];
    struct Node* prev;
    struct Node* next;
};

// functions
unsigned int get_length(struct Node* start_ref)
{
    struct Node* local_ref = start_ref;
    unsigned int result = 0;

    while (local_ref != NULL)
    {
        result++;
        local_ref = local_ref->next;
    }

    return result;
}

struct Node* get_last(struct Node* start_ref)
{
    if (start_ref == NULL)
    {
        fprintf(stderr, "get_last was called on an empty list\n");
        return NULL;
    }

    struct Node* result = start_ref;
    while (result->next != NULL)
    {
        result = result->next;
    }

    return result;
}

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
    if (base_ref == NULL)
    {
        fprintf(stderr, "In append_node, base_ref is NULL\n");
        return;
    }

    base_ref->next = alloc_node(value, base_ref, NULL);
}

void append_list(struct Node* base_ref, struct Node* upbuild_ref)
{
    if (base_ref == NULL)
    {
        fprintf(stderr, "In append_list, base_ref is NULL\n");
        return;
    }
    if (upbuild_ref == NULL)
    {
        fprintf(stderr, "In append_list, upbuild_ref is NULL\n");
        return;
    }

    struct Node* the_last_ref = get_last(base_ref);
    struct Node* working_node_ref = upbuild_ref;
    while (working_node_ref != NULL)
    {
        if (the_last_ref == NULL)
        {
            fprintf(stderr, "In append_list, invalid get_last call\n");
            return;
        }
        // append one by one
        append_node(the_last_ref, working_node_ref->file_name);
        // move to the next
        the_last_ref = the_last_ref->next;
        working_node_ref = working_node_ref->next;
    }
}
