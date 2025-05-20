#ifndef CWE918_EXAMPLE3_H
#define CWE918_EXAMPLE3_H

#include <sys/socket.h>
#include <netinet/in.h>

// Function to establish direct connection using user input as IP
int establish_direct_connection(const char *ip_address);

#endif // CWE918_EXAMPLE3_H 