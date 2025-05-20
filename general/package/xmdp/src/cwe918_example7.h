#ifndef CWE918_EXAMPLE7_H
#define CWE918_EXAMPLE7_H

#include <sys/socket.h>
#include <netinet/in.h>

// Function to establish connection using inet_pton and connect
int establish_pton_connection(const char *ip_address);

#endif // CWE918_EXAMPLE7_H 