#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cwe787_example2.h"

// Transformation: Add offset to index
static int add_offset(int index) {
    return index + 32;  // Add fixed offset
}

// Transformation: Scale index
static int scale_index(int index) {
    return index * 2;  // Double the index
}

// Process firmware buffer write from PassWord field
void process_firmware_buffer(const char *password) {
    if (!password) return;

    // Make a copy to avoid modifying the original
    char transformed[256];
    strncpy(transformed, password, sizeof(transformed) - 1);
    transformed[sizeof(transformed) - 1] = '\0';

    // Parse index from input
    int index = atoi(transformed);
    
    // Transformation 1: Add offset
    index = add_offset(index);
    
    // Transformation 2: Scale index
    index = scale_index(index);

    // Fixed-size buffer
    char buffer[64] = {0};
    
    // Write multiple bytes
    for (int i = 0; i < 10; i++) {
        // SINK: Indirect out-of-bounds write with multiple operations
        buffer[index + i] = 'B';  // Write multiple bytes
    }
    
    // Additional dangerous operation: write to a pointer
    char *ptr = buffer;
    ptr[index] = 'C';  // Another write to the same area
    
    // More dangerous: write to a pointer with offset
    *(ptr + index) = 'D';  // Direct pointer arithmetic
} 