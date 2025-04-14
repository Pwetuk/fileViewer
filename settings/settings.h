#ifdef FSACCESS_TYPES_H
#else
    #include "../FSaccess/FSaccess.h"
#endif

#ifdef SETTINGS_TYPE_H
#else
    #include "types.h"
#endif

struct settings* 
settings_init();


void
settings_clear(struct settings* to_delete_settings);

void
change_filter_mode(struct settings* settings, enum filter_type);
