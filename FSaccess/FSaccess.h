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
