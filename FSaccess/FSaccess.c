#include "FSaccess.h"
#include "filters.h"
#include "types.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

struct dirent**
get_files_in_dir(struct current_path* path, int* number_of_entries, typeof(int (const struct dirent *)) *filter)
{
    struct dirent** files_in_dir;

    *number_of_entries = scandir(
        path->current,
        &files_in_dir,
        filter,
        alphasort
    );

    if(*number_of_entries == -1){
        free(number_of_entries);
        return NULL;
    }

    return files_in_dir;
}


void
print_files_in_dir(struct current_path* path, enum filter_type filter_option)
{   
    int number_of_entries;
    typeof(int (const struct dirent*))* filter_func = resolve_filter_type(filter_option);
    struct dirent** files_in_dir = get_files_in_dir(path, &number_of_entries, filter_func);
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

typeof(int (const struct dirent *))*
resolve_filter_type(enum filter_type filter_option)
{
    switch (filter_option)
    {
    case FILTER_VISIBLE:
        return filter_visible;
    case FILTER_NON_UTILITY:
        return filter_local;
    case FILTER_ALL:
        return filter_all;
    }
    return filter_all;
}
