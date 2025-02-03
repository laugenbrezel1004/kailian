
#include "api.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>

int main(void) {
    CURL *curl;
    CURLcode res;
    const char *json_payload =
        "{\"model\":\"deepseek-r1\",\"prompt\":\"Why is the sky blue?\"}";

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL,
                         "http://localhost:11434/api/generate");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_payload);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return 0;
}
