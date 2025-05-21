#ifndef CAMERA_PROXY_CLIENT_H
#define CAMERA_PROXY_CLIENT_H

#include <curl/curl.h>

// Function to make HTTP request using user input as proxy
int make_proxy_request(const char *proxy_url);

#endif // CAMERA_PROXY_CLIENT_H 