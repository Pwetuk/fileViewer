#include "settings.h"
#include "../paths/pth_types.h"
#include <stdlib.h>
struct settings*
settings_init()
{
    struct settings* file_viewer_settings = malloc(sizeof(struct settings));
    file_viewer_settings->current_filter_type = FILTER_VISIBLE;
    return file_viewer_settings;
}


void
settings_clear(struct settings* to_delete_settings)
{
    free(to_delete_settings);
}

void
change_filter_mode(struct settings* change_settings, enum filter_type filter_option)
{
    change_settings->current_filter_type = filter_option;
}