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

void
fs_change_directory(struct FSaccess* main_access, const char* cd_to_dir);

void
fs_make_directory(struct FSaccess* main_access, const char* new_dir_name);

void
fs_create_file(struct FSaccess* main_access, const char* new_file_name);

void
fs_remove_file(struct FSaccess* main_access, const char* to_remove_file);

void
fs_remove_directory(struct FSaccess* main_access, const char* to_remove_dir);

void
fs_handle_error(struct fs_error handling_error, const char* original_function);