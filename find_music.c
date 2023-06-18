#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

#include "dll.h"
#include "array.h"

struct Node* _search_music_(char* path)
{
    // printf("called at %s\n", path); // used for debugging

    DIR* dir = opendir(path);
    if (dir == NULL) return NULL;

    struct Node* found_list_start_ref = NULL;
    struct Node* working_node_ref = NULL;
    
    // loop through each file in the directory
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        // printf("%s\n", entry->d_name); // used for debugging
        // check if directory
        if (entry->d_type == DT_DIR)
        {
            // skip the "." and ".." directories
            if (
                strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0
            ) continue;

            // recursively search the subdirectory
            char sub_path[256];
            snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name); // do i need this tho?
            
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
            if (
                dot_end && (
                    !strcmp(dot_end, ".flac")
                    || !strcmp(dot_end, ".mp3")
                    // can add more file extensions here
                )
            )
            {
                char location[256];
                snprintf(
                    location,
                    sizeof(location),
                    "%s/%s",
                    path,
                    entry->d_name
                );

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

                // printf("\t%s\n", working_node_ref->file_name); // used for debugging
            }
        }
    }

    // close the directory
    closedir(dir);

    return found_list_start_ref;
}

arr_cl search_music(char* path)
{
    return arrayify(_search_music_(path));
}

void main()
{
    // char search_path[256] = "/run/media/vlad"; // thats my external drive
    char search_path[256] = "./music"; // thats my test folder

    arr_cl music_arr = search_music(search_path);

    printf("Music found at %s: \n", search_path);
    print_arr_cl(music_arr);
}
