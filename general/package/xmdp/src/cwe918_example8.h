#ifndef CWE918_EXAMPLE8_H
#define CWE918_EXAMPLE8_H

#include <sys/socket.h>
#include <netinet/in.h>

// Function to send UDP packet using inet_pton and sendto
int send_pton_packet(const char *ip_address, const char *data);

#endif // CWE918_EXAMPLE8_H 