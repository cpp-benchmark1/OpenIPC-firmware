#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "cwe918_example2.h"

int make_proxy_request(const char *proxy_url) {
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    if (!curl) {
        return -1;
    }
    
    // Set target URL (could be internal service)
    curl_easy_setopt(curl, CURLOPT_URL, "http://internal-service.local");
    
    curl_easy_setopt(curl, CURLOPT_PROXY, proxy_url);
    
    // SINK: SSRF vulnerability - user input used to configure proxy   
    res = curl_easy_perform(curl);
    
    curl_easy_cleanup(curl);
    return (res == CURLE_OK) ? 0 : -1;
} 
