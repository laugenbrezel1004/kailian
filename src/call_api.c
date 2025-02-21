#include "../include/arguments/argumentList.h"
#include "../include/checkArgument.h"
#include "../include/loadEnv.h"
#include <cjson/cJSON.h>
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

// General-purpose callback to store response
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        fprintf(stderr, "realloc failed\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

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
        free(buffer);
        return 0;
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
    free(buffer);
    fflush(stdout);
    return realsize;
}

// Callback for connectToKi: extracts "response" from Ollama
static size_t connectToKiWriteCallback(void *contents, size_t size,
                                       size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed\n");
        return 0;
    }
    memcpy(buffer, contents, realsize);
    buffer[realsize] = 0;

    cJSON *json = cJSON_Parse(buffer);
    if (json) {
        cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");
        if (response && cJSON_IsString(response)) {
            size_t len = strlen(response->valuestring);
            char *ptr = realloc(mem->memory, mem->size + len + 1);
            if (!ptr) {
                fprintf(stderr, "realloc failed\n");
                cJSON_Delete(json);
                free(buffer);
                return 0;
            }
            mem->memory = ptr;
            memcpy(&(mem->memory[mem->size]), response->valuestring, len);
            mem->size += len;
            mem->memory[mem->size] = 0;
        }
        cJSON_Delete(json);
    } else {
        // If parsing fails, treat as raw text (Ollama might not always send
        // JSON)
        char *ptr = realloc(mem->memory, mem->size + realsize + 1);
        if (!ptr) {
            fprintf(stderr, "realloc failed\n");
            free(buffer);
            return 0;
        }
        mem->memory = ptr;
        memcpy(&(mem->memory[mem->size]), buffer, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }

    free(buffer);
    fflush(stdout);
    return realsize;
}

int connectToKi(char *buffer) {
    const Env ENV = readEnv();
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init failed\n");
        return 1;
    }

    // Build prompt
    const char *question = "What is the purpose of this command?";
    size_t prompt_len = strlen(buffer) + strlen(question) + 32;
    char *prompt = malloc(prompt_len);
    if (!prompt) {
        fprintf(stderr, "malloc failed for prompt\n");
        curl_easy_cleanup(curl);
        return 1;
    }
    snprintf(prompt, prompt_len, "Here is the content: %s\n\nQuestion: %s",
             buffer, question);

    // Create JSON payload
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "model", ENV.name);
    cJSON_AddStringToObject(root, "prompt", prompt);
    char *json_str = cJSON_PrintUnformatted(root);
    if (!json_str) {
        fprintf(stderr, "cJSON_PrintUnformatted failed\n");
        free(prompt);
        cJSON_Delete(root);
        curl_easy_cleanup(curl);
        return 1;
    }

    // Set up curl
    MemoryStruct chunk = {.memory = malloc(1), .size = 0};
    if (!chunk.memory) {
        fprintf(stderr, "malloc failed for chunk\n");
        free(prompt);
        free(json_str);
        cJSON_Delete(root);
        curl_easy_cleanup(curl);
        return 1;
    }
    chunk.memory[0] = 0;

    curl_easy_setopt(curl, CURLOPT_URL, ENV.endpoint);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, connectToKiWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

    // Perform request
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n",
                curl_easy_strerror(res));
    } else {
        fprintf(stdout, "%s\n", chunk.memory); // Print the accumulated response
    }

    // Cleanup
    free(chunk.memory);
    free(prompt);
    free(json_str);
    cJSON_Delete(root);
    curl_slist_free_all(headers);
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
