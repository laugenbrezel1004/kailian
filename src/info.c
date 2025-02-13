#include "../include/loadEnv.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/*typedef struct {*/
/*        char *memory;*/
/*        size_t size;*/
/*} MemoryStruct;*/

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
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

    /*if (response != NULL &&
     * cJSON_IsString(response)) {*/
    /*    fprintf(stdout, "%s",
     * response->valuestring);*/
    /*} else {*/
    /*    fprintf(stderr, "kailian: error");*/
    /*}*/
    fprintf(stdout, "%s\n", buffer);

    cJSON_Delete(json);
    free(buffer);
    fflush(stdout);
    return realsize;
}

int get_info(char *buffer) {
    const Env ENV = readEnv();

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        /*MemoryStruct chunk;*/
        /*chunk.memory = malloc(1);*/
        /*chunk.size = 0;*/

        curl_easy_setopt(curl, CURLOPT_URL, ENV.info_endpoint);
        // Use WriteMemoryCallback to store raw data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        /*curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);*/

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        } /* else {*/
        /*    printf("Raw response stored in memory: "*/
        /*           "%s\n",*/
        /*           chunk.memory);*/
        /**/
        // Now process the stored data with
        // WriteCallback
        /*WriteCallback(chunk.memory, 1, chunk.size, NULL);*/
    }

    /*free(chunk.memory);*/
    curl_easy_cleanup(curl);

    return 0;
}
