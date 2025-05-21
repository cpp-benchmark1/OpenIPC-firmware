#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "cwe22_example2.h"

// Define alphasort if not available
#ifndef alphasort
int alphasort_example2(const struct dirent **a, const struct dirent **b) {
    return strcoll((*a)->d_name, (*b)->d_name);
}
#endif

// First transformation: Handle slashes
char* handle_slashes_example2(const char* input) {
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
char* make_absolute_example2(const char* input) {
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

void process_firmware_path(const char* config_data) {
    // SOURCE: User-controlled input from config_data
    
    // First transformation: handle slashes
    char* transformed = handle_slashes_example2(config_data);
    if (!transformed) {
        return;
    }
    
    // Second transformation: make absolute path
    char* final_path = make_absolute_example2(transformed);
    if (!final_path) {
        return;
    }
    
    struct dirent **namelist;
    // SINK: Path traversal through scandir
    int n = scandir(final_path, &namelist, NULL, alphasort_example2);
    
    if (n < 0) {
        printf("[CWE-22 Example 2] Failed to read directory\n");
        return;
    }
    
    printf("[CWE-22 Example 2] Directory contents of %s:\n", final_path);
    for (int i = 0; i < n; i++) {
        printf("  %s\n", namelist[i]->d_name);
        free(namelist[i]);
    }
    free(namelist);
}

void list_directory_contents(const char *path) {
    char final_path[256];
    snprintf(final_path, sizeof(final_path), "%s", path);
    
    // SINK: Path traversal through opendir
    DIR *dir = opendir(final_path);
    
    if (dir == NULL) {
        printf("[CWE-22 Example 2] Failed to read directory\n");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("[CWE-22 Example 2] Found: %s\n", entry->d_name);
    }

    closedir(dir);
} 