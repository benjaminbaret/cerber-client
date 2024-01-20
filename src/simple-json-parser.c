#include "include/simple-json-parser.h"


/**
 * @brief Get the value object of the json config file
 * @return value if found, null if not
*/
gchar* get_value_from_config_file(gchar* p_cJsonKey)
{
    JsonParser *parser;
    GError *error;

    gchar* l_cValue = NULL;

    parser = json_parser_new();

    error = NULL;
    json_parser_load_from_file(parser, CONFIG_FILE, &error);

    if(error)
    {
        g_error_free(error);
        g_object_unref(parser);
        return NULL;
    }

    JsonReader *reader = json_reader_new(json_parser_get_root(parser));
    gchar** members = json_reader_list_members(reader);
    
    gint i = 0;
    while (members[i] != 0) {
        gchar* m = members[i];
        
        if (strcmp(m, p_cJsonKey) == 0) {
            json_reader_read_member (reader, members[i]);
            l_cValue = json_reader_get_string_value (reader);
        }
        i++;
    }

    g_object_unref(parser);

    return l_cValue;
}