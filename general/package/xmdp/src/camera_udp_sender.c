#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "camera_udp_sender.h"

int send_udp_packet(const char *ip_address, const char *data) {
    int sockfd;
    struct sockaddr_in servaddr;
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        return -1;
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(53);  // DNS port as example
    
    if (inet_pton(AF_INET, ip_address, &servaddr.sin_addr) <= 0) {
        perror("inet_pton failed");
        return -1;
    }
    
    // SINK: SSRF vulnerability - packet sent to user-controlled IP
    if (sendto(sockfd, data, strlen(data), 0, 
               (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("sendto failed");
        return -1;
    }
    
    return 0;
} 