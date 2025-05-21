#ifndef CAMERA_DIRECT_CONNECT_H
#define CAMERA_DIRECT_CONNECT_H

#include <sys/socket.h>
#include <netinet/in.h>

// Function to establish direct connection using user input as IP
int establish_direct_connection(const char *ip_address);

#endif // CAMERA_DIRECT_CONNECT_H 