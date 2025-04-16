#ifndef FSACCESS_TYPES_H
#define FSACCESS_TYPES_H

enum filter_type{FILTER_ALL, FILTER_VISIBLE, FILTER_NON_UTILITY};
enum file_type{
    D_UNKNOWN             = 0,
    D_PIPE                = 1,
    D_CHARACTER_DEVICE    = 2,
    D_DIRECTORY           = 4,
    D_BLOCK_DEVICE        = 6,
    D_FILE                = 8,
    D_LINK                = 10,
    D_SOCKET              = 12
};
enum cd_error{NO_DIRECTORY, NO_ERROR};
#endif