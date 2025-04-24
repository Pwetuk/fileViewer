#ifndef FSACCESS_TYPES_H
#define FSACCESS_TYPES_H

struct FSaccess {
    struct current_path* access_path;
    struct settings* fs_settings;
};
#endif