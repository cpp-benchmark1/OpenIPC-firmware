#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet_buffer.h"

// Process network packet buffer write from UserName field
void process_packet_buffer(const char *username) {
    if (!username) return;

    // Make a copy to avoid modifying the original
    char transformed[256];
    strncpy(transformed, username, sizeof(transformed) - 1);
    transformed[sizeof(transformed) - 1] = '\0';

    // Transformation: Convert to uppercase
    for (int i = 0; transformed[i]; i++) {
        transformed[i] = toupper(transformed[i]);
    }

    // Fixed-size buffer
    char buffer[64] = {0};
    
    // Parse index from input
    int index = atoi(transformed);
    
    // Write multiple bytes to cause more damage
    for (int i = 0; i < 10; i++) {
        // SINK: Direct out-of-bounds write with multiple writes
        buffer[index + i] = 'A';  // Write multiple bytes
    }
    
    // Additional dangerous operation: write to a pointer
    char *ptr = buffer;
    ptr[index] = 'B';  // Another write to the same area
} 