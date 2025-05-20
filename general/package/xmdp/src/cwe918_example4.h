#ifndef CWE918_EXAMPLE4_H
#define CWE918_EXAMPLE4_H

#include <sys/socket.h>
#include <netinet/in.h>

// Function to send UDP packet to user specified IP
int send_udp_packet(const char *ip_address, const char *data);

#endif // CWE918_EXAMPLE4_H 