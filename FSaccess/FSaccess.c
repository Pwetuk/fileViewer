#include "FSaccess.h"
#include "filters.h"
#include "types.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#ifdef PATHS_H
#else
    #include "../paths/paths.h"
#endif

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

struct dirent*
get_file_in_dir(struct current_path* path, typeof(int (const struct dirent *)) *filter, enum file_type type_of_file, const char* needed)
{
    int length;
    struct dirent** files_in_dir = get_files_in_dir(path, &length, filter);


    if(files_in_dir == NULL){
        return NULL;
    }

    struct dirent* return_file;

    for(int i = 0; i < length; ++i){
        if(strcmp(files_in_dir[i]->d_name, needed) == 0 && files_in_dir[i]->d_type == type_of_file){
            return_file = files_in_dir[i];
            free_all_files_except(files_in_dir, length, i);
            return return_file;
        }
    }

    free_all_files(files_in_dir, length);
    return NULL;
}


void 
print_files_in_dir(struct current_path* path, enum filter_type filter_option)
{   
    int number_of_entries;
    typeof(int (const struct dirent*))* filter_func = resolve_filter_type(filter_option);
    struct dirent** files_in_dir = get_files_in_dir(path, &number_of_entries, filter_func);
    if(number_of_entries == 0){
        printf("\nNo files\n");
    }
    for(int i = 0; i < number_of_entries; ++i){
        printf("%s\n", files_in_dir[i]->d_name);
    }
    free_all_files(files_in_dir, number_of_entries);
}

void
free_all_files(struct dirent** dir_entries, int length)
{
    for(int i = 0; i < length; ++i){
        free(dir_entries[i]);
    }
    free(dir_entries);
}

void
free_all_files_except(struct dirent** dir_entries, int length, int except_dirent)
{
    for(int i = 0; i < length; ++i){
        if(except_dirent != i){
            free(dir_entries[i]);
        }
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


int
mkdir_by_path(struct current_path* path, const char* dir_name)
{
    delete_end_slash(path);
    print_path(path);
    int dir_fd = open(path->current, O_RDONLY | O_DIRECTORY);
    add_end_slash(path);
    if(dir_fd == -1){
        return -1;
    }
    int result = mkdirat(dir_fd, dir_name, 0755);
    if(result == -1){
        printf("Failed to create dir\n");
        close(dir_fd);
        return -1;
    }
    result = close(dir_fd);
    if(result == -1){
        return -1;
    }
    return 1;
}


enum cd_error
cd_dir(struct current_path** path, const char* dir_name)
{
    struct dirent* is_exists = get_file_in_dir(*path, FILTER_ALL, D_DIRECTORY, dir_name);
    if(is_exists == NULL){
        return NO_DIRECTORY;
    }
    
    if(strcmp(is_exists->d_name, "..") == 0){
        *path = up_path(*path);
    }else if(strcmp(is_exists->d_name, ".") != 0){
        *path = add_path(*path, dir_name);
    }
    
    free(is_exists);

    return NO_ERROR;
}
