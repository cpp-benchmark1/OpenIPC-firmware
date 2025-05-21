#ifndef CAMERA_PTON_CONNECT_H
#define CAMERA_PTON_CONNECT_H

#include <sys/socket.h>
#include <netinet/in.h>

// Function to establish connection using inet_pton and connect
int establish_pton_connection(const char *ip_address);

#endif // CAMERA_PTON_CONNECT_H 