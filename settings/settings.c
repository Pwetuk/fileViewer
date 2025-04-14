#include "../FSaccess/types.h"
#include "settings.h"
#include <stdlib.h>

struct settings*
settings_init(char* config_path)
{
    struct settings* file_viewer_settings = malloc(sizeof(struct settings));
    file_viewer_settings->current_filter_type = FILTER_VISIBLE;
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