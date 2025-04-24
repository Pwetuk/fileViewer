#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "FSaccess/FSaccess.h"

int
main(int argc, char** argv)
{      
    char* a = malloc(sizeof(char)*100);
    if(argc > 1){
        strcpy(a, argv[1]);
    }else{
        strcpy(a, DEFAULT_PATH);
    }

    struct FSaccess* main_access = fs_init(a);
    fs_print_files_in_current_dir(main_access);
    fs_change_filter_type(main_access, FILTER_NON_UTILITY);

    printf("\n");
    //mkdir_by_path(path, "new_dir");
    //enum cd_error result = cd_dir(&path, "new_dir");
    //if(result == NO_DIRECTORY){
    //    printf("No directory\n");
    //}

    //print_files_in_dir(path, file_viewer_settings->current_filter_type);

    fs_print_files_in_current_dir(main_access);

    free(a);
    fs_delete(main_access);
    return 0;
}