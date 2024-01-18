#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <json-glib/json-glib.h>


#define MAX_LINE_LENGTH 100
#define MAX_DATA_LENGTH 50
#define MAX_ENTRIES 2


// Callback function to handle the response from cURL
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *data = (char *)userp;

    // Append the received data to the existing buffer
    memcpy(data, contents, realsize);
    return realsize;
}

int main() 
{
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char dataArray[MAX_ENTRIES][MAX_DATA_LENGTH]; // Array to store extracted data

    do{
    // Open the file
    file = fopen("../init.txt", "r");

    // Check if the file is opened successfully
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int entryCount = 0; // Counter for entries in the array

    // Read the file line by line
    while (fgets(line, sizeof(line), file) != NULL && entryCount < MAX_ENTRIES) {
        char *equalSign = strchr(line, '=');

        if (equalSign != NULL) {
            char *data = strtok(equalSign + 1, " \t\n");
            if (data != NULL) {
                // Allocate memory and copy the data to the array
                strncpy(dataArray[entryCount], data, MAX_DATA_LENGTH - 1);
                dataArray[entryCount][MAX_DATA_LENGTH - 1] = '\0'; // Ensure null-terminated string
            } else {
                // Handle case where there is no data after '='
                dataArray[entryCount][0] = '\0'; // Set the array entry as an empty string
            }

            entryCount++;
        }
    }

    // Initialize cURL
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error initializing cURL.\n");
        return 1;
    }

    // Set the URL for the REST request
    const char *url = "http://localhost:3333/auth/signin";
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the request method to POST
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Set the request headers
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the JSON data to be sent in the request
    // Create a JSON string with the desired structure
    const char *json_template = "{\"signature\":\"%s\",\"password\":\"%s\"}";
    char *json_data = NULL;
    // Calculate the required size for the json_data
    size_t json_data_size = snprintf(NULL, 0, json_template, dataArray[0], dataArray[1]) + 1;

    // Allocate memory for json_data
    json_data = (char *)malloc(json_data_size);

    // Check for allocation failure
    if (!json_data) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // Populate json_data with actual values
    snprintf(json_data, json_data_size, json_template, dataArray[0], dataArray[1]);

    // Set the JSON data to be sent in the request
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

    // Set the callback function to handle the response
    char *response_buffer = (char*)malloc(sizeof(char) * 200);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);

    // Perform the cURL request
    CURLcode res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK) {
        fprintf(stderr, "cURL request failed: %s\n", curl_easy_strerror(res));
        return 1;
    }

    // Ensure the response buffer is null-terminated
    response_buffer[200] = '\0';

    // Parse the HTTP status code
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    
    // Check the HTTP status code and take appropriate actions
    if (http_code == 200) {
        // Successful response
        JsonParser *parser = json_parser_new();
        json_parser_load_from_data(parser, response_buffer, -1, NULL);
        JsonObject *root = json_node_get_object(json_parser_get_root(parser));
        // Accessing the 'access_token' member
        const gchar *access_token = json_object_get_string_member(root, "access_token");
        // Print the result
        printf("Access Token: %s\n", access_token);
        printf("Status Code: %ld\n", http_code);
        g_object_unref(parser);

    } else if (http_code == 400 || http_code == 403) {
        // Error response
        JsonParser *parser = json_parser_new();
        json_parser_load_from_data(parser, response_buffer, -1, NULL);
        JsonObject *root = json_node_get_object(json_parser_get_root(parser));
        const gchar *message = json_object_get_string_member(root, "message");
        const gchar *error = json_object_get_string_member(root, "error");
        printf("Message: %s\n", message);
        printf("Error: %s\n", error);
        printf("Status Code: %ld\n", http_code);
        g_object_unref(parser);
    }
    else
    {
        printf("Status Code: %ld\n", http_code);   
    }
    sleep(5);


    // Clean up
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json_data);
    free(response_buffer);
    // Close the file
    fclose(file);

    }while(1);
    return 0;
}
