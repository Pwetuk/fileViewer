#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "paths/paths.h"
#include "FSaccess/FSaccess.h"
#include "FSaccess/types.h"
#include "settings/settings.h"

int
main(int argc, char** argv)
{      
    path_init();
    struct settings* file_viewer_settings = settings_init(NULL);


    char* a = malloc(sizeof(char)*100);
    if(argc > 1){
        strcpy(a, argv[1]);
    }else{
        strcpy(a, DEFAULT_PATH);
    }

    current_path* path = init_path(a);
    print_files_in_dir(path, file_viewer_settings->current_filter_type);   
    change_filter_mode(file_viewer_settings, FILTER_NON_UTILITY);
    printf("\n");
    print_files_in_dir(path, file_viewer_settings->current_filter_type);

    free(a);
    delete_path(path);
    path_clean();
    settings_clear(file_viewer_settings);
    return 0;
}