#ifndef CAMERA_PTON_SENDER_H
#define CAMERA_PTON_SENDER_H

#include <sys/socket.h>
#include <netinet/in.h>

// Function to send UDP packet using inet_pton and sendto
int send_pton_packet(const char *ip_address, const char *data);

#endif // CAMERA_PTON_SENDER_H 