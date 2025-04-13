#include <dirent.h>

#define filter_all NULL;

int filter_visible(const struct dirent* dir);


int filter_local(const struct dirent* dir);
