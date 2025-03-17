#define _POSIX_C_SOURCE 200809L
#include "../include/call_api.h"
#include "../include/environmentSettings.h"
#include "../include/loadEnv.h"
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    MODE_DEFAULT = 0,
    MODE_SHOW_MODELS = 1,
    MODE_CURRENT_MODEL = 2,
    MODE_INFO = 3
} ApiMode;

static const char *getEnvValue(const env *config, size_t count,
                               const char *key) {
    for (size_t i = 0; i < count; i++) {
        if (config[i].key && !strcmp(config[i].key, key)) {
            return config[i].value;
        }
    }
    return NULL;
}

static size_t writeCallback(void *data, size_t size, size_t nmemb,
                            void *userp) {
    size_t realsize = size * nmemb;
    ApiMode *mode = (ApiMode *)userp;
    char *buffer = malloc(realsize + 1);
    if (!buffer)
        return 0;

    memcpy(buffer, data, realsize);
    buffer[realsize] = '\0';

    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        fprintf(stderr, "kailian: JSON parse error: %s\n", cJSON_GetErrorPtr());
        free(buffer);
        return 0;
    }

    switch (*mode) {
    case MODE_SHOW_MODELS: {
        cJSON *models = cJSON_GetObjectItemCaseSensitive(json, "models");
        if (cJSON_IsArray(models)) {
            int count = cJSON_GetArraySize(models);
            for (int i = 0; i < count; i++) {
                cJSON *model = cJSON_GetArrayItem(models, i);
                cJSON *name = cJSON_GetObjectItemCaseSensitive(model, "name");
                if (cJSON_IsString(name)) {
                    printf("Model: %s\n", name->valuestring);
                }
            }
        }
    } break;
    case MODE_DEFAULT: {
        /*cJSON *json_str = cJSON_GetObjectItemCaseSensitive(json,
         * "response");*/
        printf("%s\n", buffer);
        /*free(json_str);*/
    } break;

    default:
        if (strncmp(buffer, "{\"error}", 7) == 0) {
            fprintf(stderr, "kailian: API error: %s\n", buffer);
        } else {
            cJSON *response =
                cJSON_GetObjectItemCaseSensitive(json, "response");
            if (cJSON_IsString(response)) {
                printf("%s", response->valuestring);
                fflush(stdout);
            }
        }
        break;
    }

    cJSON_Delete(json);
    free(buffer);
    return realsize;
}
int connectToAi(const char *prompt, const char *file, const char *argument) {
    size_t env_count;
    env *config = readEnv(NULL, &env_count);
    if (!config)
        return 1;

    const char *values[8];
    for (int i = 0; i < 8; i++) {
        values[i] = getEnvValue(config, env_count, keys[i]);
        if (!values[i]) {
            fprintf(stderr, "kailian: Missing config value: %s\n", keys[i]);
            freeEnv(config, env_count);
            return 1;
        }
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        freeEnv(config, env_count);
        return 1;
    }

    CURLcode res;
    char *url = NULL;
    ApiMode mode = MODE_DEFAULT; // endpoint_generate

    if (argument) {
        if (!strcmp(argument, "--show-models")) {
            mode = MODE_SHOW_MODELS;
            url = strdup(values[3]); // endpoint_info
        } else if (!strcmp(argument, "--current-model")) {
            mode = MODE_CURRENT_MODEL;
            url = strdup(values[2]); // endpoint_running_model
        } else if (!strcmp(argument, "--info")) {
            mode = MODE_INFO;
            url = strdup(values[3]); // endpoint_info
        }
        if (!url) {
            perror("kailian: strdup failed");
            freeEnv(config, env_count);
            curl_easy_cleanup(curl);
            return 1;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
    } else if (prompt) { // Nur wenn ein Prompt vorhanden ist
        cJSON *root = cJSON_CreateObject();
        if (!root) {
            curl_easy_cleanup(curl);
            freeEnv(config, env_count);
            return 1;
        }

        cJSON_AddStringToObject(root, "model", values[0]);
        cJSON_AddStringToObject(root, "system", values[7]);
        size_t prompt_len = strlen(prompt) + (file ? strlen(file) + 1 : 0) + 1;
        char *full_prompt = malloc(prompt_len);
        if (!full_prompt) {
            cJSON_Delete(root);
            curl_easy_cleanup(curl);
            freeEnv(config, env_count);
            return 1;
        }
        snprintf(full_prompt, prompt_len, file ? "%s\n%s" : "%s", prompt,
                 file ?: "");

        cJSON_AddStringToObject(root, "prompt", full_prompt);
        char *json_str = cJSON_PrintUnformatted(root);

        curl_easy_setopt(curl, CURLOPT_URL, values[1]); // endpoint_generate
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_str);

        printf("json -> %s\n", json_str);
        free(json_str);
        free(full_prompt);
        cJSON_Delete(root);
    } else {
        fprintf(stderr, "kailian: No prompt provided\n");
        curl_easy_cleanup(curl);
        freeEnv(config, env_count);
        return 1;
    }
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mode);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "kailian: CURL error: %s\n", curl_easy_strerror(res));
    }
    if (mode == MODE_DEFAULT) {
        printf("\n");
    }

    free(url);
    curl_easy_cleanup(curl);
    freeEnv(config, env_count);
    return res == CURLE_OK ? 0 : 1;
}
