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