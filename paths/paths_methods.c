#include "paths.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

char* DEFAULT_PATH;

struct current_path*
previous_path(struct current_path* path)
{
    if(!(path->previous)){
        return NULL;
    }

    struct current_path* previous_path;
    previous_path = path->previous;

    delete_path(path);

    return previous_path;
}

struct current_path*
add_path(struct current_path* path, const char* next_entry)
{   
    int length_entry = strlen(next_entry);

    if(next_entry[length_entry - 1] == '/'){
        length_entry -= 1;

    }
    char* full_path = malloc((length_entry + path->length + 1) * sizeof(char));
    
    strcpy(full_path, path->current);
    strcpy(full_path + path->length, next_entry);


    struct current_path* new_path;

    new_path = init_path(full_path);
    new_path->previous = path;
    

    free(full_path);

    return new_path;
}

bool
delete_path(struct current_path* path)
{   
    if(!path){
        return false;
    }
    free(path->current);
    free(path);
    return true;
}

struct current_path*
init_path(const char* path)
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
print_path(struct current_path* path)
{
    printf("%s with length %d\n", path->current, path->length);
}

void
cascade_delete(struct current_path* path)
{
    if(!path){
        return;
    }
    cascade_delete(path->previous);
    delete_path(path);
}

struct current_path*
up_path(struct current_path* path){
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

    struct current_path* new_path = init_path(upper_path);
    new_path->previous = path;

    free(upper_path);
    return new_path;
}

void
path_init(){
    int length_path = 100;
    DEFAULT_PATH = malloc(100*sizeof(char));
    if(!getcwd(DEFAULT_PATH, length_path)){
        DEFAULT_PATH[0] = '/';
        DEFAULT_PATH[1] = '\0';
    }

}

void
path_clean(){
    free(DEFAULT_PATH);
}