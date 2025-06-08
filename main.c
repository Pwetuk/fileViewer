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
    /*
    fs_make_directory(main_access, "new_dir1");
    fs_change_directory(main_access, "new_dir");

    fs_print_files_in_current_dir(main_access);

    fs_create_file(main_access, "test.txt");

    fs_create_file(main_access, "test2.txt");

    fs_remove_file(main_access, "test.txt");

    fs_change_directory(main_access, "..");


    fs_change_directory(main_access, "new_dir1");

    fs_create_file(main_access, "to_rm1");
    fs_create_file(main_access, "to_rm2");
    fs_create_file(main_access, "to_rm3");

    fs_change_directory(main_access, "..");

    fs_remove_directory(main_access, "new_dir1");
    */
    fs_remove_directory(main_access, "new_dir");

    free(a);
    fs_delete(main_access);
    return 0;
}