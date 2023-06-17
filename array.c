#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dll.h"

char** alloc_arr(unsigned int length)
{
    // note: max file name in linux is 255 chars
    char** result = (char**)malloc(length*256*sizeof(char));
    for (int i = 0; i < length; i++) result[i] = (char*)malloc(256*sizeof(char));
    return result;
}

char** arrayify(struct Node* start_ref)
{
    unsigned int length = get_length(start_ref);

    char** result = alloc_arr(length);

    struct Node* local_node = start_ref;
    int i = 0;
    while (local_node != NULL)
    {
        strcpy(result[i], local_node->file_name);
        
        local_node = local_node->next;
        i++;
    }

    return result;
}

// void main()
// {
//     struct Node* test_node_ref = alloc_node("a", NULL, NULL);
//     append_node(test_node_ref, "b");

//     char** r = arrayify(test_node_ref);

//     printf("%s\n", r[0]);
//     printf("%s\n", r[1]);
// }
