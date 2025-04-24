#include "pth_types.h"
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
pth_previous_path(struct current_path* path);


struct current_path*
pth_add_path(struct current_path* path, const char* next_entry);


int
pth_delete_path(struct current_path* path);


struct current_path*
pth_init_path(const char* path);

void
pth_print_path(struct current_path* path);

void
pth_cascade_delete(struct current_path* path);

struct current_path*
pth_up_path(struct current_path* path);

void
pth_delete_end_slash(struct current_path* path);

void
pth_add_end_slash(struct current_path* path);

struct dirent**
pth_get_files_in_dir(struct current_path* path, int* number_of_entries, typeof(int (const struct dirent *)) *filter);


void
pth_print_files_in_dir(struct current_path* path, enum filter_type filter_option);


void
pth_free_all_files(struct dirent** dir_entries, int length);


typeof(int (const struct dirent *))*
pth_resolve_filter_type(enum filter_type filter_option);


int
pth_mkdir_by_path(struct current_path* path, const char* dir_name);


struct dirent*
pth_find_file_in_dir(struct current_path* path, typeof(int (const struct dirent *)) *filter);

enum cd_error
pth_cd_dir(struct current_path** path, const char* dir_name);

void
pth_free_all_files_except(struct dirent** dir_entries, int length, int except_dirent);

#endif