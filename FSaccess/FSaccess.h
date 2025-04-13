#include <dirent.h>
#ifdef PATH_H
#else
    #include <../paths/paths.h>
#endif

struct dirent**
get_files_in_dir(struct current_path* path, int* number_of_entries, typeof(int (const struct dirent *)) *filter);

void
print_files_in_dir(struct current_path* path, int filter_type);

void
free_all_files(struct dirent** dir_entries, int length);

typeof(int (const struct dirent *))*
resolve_filter_type(int filter_type);