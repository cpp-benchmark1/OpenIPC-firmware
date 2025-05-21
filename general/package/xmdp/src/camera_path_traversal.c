#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "camera_path_traversal.h"

// Single transformation: Handle slashes
char* handle_slashes_example1(const char* input) {
    static char transformed[1024];
    if (!input) return NULL;
    
    // Simple transformation: replace multiple slashes with single slash
    char* p = transformed;
    int prev_was_slash = 0;
    
    while (*input) {
        if (*input == '/') {
            if (!prev_was_slash) {
                *p++ = '/';
                prev_was_slash = 1;
            }
        } else {
            *p++ = *input;
            prev_was_slash = 0;
        }
        input++;
    }
    *p = '\0';
    return transformed;
}

void process_camera_path(const char* config_data) {
    // SOURCE: User-controlled input from config_data
    
    // Single transformation: handle slashes
    char* transformed = handle_slashes_example1(config_data);
    if (!transformed) {
        return;
    }
    
    // SINK: Path traversal through fopen
    FILE* fp = fopen(transformed, "r");
    if (!fp) {
        printf("[CWE-22 Example 1] Failed to open file: %s\n", transformed);
        return;
    }
    
    printf("[CWE-22 Example 1] Successfully opened file: %s\n", transformed);
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("  %s", buffer);
    }
    fclose(fp);
} 