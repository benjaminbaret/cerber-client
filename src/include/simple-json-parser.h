#include <glib-object.h>
#include <json-glib/json-glib.h>

#define CONFIG_FILE "/home/ben/OTA/Dev/bunk/src/cerber-config.json"

gchar* get_value_from_config_file(gchar* p_cJsonKey);