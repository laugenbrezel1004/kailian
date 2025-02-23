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

// General-purpose callback to store response
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
/*    char *markdown = mem->memory;*/
/*    char *html = cmark_markdown_to_html(markdown, strlen(markdown), 0);*/
/*    printf("html -> \n%s", html);*/
/*    char *ptrhtml = html;*/
/*    while (*ptrhtml) {*/
/*        if (*ptrhtml == '<') {*/
/*            while (*ptrhtml && *ptr != '>')*/
/*                ptrhtml++; // Skip tags*/
/*            ptrhtml++;*/
/*        } else {*/
/*            putchar(*ptrhtml++);*/
/*        }*/
/*    }*/
/**/
/*    free(html); // Free memory allocated by cmark*/
/**/
/*    return realsize;*/
/*}*/

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
static size_t connectToKiWriteCallback(void *contents, size_t size,
                                       size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;

    // Allocate temp buffer for received data
    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed in callback\n");
        MELDUNG("error");
        return 0; // Tell curl to abort
    }
    memcpy(buffer, contents, realsize);
    buffer[realsize] = '\0';

    // Try parsing as JSON
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
    const Env ENV = readEnv();
    CURL *curl = NULL;
    CURLcode res = CURLE_OK;
    cJSON *root = NULL;
    char *json_str = NULL;
    char *postfield = NULL;
    MemoryStruct chunk = {.memory = NULL, .size = 0};

    // Initialize CURL
    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "curl_easy_init failed\n");
        MELDUNG("error");
        return 1;
    }

    // Allocate postfield with precise size
    size_t postfield_size = strlen(ENV.name) + strlen(promptBuffer) +
                            128; // Extra for JSON structure
    postfield = malloc(postfield_size);
    if (!postfield) {
        fprintf(stderr, "malloc failed for postfield\n");
        MELDUNG("error");
        goto cleanup;
    }

    // evtl noch gucken ob fileBuffer valid json ist
    printf("FileBuffer -> %s\n", fileBuffer);
    if (fileBuffer != NULL) {
        snprintf(postfield, postfield_size,
                 "{\"model\":\"%s\",\"prompt\":\"%s %s\"}", ENV.name,
                 promptBuffer, fileBuffer);
    } else {

        snprintf(postfield, postfield_size,
                 "{\"model\":\"%s\",\"prompt\":\"%s\"}", ENV.name,
                 promptBuffer);
    }
    printf("Hole Prompt -> %s\n", postfield);
    /*printf("%s\n", postfield);*/
    // Configure CURL options
    curl_easy_setopt(curl, CURLOPT_URL, ENV.endpoint);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfield);
    curl_easy_setopt(
        curl, CURLOPT_WRITEFUNCTION,
        connectToKiWriteCallback); // which function to call whenever it
    // receives data from the server
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,
                     &chunk); // Pass MemoryStruct to callback

    // Perform request
    res = curl_easy_perform(curl);
    printf("\n");
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n",
                curl_easy_strerror(res));
        MELDUNG("error");
        goto cleanup;
    }

    // Success: Response already printed by callback
    // Optionally, you could access chunk.memory here if storing instead of
    // printing

cleanup:
    free(chunk.memory); // Free if allocated (though not used in this version)
    free(postfield);
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
