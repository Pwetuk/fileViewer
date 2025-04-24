#ifndef PATHS_TYPES_H
#define PATHS_TYPES_H

enum filter_type{
    FILTER_ALL          = 0,
    FILTER_VISIBLE      = 1,
    FILTER_NON_UTILITY  = 2
};

enum cd_error{
    CD_INVALID_DIRECTORY, 
    CD_NO_DIRECTORY, 
    CD_NO_ERROR
};

enum creation_error {
    CR_NO_ERROR, 
    CR_NO_RIGHTS, 
    CR_SOMETHING_WENT_WRONG
};

enum remove_error {
    RM_NO_ERROR, 
    RM_NO_RIGHTS, 
    RM_NOT_A_DIRECTORY, 
    RM_NOT_A_FILE, 
    RM_SOMETHING_WENT_WRONG
};
#endif