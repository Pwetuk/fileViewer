#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "paths/paths.h"
#include "FSaccess/FSaccess.h"
#include "FSaccess/consts.h"

int
main(int argc, char** argv)
{      
    path_init();
    char* a = malloc(sizeof(char)*100);
    if(argc > 1){
        strcpy(a, argv[1]);
    }else{
        strcpy(a, DEFAULT_PATH);
    }
    
    current_path* path = init_path(a);
    print_files_in_dir(path, FILTER_NON_UTILITY);
    free(a);
    delete_path(path);
    path_clean();
    return 0;
}