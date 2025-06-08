#include "paths.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "filters/filters.h"
#include <errno.h>

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

char*
pth_get_path_string(struct current_path* path)
{
    char* path_string = malloc(sizeof(char) * 1000);
    strcpy(path_string, path->current);
    return path_string;
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


struct fs_error
pth_mkdir_by_path(struct current_path* path, const char* dir_name)
{
    pth_delete_end_slash(path);
    int dir_fd = open(path->current, O_RDONLY | O_DIRECTORY);
    pth_add_end_slash(path);
    if(dir_fd == -1){
        return pth_handle_errno_open(path, "");
    }
    int result = mkdirat(dir_fd, dir_name, 0755);
    if(result == -1){
        close(dir_fd);
        return pth_handle_errno_open(path, dir_name);
    }
    result = close(dir_fd);
    if(result == -1){
        return pth_handle_errno_close(path, "");
    }
    return create_no_error_fs_error();
}


struct fs_error
pth_cd_dir(struct current_path** path, const char* dir_name)
{
    struct dirent* is_exists = pth_get_file_in_dir(*path, FILTER_ALL, DT_DIR, dir_name);
    if(is_exists == NULL){
        return create_fs_error(NO_ENTITY, pth_get_path_string(*path));
    }
    
    if(strcmp(is_exists->d_name, "..") == 0){
        *path = pth_up_path(*path);
    }else if(strcmp(is_exists->d_name, ".") != 0){
        *path = pth_add_path(*path, dir_name);
    }
    
    free(is_exists);

    return create_no_error_fs_error();
}

struct fs_error 
pth_create_file(struct current_path* path, 
    const char* new_file)
{
    int dir_d = open(path->current, O_DIRECTORY);
    if(dir_d == -1){
        
        return pth_handle_errno_open(path, "");
    }
    int file_d = openat(dir_d, new_file, O_WRONLY | O_CREAT, 0744);
    if(file_d == -1){
        return pth_handle_errno_open(path, new_file);
    }
    int close_file_d = close(file_d);
    if(close_file_d == -1){
        return pth_handle_errno_close(path, new_file);
    }
    int close_dir_d = close(dir_d);
    if(close_dir_d == -1){
        return pth_handle_errno_close(path, "");
    }
    return create_no_error_fs_error();
}

struct fs_error
pth_remove_file(struct current_path* path, 
    const char* remove_file)
{   
    int dir_d = open(path->current, O_DIRECTORY);

    if(dir_d == -1){
        return pth_handle_errno_open(path, "");
    }

    int error = unlinkat(dir_d, remove_file, 0);
    if(error == -1){
        return pth_handle_errno_unlink(path, remove_file);
    }
    int close_dir_d = close(dir_d);
    if(close_dir_d == -1){
        return pth_handle_errno_close(path, "");
    }
    return create_no_error_fs_error();
}

struct fs_error
pth_remove_directory(struct current_path* path, 
    const char* remove_dir)
{   
    struct current_path* dir_path = pth_add_path(path, remove_dir);
    struct fs_error remove_files_error = pth_remove_all_files_from_dir(dir_path);

    if(remove_files_error.error_code != NO_ERROR){
        pth_delete_path(dir_path);
        return remove_files_error;
    }

    int dir_d = open(path->current, O_DIRECTORY);
    if(dir_d == -1){
        pth_delete_path(dir_path);
        return pth_handle_errno_open(path, "");
    }

    int unlink_err = unlinkat(dir_d, remove_dir, AT_REMOVEDIR);

    if(unlink_err == -1){
        pth_delete_path(dir_path);
        return pth_handle_errno_unlink(path, "");
    }

    int close_dir_err = close(dir_d);
    if(close_dir_err == -1){
        pth_delete_path(dir_path);
        return pth_handle_errno_close(path, "");
    }
    pth_delete_path(dir_path);
    return create_no_error_fs_error();
}

struct fs_error
pth_remove_all_files_from_dir(struct current_path* dir_path)
{
    int length;
    struct dirent** all_files = pth_get_files_in_dir(dir_path, &length, pth_resolve_filter_type(FILTER_NON_UTILITY));

    if(length == 0){
        //pth_free_all_files(all_files, length);
        return create_no_error_fs_error();
    }else if(length == -1){
        return pth_handle_errno_open(dir_path, "");
    }


    for(int i = 0; i < length; ++i){
        struct fs_error error;
        error.error_code = NO_ERROR;
        if(all_files[i]->d_type == DT_DIR){
            pth_remove_directory(dir_path, all_files[i]->d_name);
        }else if(all_files[i]->d_type == DT_REG){
            error = pth_remove_file(dir_path, all_files[i]->d_name);
        }else{
            error = create_fs_error(SOMETHING_WENT_WRONG, "Something went wrong");
        }
        if(error.error_code != NO_ERROR){
            pth_free_all_files(all_files, length);
            
            return error;
        }
    }
    pth_free_all_files(all_files, length);
    return create_no_error_fs_error();
}

struct fs_error
pth_handle_errno_open(struct current_path* needed_out, const char* file)
{
    printf("Error occured while opening\n");
    struct current_path* error_in_path = pth_add_path(needed_out, file);

    enum fs_error_code returning_code;


    switch (errno)
    {
    case EACCES:
        returning_code = PERMITION_DENIED;
        break;
    case EEXIST:
        returning_code = ALREADY_EXISTS;
        break;
    case EISDIR:
        returning_code = NOT_A_FILE;
        break;
    case ENOTDIR:
        returning_code = NOT_A_DIRECTORY;
        break;
    case ENOENT:
        returning_code = NO_ENTITY;
        break;
    default:
        printf("Error code:%d\n", errno);
        returning_code = SOMETHING_WENT_WRONG;
        break;
    }
    char* error_in_path_string = pth_get_path_string(error_in_path);
    pth_delete_path(error_in_path);

    return create_fs_error(returning_code, error_in_path_string);
}

struct fs_error
pth_handle_errno_close(struct current_path* needed_out, const char* file)
{
    struct current_path* error_in_path = pth_add_path(needed_out, file);
    char* error_in_path_string = pth_get_path_string(error_in_path);
    pth_delete_path(error_in_path);
    return create_fs_error(SOMETHING_WENT_WRONG, error_in_path_string);
}


struct fs_error
pth_handle_errno_unlink(struct current_path* needed_out, const char* file){
    printf("Error occured while unlinkig\n");
    struct current_path* error_in_path = pth_add_path(needed_out, file);
    char* error_in_path_string = pth_get_path_string(error_in_path);
    pth_delete_path(error_in_path); 

    enum fs_error_code returning_code;

    switch (errno)
    {
    case EPERM:
    case EACCES:
        returning_code = PERMITION_DENIED;
        break;
    case EBUSY:
        returning_code = CURRENTLY_IN_USE;
        break;
    case ELOOP:
        returning_code = CANT_RESOLVE_LINKS;
        break;
    case ENOTDIR:
        returning_code = NOT_A_DIRECTORY;
        break;
    case EISDIR:
        returning_code = NOT_A_FILE;
        break;
    case ENOENT:
        returning_code = NO_ENTITY;
        break;
    default:
        printf("Error code:%d\n", errno);
        returning_code = SOMETHING_WENT_WRONG;
        break;
    }
    return create_fs_error(returning_code, error_in_path_string);
}

struct fs_error
create_fs_error(enum fs_error_code error_code, char* messg){
    struct fs_error new_error;
    new_error.error_code = error_code;
    new_error.message = messg;
    return new_error;
}

struct fs_error
create_no_error_fs_error(){
    struct fs_error new_error;
    new_error.error_code = NO_ERROR;
    return new_error;
}