#define _POSIX_C_SOURCE 200809L // Enable POSIX features (strdup, getline, etc.)
#include "../include/arguments/argumentList.h"
#include "../include/checkArgument.h"
#include "../include/loadEnv.h"
#include <cjson/cJSON.h>
#include <cmark.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

typedef struct {
        char *memory;
        size_t size;
} MemoryStruct;

// Callback for sendArgument: handles model listing or full info
static size_t sendArgumentWriteCallback(void *contents, size_t size,
                                        size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    int *showModels = (int *)userp;

    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed\n");
        return 0;
    }
    memcpy(buffer, contents, realsize);
    buffer[realsize] = 0;

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        fprintf(stderr, "Error parsing JSON: %s\n", cJSON_GetErrorPtr());
        MELDUNG("JSON parsing failed");
        return 1;
    }

    if (*showModels) { // Show only model names
        cJSON *models = cJSON_GetObjectItemCaseSensitive(json, "models");
        if (cJSON_IsArray(models)) {
            int modelCount = cJSON_GetArraySize(models);
            for (int j = 0; j < modelCount; j++) {
                cJSON *model = cJSON_GetArrayItem(models, j);
                cJSON *name = cJSON_GetObjectItemCaseSensitive(model, "name");
                if (cJSON_IsString(name) && name->valuestring) {
                    fprintf(stdout, "Model Name: %s\n", name->valuestring);
                }
            }
        } else {
            fprintf(stderr, "No models array found\n");
        }
    } else { // Print full JSON
        char *jsonString = cJSON_Print(json);
        if (jsonString) {
            fprintf(stdout, "%s\n", jsonString);
            free(jsonString);
        } else {
            fprintf(stderr, "Failed to print JSON\n");
            MELDUNG("Error");
        }
    }

    cJSON_Delete(json);
    fflush(stdout);
    return realsize;
}

// Callback for connectToKi: extracts "response" from Ollama
// Callback for curl: Extracts "response" from JSON or handles raw text
// MemoryStruct for accumulating response
/*typedef struct {*/
/*    char *memory;*/
/*    size_t size;*/
/*} MemoryStruct;*/

// Callback to append data to MemoryStruct
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        fprintf(stderr, "Realloc failed in callback\n");
        return 0;
    }
    mem->memory = ptr;
    memcpy(mem->memory + mem->size, contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = '\0';

    return realsize;
}
static size_t connectToKiWriteCallback(void *contents, size_t size,
                                       size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    /*MemoryStruct *mem = (MemoryStruct *)userp;*/

    // Allocate temp buffer for received data
    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed in callback\n");
        MELDUNG("error");
        return 0; // Tell curl to abort
    }
    memcpy(buffer, contents, realsize);
    cJSON *json = cJSON_Parse(buffer);
    if (json) {
        cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");
        if (response && cJSON_IsString(response)) {
            // Print the response directly (or store if needed)
            printf("%s", response->valuestring);
            fflush(stdout);
        } else {
            fprintf(stderr, "JSON parsing succeeded but no valid 'respons"
                            "string found\n");
            MELDUNG("error");
            printf("%s", buffer); // Fallback to raw buffer
            fflush(stdout);
        }
        cJSON_Delete(json);
    } else {
        // If not JSON, treat as raw text and print
        printf("%s", buffer);
        fflush(stdout);
    }
    free(buffer);
    return realsize; // Success: processed all bytes
}

int connectToKi(const char *promptBuffer, const char *fileBuffer) {
    CURL *curl = NULL;
    const Env ENV = readEnv();
    CURLcode res = CURLE_OK;
    cJSON *root = cJSON_CreateObject();
    char *json_str = NULL;
    MemoryStruct chunk = {.memory = NULL, .size = 0};

    if (!root) {
        fprintf(stderr, "cJSON_CreateObject failed\n");
        return 1;
    }

    cJSON_AddStringToObject(root, "model", ENV.name);

    char *full_prompt = NULL;
    if (fileBuffer) {
        size_t prompt_len =
            strlen(promptBuffer) + strlen(fileBuffer) + 2; // Space + null
        full_prompt = malloc(prompt_len);
        if (!full_prompt) {
            fprintf(stderr, "malloc failed for full_prompt\n");
            cJSON_Delete(root);
            return 1;
        }
        snprintf(full_prompt, prompt_len, "%s %s", promptBuffer, fileBuffer);
    } else {
        full_prompt = strdup(promptBuffer);
        if (!full_prompt) {
            fprintf(stderr, "strdup failed for promptBuffer\n");
            cJSON_Delete(root);
            return 1;
        }
    }

    cJSON_AddStringToObject(root, "prompt", full_prompt);
    free(full_prompt);

    json_str = cJSON_PrintUnformatted(root);
    if (!json_str) {
        fprintf(stderr, "cJSON_PrintUnformatted failed\n");
        cJSON_Delete(root);
        return 1;
    }

    printf("Sending JSON: %s\n", json_str);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init failed\n");
        free(json_str);
        cJSON_Delete(root);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL,
                     ENV.endpoint); // Replace with ENV.endpoint
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, connectToKiWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n",
                curl_easy_strerror(res));
    } else {
        printf("Raw response: %s\n", chunk.memory ? chunk.memory : "NULL");
        cJSON *json = cJSON_Parse(chunk.memory);
        if (!json) {
            fprintf(stderr, "JSON parse failed: %s\n", cJSON_GetErrorPtr());
            printf("%s", chunk.memory);
        } else {
            cJSON *response =
                cJSON_GetObjectItemCaseSensitive(json, "response");
            if (response && cJSON_IsString(response)) {
                printf("%s", response->valuestring);
            } else {
                printf("No valid response field in JSON\n");
                printf("%s", chunk.memory);
            }
            cJSON_Delete(json);
        }
    }

    free(chunk.memory);
    free(json_str);
    cJSON_Delete(root);
    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? 0 : 1;
}
int sendArgument(const char *argument) {
    const Env ENV = readEnv();
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init failed\n");
        return 1;
    }

    const char *url = NULL;
    int showModels = 0;
    if (strcmp(argument, argument_model.long_form) == 0) {
        url = ENV.running_model_endpoint;
    } else if (strcmp(argument, argument_showModels.long_form) == 0) {
        showModels = 1;
        url = ENV.info_endpoint;
    } else if (strcmp(argument, argument_info.long_form) == 0) {
        url = ENV.info_endpoint;
    } else {
        fprintf(stderr, "Unknown argument: %s\n", argument);
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, sendArgumentWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &showModels);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n",
                curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? 0 : 1;
}
