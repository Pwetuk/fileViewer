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

void
fs_change_directory(struct FSaccess* main_access,
    const char* cd_to_dir)
{
    struct fs_error cd_err = pth_cd_dir(&(main_access->access_path), cd_to_dir);
    fs_handle_error(cd_err, "fs_change_directory");
}

void
fs_make_directory(struct FSaccess* main_access,
    const char* new_dir_name)
{
    struct fs_error mk_err = pth_mkdir_by_path(main_access->access_path, new_dir_name);
    fs_handle_error(mk_err, "fs_make_directory");
}

void
fs_create_file(struct FSaccess* main_access, 
    const char* new_file_name)
{
    struct fs_error create_err = pth_create_file(main_access->access_path, new_file_name);
    fs_handle_error(create_err, "fs_create_file");
}

void
fs_remove_file(struct FSaccess* main_access, 
    const char* to_remove_file)
{
    struct fs_error remove_err = pth_remove_file(main_access->access_path, to_remove_file);
    fs_handle_error(remove_err, "fs_remove_file");
}

void
fs_remove_directory(struct FSaccess* main_access, 
    const char* to_remove_dir)
{
    struct fs_error remover_err = pth_remove_directory(main_access->access_path, to_remove_dir);
    fs_handle_error(remover_err, "fs_remove_directory");
}

void
fs_handle_error(struct fs_error handling_error, const char* original_function)
{
    enum fs_error_code handling_error_code = handling_error.error_code;
    switch (handling_error_code)
    {
    case NO_ERROR:
        return;
    case PERMITION_DENIED:
        printf("Permition denied in %s\n", original_function);
        break;
    case NOT_A_DIRECTORY:
        printf("Not a directory in %s\n", original_function);
        break;
    case NOT_A_FILE:
        printf("Not a file in %s\n", original_function);
        break;
    case NO_ENTITY:
        printf("Unable to open directory in %s\n", original_function);
        break;
    case ALREADY_EXISTS:
        printf("Cant create file/directory file already exists in %s\n", original_function);
        break;
    case CURRENTLY_IN_USE:
        printf("Cant process file/dir. File is open in %s\n", original_function);
        break;
    case CANT_RESOLVE_LINKS:
        printf("Problem with resolving links in %s", original_function);
        break;
    default:
        printf("Cant resolve error in %s\n", original_function);
        break;
    }
    printf("Additional message is: %s\n", handling_error.message);
    free(handling_error.message);
}