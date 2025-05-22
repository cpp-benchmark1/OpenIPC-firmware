#include <stdio.h>
#include <string.h>
#include "config_string_formatter.h"

// Transformation: Replace spaces with underscores
static void replace_spaces(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] == ' ') {
            str[i] = '_';
        }
    }
}

// Transformation: Add user prefix
static void add_user_prefix(char *str, size_t size) {
    char temp[256];
    strncpy(temp, str, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    snprintf(str, size, "user_%s", temp);
}

// Process configuration string
void process_config_string(const char *password) {
    if (!password) return;

    // Make a copy to avoid modifying the original
    char transformed[256];
    strncpy(transformed, password, sizeof(transformed) - 1);
    transformed[sizeof(transformed) - 1] = '\0';

    // Transformation 1: Replace spaces with underscores
    replace_spaces(transformed);

    // Transformation 2: Add user prefix
    add_user_prefix(transformed, sizeof(transformed));

    char buffer[512];
    // SINK: Indirect format string vulnerability through snprintf
    snprintf(buffer, sizeof(buffer), transformed, "additional_data");  // Vulnerable to format string injection
    printf("Processed: %s\n", buffer);
} 