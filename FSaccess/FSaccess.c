#include "FSaccess.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

struct dirent**
get_files_in_dir(struct current_path* path, int* number_of_entries)
{
    struct dirent** files_in_dir;

    *number_of_entries = scandir(
        path->current,
        &files_in_dir,
        NULL,
        alphasort
    );

    if(*number_of_entries == -1){
        free(number_of_entries);
        return NULL;
    }

    return files_in_dir;
}


void
print_files_in_dir(struct current_path* path)
{   
    int number_of_entries;
    struct dirent** files_in_dir = get_files_in_dir(path, &number_of_entries);
    for(int i = 0; i < number_of_entries; ++i){
        printf("%s\n", files_in_dir[i]->d_name);
    }
    free_all_files(files_in_dir, number_of_entries);
}

void
free_all_files(struct dirent** dir_entries, int length){
    for(int i = 0; i < length; ++i){
        free(dir_entries[i]);
    }
    free(dir_entries);
}