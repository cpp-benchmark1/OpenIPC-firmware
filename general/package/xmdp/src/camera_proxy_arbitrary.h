#ifndef CAMERA_PROXY_ARBITRARY_H
#define CAMERA_PROXY_ARBITRARY_H

#include <curl/curl.h>

// Function to make HTTP request with arbitrary proxy
int make_proxy_arbitrary_request(const char *proxy_url);

#endif // CAMERA_PROXY_ARBITRARY_H 