
/*#include "../include/cJSON.h"*/
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

char buffer[100];

static size_t WriteCallback(void *contents, size_t size, size_t nmemb) {
    // void *userp
    size_t realsize = size * nmemb;

    // Temporären Puffer erstellen und nullterminieren
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
    /*jSONbuffer[realsize] = '\0'; // Nullterminierung*/

    // String in Tokens zerlegen
    /*const char delimiter[] = ",";*/
    /*const char delimiter2[] = "\"";*/
    /*char *token = strtok(jSONbuffer, delimiter);*/
    /**/
    /*while (token != NULL) {*/
    /*    if (strstr(token, "response") != NULL) {*/
    /*        printf("%s", strtok(token, delimiter2));*/
    /*    }*/
    /*    printf("%s\n", token);*/
    /*    token = strtok(NULL, delimiter);*/
    /*}*/
    /**/
    cJSON *response = cJSON_GetObjectItemCaseSensitive(json, "response");
    printf("%s", response->valuestring);

    free(buffer); // Speicher freigeben
    fflush(stdout);
    return realsize;
}

int connectToKi(char *buffer) {
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
                         "http://localhost:11434/api/generate");

        char postfields[1024]; // Stellen Sie sicher, dass dies groß genug
        /*char *postfields = malloc(sizeof(buffer) + 100);*/
        snprintf(postfields, sizeof(postfields),
                 "{\"model\":\"deepseek-r1:latest\",\"prompt\":\"%s\"}",
                 buffer);
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
