#include "../include/loadEnv.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
        char *memory;
        size_t size;
} MemoryStruct;

// for debbuing curl
/*static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,*/
/*                                  void *userp) {*/
/*    size_t realsize = size * nmemb;*/
/*    MemoryStruct *mem = (MemoryStruct *)userp;*/
/**/
/*    char *ptr = realloc(mem->memory, mem->size + realsize + 1);*/
/*    if (!ptr) {*/
/*        fprintf(stderr, "realloc failed\n");*/
/*        return 0;*/
/*    }*/
/**/
/*    mem->memory = ptr;*/
/*    memcpy(&(mem->memory[mem->size]), contents, realsize);*/
/*    mem->size += realsize;*/
/*    mem->memory[mem->size] = 0;*/
/**/
/*    return realsize;*/
/*}*/

static size_t sendArgumentWriteCallback(void *contents, size_t size,
                                        size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *buffer = malloc(realsize + 1);

    if (buffer == NULL) {
        fprintf(stderr, "Malloc failed\n");
        return 0;
    }

    memcpy(buffer, contents, realsize);
    buffer[realsize] = 0;

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        fprintf(stderr, "Error while parsing to json\n");
        free(buffer);
        return 0;
    }

    cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");
    char *formatted_json = cJSON_Print(json);
    fprintf(stdout, "%s\n", formatted_json);
    /*if (response != NULL &&
     * cJSON_IsString(response)) {*/
    /*    fprintf(stdout, "%s",
     * response->valuestring);*/
    /*} else {*/
    /*    fprintf(stderr, "kailian: error");*/
    /*}*/

    cJSON_Delete(json);
    free(buffer);
    fflush(stdout);
    return realsize;
}

static size_t connectToKiWriteCallback(void *contents, size_t size,
                                       size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *buffer = malloc(realsize + 1);

    if (buffer == NULL) {
        fprintf(stderr, "Malloc failed\n");
        return 0;
    }

    memcpy(buffer, contents, realsize);
    buffer[realsize] = 0;

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        fprintf(stderr, "Error while parsing to json\n");
        free(buffer);
        return 0;
    }

    cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");

    if (response != NULL && cJSON_IsString(response)) {
        fprintf(stdout, "%s", response->valuestring);
    } else {
        fprintf(stderr, "kailian: error");
    }

    cJSON_Delete(json);
    free(buffer);
    fflush(stdout);
    return realsize;
}

int connectToKi(char *buffer) {
    const Env ENV = readEnv();

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        MemoryStruct chunk;
        chunk.memory = malloc(1);
        chunk.size = 0;

        curl_easy_setopt(curl, CURLOPT_URL, ENV.endpoint);

        char postfields[1024];
        snprintf(postfields, sizeof(postfields),
                 "{\"model\":\"%s\",\"prompt\":\"%s\"}", ENV.name, buffer);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);

        // Use WriteMemoryCallback to store raw data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, connectToKiWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        } else {
            printf("Raw response stored in memory: %s\n", chunk.memory);

            // Now process the stored data with WriteCallback
            connectToKiWriteCallback(chunk.memory, 1, chunk.size, NULL);
        }

        free(chunk.memory);
        curl_easy_cleanup(curl);
    }

    return 0;
}
int sendArgument(const char *argument) {

    const Env ENV = readEnv();

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        if (strcmp(argument, "--info") == 0) {
            curl_easy_setopt(curl, CURLOPT_URL, ENV.info_endpoint);
        }
        if (strcmp(argument, "--model") == 0) {
            /*curl_easy_setopt(curl, CURLOPT_URL, ENV.model_endpoint);*/
            curl_easy_setopt(curl, CURLOPT_URL, "http://10.0.0.7:11434/api/ps");
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                         sendArgumentWriteCallback);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        }
    }

    curl_easy_cleanup(curl);

    return EXIT_SUCCESS;
}
