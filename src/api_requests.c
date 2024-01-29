#include "api_requests.h"

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
 * @return http* : The bearer token if found + the http code
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
        g_warning("Error on config file : Missing a value");
        goto out;
    }
    gchar* l_cConcatenatedUrl = get_complete_url(l_cUrl, "/auth/signin");
    curl = curl_easy_init();
    if (!curl) 
    {
        g_warning("Starting libcurl session failed");
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
        g_warning("Memory allocation failed.");
        goto out;
    }
    snprintf(json_data, json_data_size, json_template, l_cSignature, l_cPassword);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    response_buffer =  (gchar*)malloc(sizeof (gchar) * 200);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
    /* ---- Send request ---- */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    g_message("Sending POST request at %s", l_cConcatenatedUrl);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        g_warning("cURL request failed: %s\n", curl_easy_strerror(res));
        goto out;
    }
    /* ---- Parsing response ---- */
    response_buffer[199] = '\0'; // ensure buffer is null-terminated
    glong http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    json_parser_load_from_data(parser, response_buffer, -1, NULL);
    //g_message("Response buffer %s", response_buffer);
    root = json_node_get_object(json_parser_get_root(parser));
    gchar* l_cJwtToken = NULL;
    g_message("HTTP code: %ld\n", http_code);
    switch(http_code)
    {
        case HTTP_OK:
        {
            const gchar *l_cResponse = json_object_get_string_member(root, "access_token");
            l_cJwtToken = l_cResponse;
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
    return l_cJwtToken;
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
        g_warning("Error on config file : Missing a value");
        goto out;
    }

    if (!curl)
    {
        g_warning("Starting libcurl session failed");
        goto out;
    }

    l_cConcatenatedUrl = get_complete_url(l_cUrl, p_cRoute);

    gchar *json_template;
    gchar *json_data = NULL;
    gchar *response_buffer;

    /* ----- Building HTTP Request ----- */

    /* ---- Path ---- */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
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
        g_warning("cURL request failed: %s\n", curl_easy_strerror(res));
        goto out;
    }


    /* ---- Parsing response ---- */
    response_buffer[199] = '\0'; // ensure buffer is null-terminated
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    //g_message("Response buffer : %s", response_buffer);
    g_message("HTTP code: %ld\n", http_code);
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
glong api_patch_device_status(gchar* p_cJwtToken, gchar* p_cStatus)
{
    gchar* l_cRoute = "/device/deviceStatus";
    gchar* l_cJsonBody = NULL;
    gchar* l_cJsonBodyTemplate = "{ \"deviceStatus\" : \"%s\"}";
    glong l_lHttpCode;

    
    /* ---- Building the body ---- */
    size_t l_sizeJsonDataSize = snprintf(NULL, 0, l_cJsonBodyTemplate, p_cStatus) + 1;
    l_cJsonBody = (gchar *)malloc(l_sizeJsonDataSize);

    if (!l_cJsonBody) {
        g_warning("Memory allocation failed.");
        goto out;
    }

    snprintf(l_cJsonBody, l_sizeJsonDataSize, l_cJsonBodyTemplate, p_cStatus);


    g_message("Body: %s", l_cJsonBody);


    /* ---- Sending API request ---- */

    l_lHttpCode = api_patch(l_cRoute, p_cJwtToken, l_cJsonBody);
    
    g_free(l_cJsonBody);
    return l_lHttpCode;;
out: 
    g_free(l_cJsonBody);
    return -1;
}

/**
 * @brief Patch the update status
 * @param p_cJwtToken : The JWT token
 * @param p_cStatus : The status to patch
 * @return glong : The status code from the http response
*/
glong api_patch_update_status(gchar* p_cJwtToken, gchar* p_cStatus)
{
    gchar* l_cRoute = "/device/updateStatus";
    gchar* l_cJsonBody = NULL;
    gchar* l_cJsonBodyTemplate = "{ \"updateStatus\" : \"%s\"}";
    glong l_lHttpCode;

    
    /* ---- Building the body ---- */
    size_t l_sizeJsonDataSize = snprintf(NULL, 0, l_cJsonBodyTemplate, p_cStatus) + 1;
    l_cJsonBody = (gchar *)malloc(l_sizeJsonDataSize);

    if (!l_cJsonBody) {
        g_warning("Memory allocation failed.");
        goto out;
    }

    snprintf(l_cJsonBody, l_sizeJsonDataSize, l_cJsonBodyTemplate, p_cStatus);


    g_message("Body: %s", l_cJsonBody);


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
    size_t l_sizeJsonDataSize = snprintf(NULL, 0, l_cJsonBodyTemplate, p_cUpdateProgress) + 1;
    l_cJsonBody =  (gchar *)malloc(l_sizeJsonDataSize);

    if (!l_cJsonBody) {
        g_warning("Memory allocation failed.");
        goto out;
    }

    snprintf(l_cJsonBody, l_sizeJsonDataSize, l_cJsonBodyTemplate, p_cUpdateProgress);

    g_message("Body: %s", l_cJsonBody);

    return api_patch(l_cRoute, p_cJwtToken, l_cJsonBody);

out:
    return -1;
}

/**
 * @brief Get the next update url
 * @param p_cJwtToken : The JWT token
 * @return http* : The next update url + the http code
 * @note The returned string must be freed
*/
http* api_get_update_next (gchar* p_cJwtToken)
{
    CURL *curl;
    CURLcode res;
    GError **error = NULL;
    http *l_http = (http*)malloc(sizeof(http));

    JsonParser *parser = json_parser_new();;
    gchar* l_cConcatenatedUrl;

    l_http->code = -1;

    const gchar* l_cUrl = get_value_from_config_file("url");    

    curl = curl_easy_init();

    if(l_cUrl == NULL)
    {
        g_warning("Error on config file : Missing a value");
        goto out;
    }

    if (!curl)
    {
        g_warning("Starting libcurl session failed");
        goto out;
    }

    l_cConcatenatedUrl = get_complete_url(l_cUrl, "/device/update/next");

    gchar *json_template;
    gchar *json_data = NULL;
    gchar *response_buffer;

    /* ----- Building HTTP Request ----- */

    /* ---- Path ---- */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
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
    response_buffer =  (gchar*)malloc(sizeof (gchar) * 2000);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);
    

    /* ---- Send request ---- */
    g_message("Sending GET request at %s", l_cConcatenatedUrl);

    res = curl_easy_perform(curl); 

    if (res != CURLE_OK) {
        g_warning("cURL request failed: %s\n", curl_easy_strerror(res));
        goto out;
    }

    /* ---- Parsing response ---- */
    response_buffer[2000] = '\0'; // ensure buffer is null-terminated
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &l_http->code);
    //g_message("Response buffer : %s", response_buffer);
    if(l_http->code != 200)
    {
        g_warning("HTTP CODE %ld received, message %s:", l_http->code, response_buffer);
        goto out;
    }

    json_parser_load_from_data(parser, response_buffer, -1, NULL);
    JsonObject *root = json_node_get_object(json_parser_get_root(parser));
    l_http->body = json_object_get_string_member(root, "url");

    g_free(l_cConcatenatedUrl);    
    return l_http;

out:
    g_free(l_cConcatenatedUrl);
    return NULL;
}

/**
 * @brief Poll for updates on the API
 * @param jwtToken : The JWT token
 * @return gchar: The number of updates available
 */
gchar* poll_for_updates(gchar* p_cJwtToken) {
    gint l_iPolling = 1;
    gchar* l_cUpdateUrl;
    
    do {
        l_cUpdateUrl = api_get_update_next(p_cJwtToken)->body;
        if(l_cUpdateUrl == NULL)
        {
            g_warning("Error on api_get_update_next");
            return NULL;
        }

        if (l_cUpdateUrl != NULL && strlen(l_cUpdateUrl) > 0) {
            // Stop polling
            l_iPolling = 0;

            // Handle the update URL, for example, initiate a download or perform other actions
            printf("Update available at: %s\n", l_cUpdateUrl);
        } else {
            // No update available, you can log this if needed
            printf("No update available.\n");
        }

        // Sleep for the specified interval before polling again
        sleep(1);

    } while (l_iPolling == 1);

    // It's important to check if updateUrl is not NULL before returning it
    if (l_cUpdateUrl != NULL) {
        return l_cUpdateUrl;
    } else {
        // Handle the case where updateUrl is NULL (no update available)
        return NULL;
    }
}