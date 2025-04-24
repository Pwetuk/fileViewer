#include "paths.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "filters/filters.h"

char* DEFAULT_PATH = "/";

struct current_path*
pth_previous_path(struct current_path* path)
{
    if(!(path->previous)){
        return NULL;
    }

    struct current_path* previous_path;
    previous_path = path->previous;

    pth_delete_path(path);

    return previous_path;
}

struct current_path*
pth_add_path(struct current_path* path, const char* next_entry)
{   
    int length_entry = strlen(next_entry);

    if(next_entry[length_entry - 1] == '/'){
        length_entry -= 1;

    }
    char* full_path = malloc((length_entry + path->length + 1) * sizeof(char));
    
    strcpy(full_path, path->current);
    strcpy(full_path + path->length, next_entry);


    struct current_path* new_path;

    new_path = pth_init_path(full_path);
    new_path->previous = path;
    

    free(full_path);

    return new_path;
}

int
pth_delete_path(struct current_path* path)
{   
    if(!path){
        return 0;
    }
    free(path->current);
    free(path);
    return 1;
}

struct current_path*
pth_init_path(const char* path)
{   
    int path_length = strlen(path);
    if (path[path_length - 1] == '/'){
        path_length -= 1;
    }
    struct current_path* inited_path = malloc(sizeof(struct current_path));
    inited_path->length = path_length + 1;
    inited_path->current = malloc(sizeof(char)*(path_length + 2));
    
    strcpy(inited_path->current, path);

    inited_path->current[path_length] = '/';
    inited_path->current[path_length + 1] = '\0';

    inited_path->previous = NULL;
    inited_path->length = path_length + 1;

    return inited_path;
}

void
pth_print_path(struct current_path* path)
{
    printf("%s with length %d\n", path->current, path->length);
}

void
pth_cascade_delete(struct current_path* path)
{
    if(!path){
        return;
    }
    pth_cascade_delete(path->previous);
    pth_delete_path(path);
}

struct current_path*
pth_up_path(struct current_path* path){
    int length = path->length;

    if(path->length == 1){
        return path;
    }

    char* upper_path = malloc(sizeof(char) * (path->length + 1));

    strcpy(upper_path, path->current);

    for(int i = length - 2; upper_path[i] != '/'; i -= 1){
        length -= 1;
    }

    length -= 1;

    upper_path[length] = '\0';

    struct current_path* new_path = pth_init_path(upper_path);
    new_path->previous = path;

    free(upper_path);
    return new_path;
}

void
pth_delete_end_slash(struct current_path* path)
{
    path->current[path->length - 1] = '\0';
}

void
pth_add_end_slash(struct current_path* path)
{
    path->current[path->length - 1] = '/';
}

struct dirent**
pth_get_files_in_dir(
    struct current_path* path,
    int* number_of_entries,
    typeof(int (const struct dirent *)) *filter)
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
pth_get_file_in_dir(
    struct current_path* path,
    typeof(int (const struct dirent *)) *filter, 
    int type_of_file, 
    const char* needed)
{
    int length;
    struct dirent** files_in_dir = pth_get_files_in_dir(path, &length, filter);


    if(files_in_dir == NULL){
        return NULL;
    }

    struct dirent* return_file;

    for(int i = 0; i < length; ++i){
        if(strcmp(files_in_dir[i]->d_name, needed) == 0 && files_in_dir[i]->d_type == type_of_file){
            return_file = files_in_dir[i];
            pth_free_all_files_except(files_in_dir, length, i);
            return return_file;
        }
    }

    pth_free_all_files(files_in_dir, length);
    return NULL;
}


void 
pth_print_files_in_dir(struct current_path* path, enum filter_type filter_option)
{   
    int number_of_entries;
    typeof(int (const struct dirent*))* filter_func = pth_resolve_filter_type(filter_option);
    struct dirent** files_in_dir = pth_get_files_in_dir(path, &number_of_entries, filter_func);
    if(number_of_entries == 0){
        printf("\nNo files\n");
    }
    for(int i = 0; i < number_of_entries; ++i){
        printf("%s\n", files_in_dir[i]->d_name);
    }
    pth_free_all_files(files_in_dir, number_of_entries);
}

void
pth_free_all_files(struct dirent** dir_entries, int length)
{
    for(int i = 0; i < length; ++i){
        free(dir_entries[i]);
    }
    free(dir_entries);
}

void
pth_free_all_files_except(struct dirent** dir_entries, int length, int except_dirent)
{
    for(int i = 0; i < length; ++i){
        if(except_dirent != i){
            free(dir_entries[i]);
        }
    }
    free(dir_entries);
}

typeof(int (const struct dirent *))*
pth_resolve_filter_type(enum filter_type filter_option)
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
pth_mkdir_by_path(struct current_path* path, const char* dir_name)
{
    pth_delete_end_slash(path);
    int dir_fd = open(path->current, O_RDONLY | O_DIRECTORY);
    pth_add_end_slash(path);
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
pth_cd_dir(struct current_path** path, const char* dir_name)
{
    struct dirent* is_exists = pth_get_file_in_dir(*path, FILTER_ALL, DT_DIR, dir_name);
    if(is_exists == NULL){
        return CD_INVALID_DIRECTORY;
    }
    
    if(strcmp(is_exists->d_name, "..") == 0){
        *path = pth_up_path(*path);
    }else if(strcmp(is_exists->d_name, ".") != 0){
        *path = pth_add_path(*path, dir_name);
    }
    
    free(is_exists);

    return CD_NO_ERROR;
}

enum creation_error 
pth_create_file(struct current_path* path, 
    const char* new_file)
{
    int dir_d = open(path->current, O_DIRECTORY);
    if(dir_d == -1){
        return CR_SOMETHING_WENT_WRONG;
    }
    int file_d = openat(dir_d, new_file, O_WRONLY | O_CREAT, 0744);
    if(file_d == -1){
        return CR_SOMETHING_WENT_WRONG;
    }
    int close_file_d = close(file_d);
    if(close_file_d == -1){
        return CR_SOMETHING_WENT_WRONG;
    }
    int close_dir_d = close(dir_d);
    if(close_dir_d == -1){
        return CR_SOMETHING_WENT_WRONG;
    }
    return CR_NO_ERROR;
}

enum remove_error
pth_remove_file(struct current_path* path, 
    const char* remove_file)
{   
    int dir_d = open(path->current, O_DIRECTORY);

    if(dir_d == -1){
        return RM_SOMETHING_WENT_WRONG;
    }

    int error = unlinkat(dir_d, remove_file, 0);
    if(error == -1){
        return RM_SOMETHING_WENT_WRONG;
    }
    int close_dir_d = close(dir_d);
    if(close_dir_d == -1){
        return RM_SOMETHING_WENT_WRONG;
    }
    return RM_NO_ERROR;
}

enum remove_error
pth_remove_directory(struct current_path* path, 
    const char* remove_dir)
{   
    struct current_path* dir_path = pth_add_path(path, remove_dir);
    enum remove_error remove_files_error = pth_remove_all_files_from_dir(dir_path);

    if(remove_files_error != RM_NO_ERROR){
        pth_delete_path(dir_path);
        return remove_files_error;
    }

    int dir_d = open(path->current, O_DIRECTORY);
    if(dir_d == -1){
        pth_delete_path(dir_path);
        return RM_NO_RIGHTS;
    }

    int unlink_err = unlinkat(dir_d, remove_dir, AT_REMOVEDIR);

    if(unlink_err == -1){
        pth_delete_path(dir_path);
        return RM_NOT_A_DIRECTORY;
    }

    int close_dir_err = close(dir_d);
    if(close_dir_err == -1){
        pth_delete_path(dir_path);
        return RM_NOT_A_FILE;
    }
    pth_delete_path(dir_path);
    return RM_NO_ERROR;
}

enum remove_error
pth_remove_all_files_from_dir(struct current_path* dir_path)
{
    //printf("Starting to remove files from dir: %s\n", dir_path->current);
    int length;
    struct dirent** all_files = pth_get_files_in_dir(dir_path, &length, pth_resolve_filter_type(FILTER_NON_UTILITY));

    if(all_files == NULL){
        pth_free_all_files(all_files, length);
        return RM_NO_RIGHTS;
    }


    for(int i = 0; i < length; ++i){
        //printf("Deleting file: %s with number %d\n", all_files[i]->d_name, i);
        enum remove_error error = pth_remove_file(dir_path, all_files[i]->d_name);
        if(error != RM_NO_ERROR){
            pth_free_all_files(all_files, length);
            return error;
        }
    }
    pth_free_all_files(all_files, length);
    return RM_NO_ERROR;
}