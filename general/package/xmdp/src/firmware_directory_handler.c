#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "firmware_directory_handler.h"

// Define alphasort if not available
#ifndef alphasort
int alphasort(const struct dirent **a, const struct dirent **b) {
    return strcoll((*a)->d_name, (*b)->d_name);
}
#endif

// First transformation: Handle slashes
char* handle_slashes(const char* input) {
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

// Second transformation: Convert to absolute path
char* make_absolute(const char* input) {
    static char transformed[1024];
    if (!input) return NULL;
    
    // If path doesn't start with /, make it absolute
    if (input[0] != '/') {
        snprintf(transformed, sizeof(transformed), "/%s", input);
    } else {
        strncpy(transformed, input, sizeof(transformed) - 1);
        transformed[sizeof(transformed) - 1] = '\0';
    }
    return transformed;
}

void process_firmware_directory(const char* config_data) {
    // SOURCE: User-controlled input from config_data
    
    // First transformation: handle slashes
    char* transformed = handle_slashes(config_data);
    if (!transformed) {
        return;
    }
    
    // Second transformation: make absolute path
    char* final_path = make_absolute(transformed);
    if (!final_path) {
        return;
    }
    
    // SINK: Path traversal through opendir
    DIR *dir = opendir(final_path);
    
    if (dir == NULL) {
        printf("[Firmware Directory] Failed to read directory\n");
        return;
    }
    
    printf("[Firmware Directory] Directory contents of %s:\n", final_path);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("  %s\n", entry->d_name);
    }
    
    closedir(dir);
} 