#include <glib-object.h>
#include <json-glib/json-glib.h>

#define CONFIG_FILE "/data/cerber-config.json"
//#define CONFIG_FILE "/home/nicolas/OTA/Dev/cerber-client/src/cerber-config.json"

gchar* get_value_from_config_file(gchar* p_cJsonKey);