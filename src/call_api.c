// call_api.c
#include "../include/call_api.h"
#include "../include/loadEnv.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_ENDPOINT "http://localhost:11434/api/generate"
#define DEFAULT_INFO_ENDPOINT "http://localhost:11434/api/tags"

static const char *getEnvValue(const env *config, size_t count,
                               const char *key) {
    for (size_t i = 0; i < count; i++) {
        if (config[i].key && strcmp(config[i].key, key) == 0) {
            return config[i].value;
        }
    }
    return NULL;
}

static size_t cbSendArgument(void *data, size_t size, size_t nmemb,
                             void *userp) {
    size_t realsize = size * nmemb;
    int *showModels = (int *)userp;
    char *buffer = malloc(realsize + 1);
    if (!buffer)
        return 0;

    memcpy(buffer, data, realsize);
    buffer[realsize] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        fprintf(stderr, "JSON parse error: %s\n", cJSON_GetErrorPtr());
        free(buffer);
        return 1;
    }

    if (*showModels) {
        cJSON *models = cJSON_GetObjectItemCaseSensitive(json, "models");
        if (cJSON_IsArray(models)) {
            int count = cJSON_GetArraySize(models);
            for (int i = 0; i < count; i++) {
                cJSON *model = cJSON_GetArrayItem(models, i);
                cJSON *name = cJSON_GetObjectItemCaseSensitive(model, "name");
                if (cJSON_IsString(name)) {
                    printf("Model Name: %s\n", name->valuestring);
                }
            }
        }
    } else {
        char *json_str = cJSON_Print(json);
        if (json_str) {
            printf("%s\n", json_str);
            free(json_str);
        }
    }

    cJSON_Delete(json);
    free(buffer);
    return realsize;
}

static size_t cbAi(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char *buffer = malloc(realsize + 1);
    if (!buffer)
        return 0;

    memcpy(buffer, data, realsize);
    buffer[realsize] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    if (json) {
        if (strncmp(buffer, "{\"error}", 7) == 0) {
            fprintf(stderr, "%s\n", buffer);
            free(buffer);
            cJSON_Delete(json);
            exit(1);
        }
        cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");
        if (cJSON_IsString(response)) {
            printf("%s", response->valuestring);
            fflush(stdout);
        }
        cJSON_Delete(json);
    }
    free(buffer);
    return realsize;
}

int connectToAi(const char *bufferPrompt, const char *bufferFile,
                const char *argument) {
    size_t env_count;
    env *config = readEnv(NULL, &env_count);
    if (!config)
        return 1;

    const char *name = getEnvValue(config, env_count, "name");
    const char *endpoint =
        getEnvValue(config, env_count, "endpoint_generate") ?: DEFAULT_ENDPOINT;
    const char *info_endpoint = getEnvValue(config, env_count, "endpoint_info")
                                    ?: DEFAULT_INFO_ENDPOINT;
    const char *system = getEnvValue(config, env_count, "system");

    if (!name || !system) {
        fprintf(stderr, "Missing required config values\n");
        freeEnv(config, env_count);
        return 1;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        freeEnv(config, env_count);
        return 1;
    }

    CURLcode res;
    if (argument) {
        const char *url = (strcmp(argument, "--show-models") == 0 ||
                           strcmp(argument, "-s") == 0)
                              ? info_endpoint
                              : info_endpoint; // Vereinfacht für Beispiel
        int showModels = (strcmp(argument, "--show-models") == 0 ||
                          strcmp(argument, "-s") == 0);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cbSendArgument);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &showModels);

        res = curl_easy_perform(curl);
    } else {
        cJSON *root = cJSON_CreateObject();
        if (!root) {
            curl_easy_cleanup(curl);
            freeEnv(config, env_count);
            return 1;
        }

        cJSON_AddStringToObject(root, "model", name);
        cJSON_AddStringToObject(root, "system", system);

        size_t prompt_len = strlen(bufferPrompt) +
                            (bufferFile ? strlen(bufferFile) + 1 : 0) + 1;
        char *full_prompt = malloc(prompt_len);
        if (!full_prompt) {
            cJSON_Delete(root);
            curl_easy_cleanup(curl);
            freeEnv(config, env_count);
            return 1;
        }
        snprintf(full_prompt, prompt_len, bufferFile ? "%s\n%s" : "%s",
                 bufferPrompt, bufferFile ?: "");

        cJSON_AddStringToObject(root, "prompt", full_prompt);
        char *json_str = cJSON_PrintUnformatted(root);

        curl_easy_setopt(curl, CURLOPT_URL, endpoint);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cbAi);

        res = curl_easy_perform(curl);
        printf("\n");

        free(json_str);
        free(full_prompt);
        cJSON_Delete(root);
    }

    if (res != CURLE_OK) {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
    freeEnv(config, env_count);
    return res == CURLE_OK ? 0 : 1;
}
