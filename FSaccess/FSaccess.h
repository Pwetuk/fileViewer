#include <dirent.h>

#include "fs_types.h"
#include "paths/paths.h"

struct FSaccess*
fs_init(const char* path);

int
fs_delete(struct FSaccess* main_access);


struct dirent**
fs_get_files_in_dir(const struct FSaccess* main_access);

void
fs_print_files_in_current_dir(const struct FSaccess* main_access);

void
fs_change_filter_type(struct FSaccess* main_access, const enum filter_type);

enum cd_error
fs_change_directory(struct FSaccess* main_access, const char* cd_to_dir);


int
fs_make_directory(struct FSaccess* main_access, const char* new_dir_name);

enum creation_error
fs_create_file(struct FSaccess* main_access, const char* new_file_name);

enum remove_error
fs_remove_file(struct FSaccess* main_access, const char* to_remove_file);

enum remove_error
fs_remove_directory(struct FSaccess* main_access, const char* to_remove_dir);