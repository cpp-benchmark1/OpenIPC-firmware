#ifndef CAMERA_UDP_SENDER_H
#define CAMERA_UDP_SENDER_H

#include <sys/socket.h>
#include <netinet/in.h>

// Function to send UDP packet to user specified IP
int send_udp_packet(const char *ip_address, const char *data);

#endif // CAMERA_UDP_SENDER_H 