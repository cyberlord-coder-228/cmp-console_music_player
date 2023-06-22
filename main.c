#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#include "dll.h"
#include "array.h"
#include "find_music.h"
#include "play_music.h"

void main(int argc, char *argv[])
{
    char working_directory[PATH_MAX];

    // Get the current working directory
    if (getcwd(working_directory, sizeof(working_directory)) != NULL)
    {
        printf("Current working directory: %s\n", working_directory);
    }
    else
    {
        perror("getcwd");
        return;
    }

    arr_cl music_arr = search_music(working_directory);

    printf("Music found at %s: \n", working_directory);
    print_arr_cl(music_arr);

    init_alsa();

    for (int i = 0; i < music_arr.length; i++)
    {
        play_flac(music_arr.array[0]);
    }

    exit_alsa();
}