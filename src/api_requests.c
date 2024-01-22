#include "include/api_requests.h"

/**
 * @brief Write callback for cURL
 * @param contents : The content of the response
 * @param size : The size of the response
 * @param nmemb : The number of members
 * @param userp : The user pointer
 * @return size_t : The size of the response
*/
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    gchar *data =  (gchar *)userp;

    // Append the received data to the existing buffer
    memcpy(data, contents, realsize);
    return realsize;
}

/**
 * @brief Get the complete url from the base url and the route
 * @param p_cUrl : The base url
 * @param p_cRoute : The route
 * @return gchar* : The complete url
 * @note The returned string must be freed
*/
gchar* get_complete_url(const gchar* p_cUrl, const gchar* p_cRoute) {

    size_t total_length = strlen(p_cUrl) + strlen(p_cRoute) + 1;

    gchar* l_cCompleteUrl = (gchar*)malloc(total_length);

    if (l_cCompleteUrl != NULL) {
        strcpy(l_cCompleteUrl, p_cUrl);
        strcat(l_cCompleteUrl, p_cRoute);
    }

    return l_cCompleteUrl;
}

/**
 * @brief Post the device signin request
 * @return gchar* : The bearer token if found, null if not
 * @note The returned string must be freed
*/
gchar* api_post_device_signin()
{
    CURL *curl;
    CURLcode res;
    GError **error = NULL;

    JsonParser *parser = json_parser_new();;
    JsonObject *root;

    const gchar* l_cUrl = get_value_from_config_file("url");    
    const gchar* l_cSignature = get_value_from_config_file("signature");
    const gchar* l_cPassword = get_value_from_config_file("password");

    const gchar *json_template;
    gchar *json_data = NULL;
    gchar *response_buffer;


    if(l_cUrl == NULL || l_cSignature == NULL || l_cPassword == NULL)
    {
        g_error("Error on config file : Missing a value");
        goto out;
    }

    gchar* l_cConcatenatedUrl = get_complete_url(l_cUrl, "/auth/signin");

    curl = curl_easy_init();

    if (!curl) 
    {
        g_error("Starting libcurl session failed");
        goto out;
    }

    /* ----- Building HTTP Request ----- */

    /* ---- Path ---- */
    curl_easy_setopt(curl, CURLOPT_URL, l_cConcatenatedUrl);

    /* ---- Method ---- */
    curl_easy_setopt(curl, CURLOPT_POST, 1L); 

    /* ---- Header ---- */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /*  ---- Body ---- */
    json_template = "{\"signature\":\"%s\",\"password\":\"%s\"}";
    size_t json_data_size = snprintf(NULL, 0, json_template, l_cSignature, l_cPassword) + 1;
    json_data =  (gchar *)malloc(json_data_size);

    if (!json_data) {
        g_error("Memory allocation failed.");
        goto out;
    }
    snprintf(json_data, json_data_size, json_template, l_cSignature, l_cPassword);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    response_buffer =  (gchar*)malloc(sizeof (gchar) * 200);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);


    /* ---- Send request ---- */
    g_message("Sending POST request at %s", l_cConcatenatedUrl);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        g_error("cURL request failed: %s\n", curl_easy_strerror(res));
        goto out;
    }

    /* ---- Parsing response ---- */
    response_buffer[200] = '\0'; // ensure buffer is null-terminated
    glong http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    json_parser_load_from_data(parser, response_buffer, -1, NULL);
    root = json_node_get_object(json_parser_get_root(parser));
    gchar* l_cToken = NULL;

    g_message("HTTP code: %ld\n", http_code);

    switch(http_code)
    {
        case HTTP_OK:
        {
            const gchar *l_cResponse = json_object_get_string_member(root, "access_token");
            l_cToken = l_cResponse;
            break;
        }
        case HTTP_BAD_REQUEST:
        case HTTP_FORBIDDEN:
        {
            
            const gchar *message = json_object_get_string_member(root, "message");
            g_warning("Error on curl, message: %s", message);
            break;
        }
        default:
        {
            break;
        }

    }

    g_free(l_cConcatenatedUrl);
    g_free(json_data);
    g_free(response_buffer);
    return l_cToken;

out:
    g_free(l_cConcatenatedUrl);
    g_free(json_data);
    g_free(response_buffer);
    return NULL;
}

/**
 * @brief Generic Patch method
 * @param p_cJwtToken : The JWT token
 * @param p_cBody : The body of the request
 * @return gchar* status code and response body
*/

glong api_patch (gchar* p_cRoute, gchar* p_cJwtToken, gchar* p_cBody)
{
    CURL *curl;
    CURLcode res;
    GError **error = NULL;
    JsonParser *parser = json_parser_new();;
    JsonObject *root;
    gchar* l_cConcatenatedUrl;

    glong http_code = -1;

    const gchar* l_cUrl = get_value_from_config_file("url");    

    curl = curl_easy_init();

    if(l_cUrl == NULL)
    {
        g_error("Error on config file : Missing a value");
        goto out;
    }

    if (!curl)
    {
        g_error("Starting libcurl session failed");
        goto out;
    }

    l_cConcatenatedUrl = get_complete_url(l_cUrl, p_cRoute);

    gchar *json_template;
    gchar *json_data = NULL;
    gchar *response_buffer;

    /* ----- Building HTTP Request ----- */

    /* ---- Path ---- */
    curl_easy_setopt(curl, CURLOPT_URL, l_cConcatenatedUrl);

    /* ---- Method ---- */
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");

    /* ---- Header ---- */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    gchar authorizationHeader[SIZE];  // Adjust the size as needed
    snprintf(authorizationHeader, sizeof(authorizationHeader), "Authorization: Bearer %s", p_cJwtToken);
    headers = curl_slist_append(headers, authorizationHeader);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /* ---- Body ---- */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, p_cBody);

    /* ---- Response ---- */
    response_buffer =  (gchar*)malloc(sizeof (gchar) * 200);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
    

    /* ---- Send request ---- */
    g_message("Sending PATCH request at %s", l_cConcatenatedUrl);

    res = curl_easy_perform(curl); 

    if (res != CURLE_OK) {
        g_error("cURL request failed: %s\n", curl_easy_strerror(res));
        goto out;
    }


    /* ---- Parsing response ---- */
    response_buffer[200] = '\0'; // ensure buffer is null-terminated
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    g_message("%s", response_buffer);

    if(http_code != 200)
    {
        g_warning("HTTP CODE %ld received, message %s:", http_code, response_buffer);
    }

    g_free(l_cConcatenatedUrl);
    g_free(response_buffer);
    return http_code;

out:
    g_free(l_cConcatenatedUrl);
    g_free(response_buffer);
    return http_code;
}

/**
 * @brief Patch the device status
 * @param p_cJwtToken : The JWT token
 * @param p_cStatus : The status to patch
 * @return glong : The status code from the http response
*/
glong api_patch_status(gchar* p_cJwtToken, gchar* p_cStatus)
{
    gchar* l_cRoute = "/device/status";
    gchar* l_cJsonBody = NULL;
    gchar* l_cJsonBodyTemplate = "{ \"status\" : \"%s\"}}";
    glong l_lHttpCode;

    
    /* ---- Building the body ---- */
    size_t l_sizeJsonDataSize = snprintf(NULL, 0, l_cJsonBodyTemplate, &p_cStatus) + 1;
    l_cJsonBody =  (gchar *)malloc(l_sizeJsonDataSize);

    if (!l_cJsonBody) {
        g_warning("Memory allocation failed.");
        goto out;
    }

    snprintf(l_cJsonBody, l_sizeJsonDataSize, l_cJsonBodyTemplate, p_cStatus);

    g_message("Body: %s", l_cJsonBody);
    printf("Body: %s", l_cJsonBody);


    /* ---- Sending API request ---- */

    l_lHttpCode = api_patch(l_cRoute, p_cJwtToken, l_cJsonBody);
    
    g_free(l_cJsonBody);
    return l_lHttpCode;;
out: 
    g_free(l_cJsonBody);
    return -1;
}


/**
 * @brief Patch the device progress
 * @param p_cJwtToken : The JWT token
 * @param p_cUpdateProgress : The progress to patch
 * @return glong : The status code from the http response
*/
glong api_patch_progress (gchar* p_cJwtToken, gchar* p_cUpdateProgress)
{
    gchar* l_cRoute = "/device/progress";
    gchar* l_cJsonBody = NULL;
    gchar* l_cJsonBodyTemplate = "{ \"updateProgress\" : %s}";
    
    /* ---- Building the body ---- */
    size_t l_sizeJsonDataSize = snprintf(NULL, 0, l_cJsonBodyTemplate, &p_cUpdateProgress) + 1;
    l_cJsonBody =  (gchar *)malloc(l_sizeJsonDataSize);

    if (!l_cJsonBody) {
        g_warning("Memory allocation failed.");
        goto out;
    }

    snprintf(l_cJsonBody, l_sizeJsonDataSize, l_cJsonBodyTemplate, p_cUpdateProgress);

    g_message("Body: %s", l_cJsonBody);
    printf("Body: %s", l_cJsonBody);

    return api_patch(l_cRoute, p_cJwtToken, l_cJsonBody);

out:
    return -1;
}

/**
 * @brief Get the next update url
 * @param p_cJwtToken : The JWT token
 * @return gchar* : The next update url
 * @note The returned string must be freed
*/
char* api_get_update_next (gchar* p_cJwtToken)
{
    CURL *curl;
    CURLcode res;
    GError **error = NULL;
    JsonParser *parser = json_parser_new();;
    gchar* l_cConcatenatedUrl;

    glong http_code = -1;

    const gchar* l_cUrl = get_value_from_config_file("url");    

    curl = curl_easy_init();

    if(l_cUrl == NULL)
    {
        g_error("Error on config file : Missing a value");
        goto out;
    }

    if (!curl)
    {
        g_error("Starting libcurl session failed");
        goto out;
    }

    l_cConcatenatedUrl = get_complete_url(l_cUrl, "/device/update/next");

    gchar *json_template;
    gchar *json_data = NULL;
    gchar *response_buffer;

    /* ----- Building HTTP Request ----- */

    /* ---- Path ---- */
    curl_easy_setopt(curl, CURLOPT_URL, l_cConcatenatedUrl);

    /* ---- Method ---- */
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");

    /* ---- Header ---- */
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    gchar authorizationHeader[SIZE];  // Adjust the size as needed
    snprintf(authorizationHeader, sizeof(authorizationHeader), "Authorization: Bearer %s", p_cJwtToken);
    headers = curl_slist_append(headers, authorizationHeader);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /* ---- Response ---- */
    response_buffer =  (gchar*)malloc(sizeof (gchar) * 200);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
    

    /* ---- Send request ---- */
    g_message("Sending GET request at %s", l_cConcatenatedUrl);

    res = curl_easy_perform(curl); 

    if (res != CURLE_OK) {
        g_error("cURL request failed: %s\n", curl_easy_strerror(res));
        goto out;
    }

    /* ---- Parsing response ---- */
    response_buffer[200] = '\0'; // ensure buffer is null-terminated
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    g_message("%s", response_buffer);

    if(http_code != 200)
    {
        g_warning("HTTP CODE %ld received, message %s:", http_code, response_buffer);
        goto out;
    }

    json_parser_load_from_data(parser, response_buffer, -1, NULL);
    JsonObject *root = json_node_get_object(json_parser_get_root(parser));
    gchar *url = json_object_get_string_member(root, "uri");

    g_free(l_cConcatenatedUrl);    
    return url;

out:
    g_free(l_cConcatenatedUrl);
    return "";
}