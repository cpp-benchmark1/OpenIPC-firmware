#ifndef CAMERA_HTTP_CLIENT_H
#define CAMERA_HTTP_CLIENT_H

#include <curl/curl.h>

// Function to make direct HTTP request using user input as URL
int make_direct_request(const char *url);

#endif // CAMERA_HTTP_CLIENT_H 