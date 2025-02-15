#include "../include/arguments/argumentList.h"
#include "../include/checkArgument.h"
#include "../include/loadEnv.h"
#include "../include/loggerInterface.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
        char *memory;
        size_t size;
} MemoryStruct;
int i = 0;

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
    Log logger = initLogger();
    char *buffer = malloc(realsize + 1);

    if (buffer == NULL) {
        logger.debugConsole("Malloc failed\n");
        return 0;
    }

    memcpy(buffer, contents, realsize);
    buffer[realsize] = 0;

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        logger.debugConsole("Error while parsing to json\n");
        free(buffer);
        return 0;
    }

    if (i) { // Use the global i variable
        cJSON *models = cJSON_GetObjectItemCaseSensitive(json, "models");
        if (cJSON_IsArray(models)) {
            int modelCount = cJSON_GetArraySize(models);
            for (int j = 0; j < modelCount; j++) {
                cJSON *model = cJSON_GetArrayItem(models, j);
                cJSON *name = cJSON_GetObjectItemCaseSensitive(model, "name");
                if (cJSON_IsString(name) && (name->valuestring != NULL)) {
                    fprintf(stdout, "Model Name: %s\n", name->valuestring);
                }
            }
        } else {
            logger.debugConsole("No models array found or it's not an array\n");
        }
    } else {
        char *jsonString = cJSON_Print(json);
        if (jsonString) {
            fprintf(stdout, "%s\n", jsonString); // print out info
            free(jsonString);
        } else {
            logger.debugConsole("Failed to print JSON\n");
        }
    }

    cJSON_Delete(json);
    free(buffer);
    fflush(stdout);
    return realsize;
}
static size_t connectToKiWriteCallback(void *contents, size_t size,
                                       size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    Log logger = initLogger();
    char *buffer = malloc(realsize + 1);

    if (buffer == NULL) {
        logger.debugConsole("Malloc failed\n");
        return 0;
    }

    memcpy(buffer, contents, realsize);
    buffer[realsize] = 0;

    cJSON *json = cJSON_Parse(buffer);
    if (json == NULL) {
        logger.debugConsole("Error while parsing to json\n");
        free(buffer);
        return 0;
    }

    cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");

    if (response != NULL && cJSON_IsString(response)) {
        fprintf(stdout, "%s", response->valuestring);
    } else {
        logger.debugConsole("kailian: error");
    }

    cJSON_Delete(json);
    free(buffer);
    fflush(stdout);
    return realsize;
}

int connectToKi(char *buffer) {
    Log logger = initLogger();
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
            logger.debugConsole("curl_easy_perform() has failed\n");
            fprintf(stderr, "%s\n", curl_easy_strerror(res));
        } else {
            connectToKiWriteCallback(chunk.memory, 1, chunk.size, NULL);
        }

        free(chunk.memory);
        curl_easy_cleanup(curl);
    }

    return 0;
}

int sendArgument(const char *argument) {
    const Env ENV = readEnv();
    Log logger = initLogger();
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        const char *url = NULL;
        if (strcmp(argument, argument_model.long_form) == 0) {
            url = ENV.running_model_endpoint;
        }
        if (strcmp(argument, argument_showModels.long_form) == 0) {
            i = 1; // for sendArgumentWriteCallback to know that i want only the
                   // model and not everything
            url = ENV.info_endpoint;
        }
        if (strcmp(argument, argument_info.long_form) == 0) {
            url = ENV.info_endpoint;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
                         sendArgumentWriteCallback);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            logger.debugConsole("curl_easy_perform() has failed\n");
            fprintf(stderr, "%s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
    exit(0);
}
