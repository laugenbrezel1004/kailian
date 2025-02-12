// safe respons in a file...
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct MemoryStruct {
        char *memory;
        size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                                  void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
        return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

int main(void) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
                         "http://localhost:11434/api/generate");
        curl_easy_setopt(
            curl, CURLOPT_POSTFIELDS,
            "{\"model\":\"llama3.2\",\"prompt\":\"Why is the sky blue?\"}");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        } else {
            printf("API response:\n%s\n", chunk.memory);
            // Hier können Sie chunk.memory weiter verarbeiten
        }

        curl_easy_cleanup(curl);
    }

    free(chunk.memory);

    return 0;
}
