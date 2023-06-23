#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dll.h"
#include "array.h"

struct Node* _search_music_(char* path)
{
    DIR* dir = opendir(path);
    if (dir == NULL) return NULL;

    struct Node* found_list_start_ref = NULL;
    struct Node* working_node_ref = NULL;
    
    // loop through each file in the directory
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // check if directory
        if (entry->d_type == DT_DIR)
        {
            // skip the "." and ".." directories
            if (
                strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0
            ) continue;

            // recursively search the subdirectory
            char sub_path[PATH_MAX];
            snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
            
            // append (all) newly found to already found
            struct Node* sub_found = _search_music_(sub_path);
            if (sub_found != NULL)
            {
                if (found_list_start_ref == NULL) 
                {
                    found_list_start_ref = alloc_node(
                        sub_found->file_name,
                        NULL,
                        NULL
                    );
                    working_node_ref = found_list_start_ref;
                    append_list(found_list_start_ref, sub_found->next);
                }
                else
                {
                    append_list(found_list_start_ref, sub_found);
                }
            }
        }
        else if (entry->d_type == DT_REG)
        {
            // check if the file is a music file
            char *dot_end = strrchr(entry->d_name, '.');
            if (!dot_end) continue;

            if (
                strcmp(dot_end, ".flac") == 0
                || strcmp(dot_end, ".mp3") == 0 // not supported yet
                || strcmp(dot_end, ".mpv") == 0 // not supported yet
                // etc.
            )
            {
                char location[PATH_MAX];
                snprintf(
                    location,
                    sizeof(location),
                    "%s/%s",
                    path,
                    entry->d_name
                );

                // append music file to found
                if (found_list_start_ref == NULL)
                {
                    found_list_start_ref = alloc_node(location, NULL, NULL);
                    working_node_ref = found_list_start_ref;
                }
                else
                {
                    append_node(working_node_ref, location);
                    working_node_ref = working_node_ref->next;
                }
            }
        }
    }

    // close the directory
    closedir(dir);

    return found_list_start_ref;
}

arr_cl search_music(char* path)
{
    struct Node* list_found = _search_music_(path);
    return arrayify(list_found);
}
