#include "FSaccess.h"
#include "paths/filters/filters.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include "settings/settings.h"

struct FSaccess*
fs_init(const char* path)
{
    struct FSaccess* main_access = malloc(sizeof(struct FSaccess));
    main_access->access_path = pth_init_path(path);
    main_access->fs_settings = settings_init();
    return main_access;
}

int
fs_delete(struct FSaccess* main_access)
{
    pth_cascade_delete(main_access->access_path);
    settings_clear(main_access->fs_settings);
    free(main_access);
    return 1;
}

struct dirent**
fs_get_files_in_dir(const struct FSaccess* main_access)
{
    int number_of_entries;
    return pth_get_files_in_dir(
        main_access->access_path,
        &number_of_entries,
        pth_resolve_filter_type((main_access->fs_settings)->current_filter_type)
    );
}


void
fs_print_files_in_current_dir(const struct FSaccess* main_access)
{
    pth_print_files_in_dir(
        main_access->access_path, 
        (main_access->fs_settings)->current_filter_type);
}

void
fs_change_filter_type(struct FSaccess* main_access,
    const enum filter_type filter_mode)
{
    change_filter_mode(main_access->fs_settings, filter_mode);
}