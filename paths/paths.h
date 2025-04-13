#include <stdbool.h>


#ifndef PATHS_H
#define PATHS_H

extern char* DEFAULT_PATH;

typedef struct current_path current_path;

struct current_path{
    char* current;
    int length;
    struct current_path* previous;
};


typedef struct {
    struct current_path* path;

} file_viewer;


struct current_path*
previous_path(struct current_path* path);


struct current_path*
add_path(struct current_path* path, const char* next_entry);


bool
delete_path(struct current_path* path);


struct current_path*
init_path(const char* path);

void
print_path(struct current_path* path);

void
cascade_delete(struct current_path* path);

struct current_path*
up_path(struct current_path* path);

void
path_init();

void
path_clean();

#endif