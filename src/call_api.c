// imports
#include <fcntl.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#define _POSIX_C_SOURCE 200809L // Enable POSIX features (strdup, getline, etc.)
#include "../include/arguments/argumentList.h"
#include "../include/checkArgument.h"
#include "../include/loadEnv.h"
#include <cjson/cJSON.h>
/*#include <cmark.h>*/
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/chat/chat_history.h"

typedef struct {
    char *data;
    size_t size;
} ResponseData;

// prototypes
int sendArgument(const char *buffer);
static size_t cbSendArgument(void *data, size_t size, size_t nmemb,
                             void *userp);
int connectToAi(const char *prompt, const char *file, const char *argument);
static size_t cbAi(void *data, size_t size, size_t nmemb, void *userp);
int connectToAiChat(const char *promptBuffer, const char *fileBuffer);
static size_t cbAiChat(void *data, size_t size, size_t nmemb, void *userp);

// macros
#define MELDUNG(text)                                                          \
    fprintf(stderr, "Datei [%s], Zeile %d: %s\n", __FILE__, __LINE__, text)

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
    if (strcmp(argument, arguments.model.long_form) == 0) {
        url = ENV.running_model_endpoint;
    } else if (strcmp(argument, arguments.showModels.long_form) == 0) {
        url = ENV.info_endpoint;
    } else if (strcmp(argument, arguments.info.long_form) == 0) {
        url = ENV.info_endpoint;
    } else {
        curl_easy_cleanup(curl);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    /*curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,
     * sendArgumentWriteCallback);*/
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cbSendArgument);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n",
                curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? 0 : 1;
}
static size_t cbSendArgument(void *data, size_t size, size_t nmemb,
                             void *userp) {
    size_t realsize = size * nmemb;
    int *showModels = (int *)userp;

    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed\n");
        return 0;
    }
    memcpy(buffer, data, realsize);
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

int connectToAi(const char *prompt, const char *file, const char *argument) {
    const Env ENV = readEnv();
    CURL *curl = NULL;
    CURLcode res = CURLE_OK;
    cJSON *root = cJSON_CreateObject();
    char *json_str = NULL;
    ResponseData response = {NULL, 0};

    if (!root) {
        fprintf(stderr, "cJSON_CreateObject failed\n");
        return 1;
    }

    cJSON_AddStringToObject(root, "model", ENV.name);
    /*cJSON_AddBoolToObject(root, "raw", cJSON_True);*/
    cJSON_AddStringToObject(root, "system", ENV.system);

    // Füge Chat-History zum Prompt hinzu
    char *history_context = chat_history_get_context();
    char *full_prompt = NULL;
    
    if (history_context) {
        size_t total_len = strlen(history_context) + 
                          (prompt ? strlen(prompt) : 0) + 
                          (file ? strlen(file) : 0) + 3;
        
        full_prompt = malloc(total_len);
        if (full_prompt) {
            snprintf(full_prompt, total_len, "%s\n%s%s%s",
                    history_context,
                    prompt ? prompt : "",
                    file ? "\n" : "",
                    file ? file : "");
        }
        free(history_context);
    }

    cJSON_AddStringToObject(root, "prompt", full_prompt);
    free(full_prompt);

    json_str = cJSON_PrintUnformatted(root);
    if (!json_str) {
        fprintf(stderr, "cJSON_PrintUnformatted failed\n");
        cJSON_Delete(root);
        return 1;
    }

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Ollama's napping! -> curl_easy_init failed\n");
        free(json_str);
        cJSON_Delete(root);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL,
                     ENV.endpoint); // Replace with ENV.endpoint
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
    /*curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cbKi);*/
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cbAi);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n",
                curl_easy_strerror(res));
    } /*else {*/
    /*    printf("Raw response: %s\n", chunk.memory ? chunk.memory : "NULL");*/
    /*    cJSON *json = cJSON_Parse(chunk.memory);*/
    /*    if (!json) {*/
    /*        fprintf(stderr, "JSON parse failed: %s\n", cJSON_GetErrorPtr());*/
    /*        printf("%s", chunk.memory);*/
    /*        ;*/
    /*    } else {*/
    /*        cJSON *response =*/
    /*            cJSON_GetObjectItemCaseSensitive(json, "response");*/
    /*        if (response && cJSON_IsString(response)) {*/
    /*            printf("%s", response->valuestring);*/
    /*        } else {*/
    /*            printf("No valid response field in JSON\n");*/
    /*            printf("%s", chunk.memory);*/
    /*        }*/
    /*        cJSON_Delete(json);*/
    /*    }*/
    /*}*/
    printf("\n"); // to remote the "%" after the ki answer
    free(json_str);
    cJSON_Delete(root);
    curl_easy_cleanup(curl);

    // Speichere Prompt und Antwort in der History
    if (prompt && res == CURLE_OK && response.data) {
        chat_history_add(prompt, response.data);
        free(response.data);
    }

    return (res == CURLE_OK) ? 0 : 1;
}
static size_t cbAi(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    ResponseData *response = (ResponseData *)userp;

    // Allocate temp buffer for received data plus nullterminator
    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed in callback\n");
        MELDUNG("error");
        return 0;
    }

    memcpy(buffer, data, realsize);
    buffer[realsize] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    const char *errorKi = "{\"error}";
    if (json) {
        if (strncmp(errorKi, buffer, 6) == 0) {
            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            return 0;
        }

        cJSON *resp = cJSON_GetObjectItemCaseSensitive(json, "response");
        if (resp && cJSON_IsString(resp)) {
            printf("%s", resp->valuestring);
            size_t resp_len = strlen(resp->valuestring);
            response->data = realloc(response->data, response->size + resp_len + 1);
            if (response->data) {
                memcpy(response->data + response->size, resp->valuestring, resp_len);
                response->size += resp_len;
                response->data[response->size] = '\0';
            }
            fflush(stdout);
        }
        cJSON_Delete(json);
    }
    free(buffer);
    return realsize;
}

int connectToAiChat(const char *promptBuffer, const char *fileBuffer) {
    const Env ENV = readEnv();
    CURL *curl = NULL;
    CURLcode res = CURLE_OK;
    cJSON *root = cJSON_CreateObject();
    char *json_str = NULL;

    if (!root) {
        fprintf(stderr, "cJSON_CreateObject failed\n");
        return 1;
    }

    char chatHistory[256];
    uid_t userID = getuid();
    snprintf(chatHistory, sizeof(chatHistory), "/run/user/%u/chatHistory",
             userID);
    int fd_chatHistory = open(chatHistory, O_WRONLY | O_CREAT, 0644);

    if (fd_chatHistory < 0) {
        fprintf(stderr, "Failed to open chatHistory");
        return 1;
    }
    close(fd_chatHistory);

    cJSON_AddStringToObject(root, "model", ENV.name);
    /*cJSON_AddBoolToObject(root, "raw", cJSON_True);*/
    cJSON_AddStringToObject(root, "system", ENV.system);

    // Combine prompt and fileBuffer with a newline separator
    char *full_prompt = NULL;
    size_t prompt_len = strlen(promptBuffer) + 1; // Prompt + null
    if (fileBuffer)
        prompt_len += strlen(fileBuffer) + 1; // Space + fileBuffer
    full_prompt = malloc(prompt_len);
    if (!full_prompt) {
        fprintf(stderr, "malloc failed for full_prompt\n");
        cJSON_Delete(root);
        return 1;
    }
    if (fileBuffer) {
        snprintf(full_prompt, prompt_len, "%s\n%s", promptBuffer, fileBuffer);
    } else {
        strcpy(full_prompt, promptBuffer);
    }

    cJSON_AddStringToObject(root, "prompt", full_prompt);
    free(full_prompt);

    json_str = cJSON_PrintUnformatted(root);
    if (!json_str) {
        fprintf(stderr, "cJSON_PrintUnformatted failed\n");
        cJSON_Delete(root);
        return 1;
    }

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Ollama's napping! -> curl_easy_init failed\n");
        free(json_str);
        cJSON_Delete(root);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL,
                     ENV.endpoint); // Replace with ENV.endpoint
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cbAi);
    /*curl_easy_setopt(curl, CURLOPT_WRITEDATA, &cuc);*/

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform failed: %s\n",
                curl_easy_strerror(res));
    }

    printf("\n"); // to remote the "%" after the ki answer
    free(json_str);
    cJSON_Delete(root);
    close(fd_chatHistory);
    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? 0 : 1;
}
static size_t cbAiChat(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;

    // Allocate temp buffer for received data plus nullterminator
    char *buffer = malloc(realsize + 1);
    if (!buffer) {
        fprintf(stderr, "Malloc failed in callback\n");
        MELDUNG("error");
        return 0; // Tell curl to abort
    }

    memcpy(buffer, data, realsize);
    /*printf("buffer -> %s", buffer);*/
    cJSON *json = cJSON_Parse(buffer);
    const char *errorKi = "{\"error}";
    if (json) {
        if (strncmp(errorKi, buffer, 6) == 0) {
            fprintf(stderr, "%s\n", buffer);
            exit(1);
        }

        cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");
        if (response && cJSON_IsString(response)) {
            // Print the response directly (or store if needed)
            printf("%s", response->valuestring);
            fflush(stdout);
        } else {
            /*fprintf(stderr, "JSON parsing succeeded but no valid 'respons"*/
            /*                "string found\n");*/
            /*MELDUNG("error");*/
            /*printf("%s", buffer); // Fallback to raw buffer*/
            /*fflush(stdout);*/
        }
        cJSON_Delete(json);
    } else {
        // If not JSON, treat as raw text and print
        /*MELDUNG("error");*/
        /*printf("%s", buffer);*/
        fflush(stdout);
    }
    free(buffer);
    return realsize; // Success: processed all bytes
}
