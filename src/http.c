#include <curl/curl.h>

// Struct to hold response data
struct ResponseData {
    char *data;
    size_t size;
};

// Callback function to handle the response data
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct ResponseData *response = (struct ResponseData *)userp;

    // Allocate memory for the new data
    response->data = realloc(response->data, response->size + realsize + 1);
    
    if (response->data == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 0;
    }

    // Copy the new data to the buffer
    memcpy(&(response->data[response->size]), contents, realsize);
    response->size += realsize;
    response->data[response->size] = 0; // Null-terminate the string

    return realsize;
}

int testGetHttp() {
    // Initialize libcurl
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing libcurl\n");
    }

    int ret = 0;

    // Set the URL of the API endpoint
    const char *url = "http://10.0.2.2:3000/slot-status";
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the callback function to handle the response
    struct ResponseData response_data = { NULL, 0 };
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    // Make the HTTP GET request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        // Print the response body
        printf("Response Body: %s\n", response_data.data);
    }
    ret = response_data.data == 0 ? 0 : 1;

    // Cleanup
    curl_easy_cleanup(curl);
    free(response_data.data);

    return ret;
}

// post method to post string data
int testPostHttp(const char *data) {
    // Initialize libcurl
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing libcurl\n");
    }

    int ret = 0;

    // Set the URL of the API endpoint
    const char *url = "http://10.0.2.2:3000/slot-status";
    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    // Set the POST data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

    // Set the callback function to handle the response
    struct ResponseData response_data = { NULL, 0 };
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    // Make the HTTP POST request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        ret = 1;
    } else {
        // Print the response body
        printf("Response Body: %s\n", response_data.data);
        ret = 0;
    }

    // Cleanup
    curl_easy_cleanup(curl);
    free(response_data.data);

    return ret;
}
