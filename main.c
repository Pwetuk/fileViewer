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
    fs_make_directory(main_access, "new_dir1");
    enum cd_error cd_result = fs_change_directory(main_access, "new_dir");
    if(cd_result == CD_INVALID_DIRECTORY){
        printf("No directory\n");
    }

    fs_print_files_in_current_dir(main_access);

    enum creation_error creation_result = fs_create_file(main_access, "test.txt");
    if(creation_result != CR_NO_ERROR){
        printf("Something went wrong with create\n");
    }

    creation_result = fs_create_file(main_access, "test2.txt");
    if(creation_result != CR_NO_ERROR){
        printf("Something went wrong with create2\n");
    }


    enum remove_error rm_result = fs_remove_file(main_access, "test.txt");
    if(rm_result != RM_NO_ERROR){
        printf("Something went wrong with remove\n");
    }

    cd_result = fs_change_directory(main_access, "..");
    if(cd_result == CD_INVALID_DIRECTORY){
        printf("No directory\n");
    }

    cd_result = fs_change_directory(main_access, "new_dir1");

    creation_result = fs_create_file(main_access, "to_rm1");
    creation_result = fs_create_file(main_access, "to_rm2");
    creation_result = fs_create_file(main_access, "to_rm3");

    cd_result = fs_change_directory(main_access, "..");


    //pth_print_path(main_access->access_path);
    //fs_change_directory(main_access, "new_dir");
    //pth_remove_all_files_from_dir(main_access->access_path);
    //struct current_path* some = pth_add_path(main_access->access_path, "new_dir1");

    rm_result = fs_remove_directory(main_access, "new_dir1");
    switch (rm_result)
    {
    case RM_NO_RIGHTS:
        printf("Faild to get all files\n");
        break;
    case RM_SOMETHING_WENT_WRONG:
        printf("Something went wrong with rm directory\n");
        break;
    case RM_NOT_A_DIRECTORY:
        printf("Cant unlink\n");
        break;
    case RM_NOT_A_FILE:
        printf("Cant close\n");
        break;
    default:
        printf("All good\n");
        break;
    }

    

    free(a);
    fs_delete(main_access);
    return 0;
}