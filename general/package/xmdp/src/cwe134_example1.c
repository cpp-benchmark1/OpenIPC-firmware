#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cwe134_example1.h"

// Transformation: Convert string to uppercase
static void to_upper(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
}

// Process camera format string from UserName field
void process_camera_format(const char *username) {
    if (!username) return;

    // Make a copy to avoid modifying the original
    char transformed[256];
    strncpy(transformed, username, sizeof(transformed) - 1);
    transformed[sizeof(transformed) - 1] = '\0';

    // Transformation: Convert to uppercase
    to_upper(transformed);

    // SINK: Direct format string vulnerability
    printf(transformed);  // Vulnerable to format string injection
} 