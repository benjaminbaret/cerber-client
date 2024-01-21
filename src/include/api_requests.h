#ifndef API_REQUESTS_H
#define API_REQUESTS_H


#include <curl/curl.h>
#include <glib-object.h>
#include <json-glib/json-glib.h>
#include "simple-json-parser.h"

#define MAX_LINE_LENGTH 200
#define MAX_DATA_LENGTH 200
#define MAX_ENTRIES 2
#define SIZE 250


#define HTTP_OK 200
#define HTTP_BAD_REQUEST 400
#define HTTP_UNAUTHORIZED 401
#define HTTP_FORBIDDEN 403

gchar* get_complete_url(const gchar* p_cUrl, const gchar* p_cRoute);

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

gchar* api_post_device_signin();

glong api_patch(char* p_cRoute, char* p_cJwtToken, char* p_cBody);

glong api_patch_status(char* p_cJwtToken, char* p_cStatus);
glong api_patch_progress(char* p_cJwtToken, char* p_cUpdateProgress);

gchar* api_get_update_next(char* p_cJwtToken);

#endif // API_REQUESTS_H