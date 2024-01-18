#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <curl/curl.h>
#include <json-glib/json-glib.h>

#define MAX_LINE_LENGTH 200
#define MAX_DATA_LENGTH 200
#define MAX_ENTRIES 2
#define SIZE 250

// Callback function to handle the response from cURL
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *data = (char *)userp;

    // Append the received data to the existing buffer
    memcpy(data, contents, realsize);
    return realsize;
}

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char dataArray[MAX_ENTRIES][MAX_DATA_LENGTH]; // Array to store extracted data

    do {
        // Open the file
        file = fopen("../initProg.txt", "r");

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
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if (!curl) {
            fprintf(stderr, "Error initializing cURL.\n");
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:3333/device/progress");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "http");
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        // Build the Authorization header
        char authorizationHeader[SIZE];  // Adjust the size as needed
        snprintf(authorizationHeader, sizeof(authorizationHeader), "Authorization: Bearer %s", dataArray[0]);

        // Set the Authorization header
        headers = curl_slist_append(headers, authorizationHeader);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the request data
        const char *json_template = "{\n    \"progress\":\"%s\"\n}";
        char *json_data = NULL;
        // Calculate the required size for the json_data
        size_t json_data_size = snprintf(NULL, 0, json_template, dataArray[1]) + 1;

        // Allocate memory for json_data
        json_data = (char *)malloc(json_data_size);

        // Check for allocation failure
        if (!json_data) {
            fprintf(stderr, "Memory allocation failed.\n");
            return 1;
        }

        // Populate json_data with actual values
        snprintf(json_data, json_data_size, json_template, dataArray[1]);

        // Set the JSON data to be sent in the request
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        // Set the callback function to handle the response
        char *response_buffer = (char *)malloc(sizeof(char) * SIZE);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_buffer);

        // Perform the cURL request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "cURL request failed: %s\n", curl_easy_strerror(res));
            return 1;
        }

        // Ensure the response buffer is null-terminated
        response_buffer[SIZE] = '\0';  // Adjust the index as needed

        // Parse the HTTP status code
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        // Check the HTTP status code and take appropriate actions
        if (http_code == 200) {
            // Print the result
            printf("Update Status: %s\n", dataArray[1]);
            printf("Status Code: %ld\n", http_code);
        } else if (http_code == 401) {
            // Error response
            JsonParser *parser = json_parser_new();
            json_parser_load_from_data(parser, response_buffer, -1, NULL);
            JsonObject *root = json_node_get_object(json_parser_get_root(parser));
            const gchar *message = json_object_get_string_member(root, "message");
            printf("Message: %s\n", message);
            printf("Status Code: %ld\n", http_code);
            g_object_unref(parser);
        }
        else
        {
            printf("Status Code: %ld\n", http_code);
        }
        //http_code = 403 is not treated
        sleep(5);

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        free(json_data);
        free(response_buffer);
        // Close the file
        fclose(file);
    } while (1);

    return 0;
}