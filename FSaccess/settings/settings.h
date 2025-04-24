#include "settings_types.h"
#include "../paths/filters/filters.h"
#ifdef PATHS_TYPES_H
#else
    #include "../paths/pth_types.h"
#endif

struct settings* 
settings_init();


void
settings_clear(struct settings* to_delete_settings);

void
change_filter_mode(struct settings* settings, const enum filter_type filter_mode);
