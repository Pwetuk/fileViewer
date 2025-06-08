#ifndef PATHS_TYPES_H
#define PATHS_TYPES_H

enum filter_type{
    FILTER_ALL          = 0,
    FILTER_VISIBLE      = 1,
    FILTER_NON_UTILITY  = 2
};

enum fs_error_code{
    NO_ERROR,
    PERMITION_DENIED,
    NOT_A_DIRECTORY,
    NOT_A_FILE,
    SOMETHING_WENT_WRONG,
    NO_ENTITY,
    ALREADY_EXISTS,
    CURRENTLY_IN_USE,
    CANT_RESOLVE_LINKS,
};

struct fs_error{
    enum fs_error_code error_code;
    char* message;
};
#endif