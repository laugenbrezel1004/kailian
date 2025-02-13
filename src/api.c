
/*#include "../include/cJSON.h"*/
#include "../include/loadEnv.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp);

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp);
int connectToKi(char *buffer);

static char buffer[100];

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
    size_t realsize = size * nmemb;
    typedef struct {
            char *memory;
            size_t size;
    } MemoryStruct;

    MemoryStruct *mem;
    mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        perror("realloc failed");
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {

    size_t realsize = size * nmemb;
    char *buffer = malloc(realsize + 1);

    if (buffer == NULL) {
        fprintf(stderr, "Malloc failed\n");
        return 0;
    }

    memcpy(buffer, contents, realsize);
    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        perror("Error while parsing to json");
        return EXIT_FAILURE;
    }
    cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");

    if (response != NULL && cJSON_IsString(response)) {
        printf("%s", response->valuestring);
    } else {
        perror("Response is not a valid string\n");
        return EXIT_FAILURE;
    }

    free(buffer); // Speicher freigeben
    fflush(stdout);
    return realsize;
}

int connectToKi(char *buffer) {
    Env env = readEnv(); // Initialize with empty strings
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, env.endpoint);

        char postfields[1024]; // Stellen Sie sicher, dass dies groß genug
        /*char *postfields = malloc(sizeof(buffer) + 100);*/
        snprintf(postfields, sizeof(postfields),
                 "{\"model\":\"%s\",\"prompt\":\"%s\"}", env.name, buffer);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    return 0;
}
