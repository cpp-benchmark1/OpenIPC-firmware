#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "camera_http_client.h"

int make_direct_request(const char *url) {
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if (!curl) {
        return -1;
    }
    
    // SINK: SSRF vulnerability - user input used to configure URL
    curl_easy_setopt(curl, CURLOPT_URL, url);
    
    res = curl_easy_perform(curl);
    
    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? 0 : -1;
} 