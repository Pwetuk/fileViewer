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

enum cd_error
fs_change_directory(struct FSaccess* main_access,
    const char* cd_to_dir)
{
    return pth_cd_dir(&(main_access->access_path), cd_to_dir);
}

int
fs_make_directory(struct FSaccess* main_access,
    const char* new_dir_name)
{
    return pth_mkdir_by_path(main_access->access_path, new_dir_name);
}

enum creation_error
fs_create_file(struct FSaccess* main_access, 
    const char* new_file_name)
{
    return pth_create_file(main_access->access_path, new_file_name);
}

enum remove_error
fs_remove_file(struct FSaccess* main_access, 
    const char* to_remove_file)
{
    return pth_remove_file(main_access->access_path, to_remove_file);
}

enum remove_error
fs_remove_directory(struct FSaccess* main_access, 
    const char* to_remove_dir)
{
    return pth_remove_directory(main_access->access_path, to_remove_dir);
}