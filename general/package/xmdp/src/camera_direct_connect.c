#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "camera_direct_connect.h"

int establish_direct_connection(const char *ip_address) {
    int sockfd;
    struct sockaddr_in servaddr;
    
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return -1;
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(80);  // Default HTTP port
    
    if (inet_pton(AF_INET, ip_address, &servaddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        return -1;
    }
    
    // SINK: SSRF vulnerability - connection established to user-controlled IP
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect failed");
        return -1;
    }
    
    return sockfd;
} 