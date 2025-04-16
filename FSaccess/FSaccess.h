#include <dirent.h>

#ifdef FSACCESS_TYPES_H
#else
    #include "types.h"
#endif

#ifdef PATH_H
#else
    #include <../paths/paths.h>
#endif


struct dirent**
get_files_in_dir(struct current_path* path, int* number_of_entries, typeof(int (const struct dirent *)) *filter);


void
print_files_in_dir(struct current_path* path, enum filter_type filter_option);


void
free_all_files(struct dirent** dir_entries, int length);


typeof(int (const struct dirent *))*
resolve_filter_type(enum filter_type filter_option);


int
mkdir_by_path(struct current_path* path, const char* dir_name);


struct dirent*
find_file_in_dir(struct current_path* path, typeof(int (const struct dirent *)) *filter);

enum cd_error
cd_dir(struct current_path** path, const char* dir_name);

void
free_all_files_except(struct dirent** dir_entries, int length, int except_dirent);