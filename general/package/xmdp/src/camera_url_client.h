#ifndef CAMERA_URL_CLIENT_H
#define CAMERA_URL_CLIENT_H

#include <curl/curl.h>

// Function to make HTTP request with arbitrary URL
int make_arbitrary_request(const char *url);

#endif // CAMERA_URL_CLIENT_H 