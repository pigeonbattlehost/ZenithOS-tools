// CLZV - Check Last ZenithOS Version V1
// MADE BY: zntsproj 
// 2024-2025 LICGX


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>

#define BLUE "\033[1;34m"
#define GREEN "\033[1;32m"
#define RED  "\033[1;31m"
#define RESET "\033[0m"

#define OTA_URL "https://pigeonbattlehost.github.io/Z-OTA/version.json"
#define TIMEOUT 60L

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr) return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

char *extract_version(const char *json) {
    const char *key = "\"latest_version\"";
    char *pos = strstr(json, key);
    if(!pos) return NULL;

    pos += strlen(key);
    while(*pos && (*pos != '\"')) pos++;
    if(*pos != '\"') return NULL;
    pos++;
    char *end = strchr(pos, '\"');
    if(!end) return NULL;

    size_t len = end - pos;
    char *version = malloc(len + 1);
    strncpy(version, pos, len);
    version[len] = 0;

    return version;
}

int main() {
    printf(BLUE "clzv V1\n" RESET);
    printf("Getting information from ZenithOS OTA database...\n");

    clock_t start_time = clock();

    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(!curl) {
        printf(RED "FAIL!\n" RESET);
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, OTA_URL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, TIMEOUT);

    res = curl_easy_perform(curl);

    clock_t end_time = clock();
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    if(res != CURLE_OK) {
        printf(RED "FAIL!\n\n" RESET);
        printf("Response from the server:\n%s\n\n", curl_easy_strerror(res));
        printf("Failed in %.2f s\n", elapsed);
    } else {
        char *version = extract_version(chunk.memory);
        if(version) {
            printf(GREEN "OK!\n" RESET);
            printf("The latest version is: %s\n", version);
            printf("Done in %.2f s\n", elapsed);
            free(version);
        } else {
            printf(RED "FAIL!\n\n" RESET);
            printf("Response from the server:\n%s\n\n", chunk.memory);
            printf("Failed in %.2f s\n", elapsed);
        }
    }

    curl_easy_cleanup(curl);
    free(chunk.memory);
    curl_global_cleanup();

    return 0;
}

