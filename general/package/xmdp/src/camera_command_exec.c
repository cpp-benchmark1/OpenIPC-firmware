#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "camera_command_exec.h"

// Transform command to lowercase
char* transform_command(const char* cmd) {
    static char transformed[1024];
    if (!cmd) return NULL;
    
    // Copy and convert to lowercase
    char* p = transformed;
    while (*cmd) {
        *p++ = tolower(*cmd++);
    }
    *p = '\0';
    return transformed;
}

void process_camera_config(const char* config_data) {
    char output[1024];
    FILE* fp;
    
    // SOURCE: User-controlled input from config_data
    
    // Transform command to lowercase
    char* transformed_cmd = transform_command(config_data);
    if (!transformed_cmd) {
        printf("[Command Exec] Failed to transform command\n");
        return;
    }
    printf("[Command Exec] Transformed command: %s\n", transformed_cmd);
    
    // SINK: Command injection through popen
    fp = popen(transformed_cmd, "r");
    if (fp == NULL) {
        printf("Error executing command\n");
        return;
    }
    
    // Read and print output
    while (fgets(output, sizeof(output), fp) != NULL) {
        printf("[Command Exec] Output: %s", output);
    }
    
    pclose(fp);
} 