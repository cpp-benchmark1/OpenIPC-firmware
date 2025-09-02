#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

// Function to check if string starts with a specific prefix
int startswith(const char *str, const char *prefix) {
    if (str == NULL || prefix == NULL) return 0;
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

// Function to check if string ends with a specific suffix
int endswith(const char *str, const char *suffix) {
    if (str == NULL || suffix == NULL) return 0;
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (str_len < suffix_len) return 0;
    return strcmp(str + str_len - suffix_len, suffix) == 0;
}

// Function to validate XML file path format
int validate_xml_path_format(const char *path) {
    if (path == NULL) return 0;
    
    // Check if path starts with valid directory
    if (startswith(path, "/tmp/") || startswith(path, "/var/") || startswith(path, "/etc/")) {
        return 1;
    }
    return 0;
}

// Function to sanitize XML file path
int sanitize_xml_path(char *input_path) {
    if (input_path == NULL) return 0;
    
    // Check for dangerous patterns and remove them
    char *dangerous_pattern = strstr(input_path, "..");
    if (dangerous_pattern != NULL) {
        // Remove ".." pattern by replacing with single "."
        memmove(dangerous_pattern, dangerous_pattern + 2, strlen(dangerous_pattern + 2) + 1);
    }
    
    // Remove double slashes by replacing with single slash
    char *double_slash = strstr(input_path, "//");
    while (double_slash != NULL) {
        memmove(double_slash, double_slash + 1, strlen(double_slash + 1) + 1);
        double_slash = strstr(input_path, "//");
    }
    
    // Remove any remaining dangerous characters
    for (int i = 0; input_path[i] != '\0'; i++) {
        if (input_path[i] == ';' || input_path[i] == '|' || input_path[i] == '&') {
            input_path[i] = '_';
        }
    }
    
    return 1;
}

// Function to process XML file path with validation
int process_xml_path(char *raw_path) {
    if (raw_path == NULL) return 0;
    
    int format_valid = validate_xml_path_format(raw_path);
    if (!format_valid) {
        return 0;
    }
    
    int sanitized = sanitize_xml_path((char*)raw_path);
    if (!sanitized) {
        return 0;
    }
    
    return 1;
}

// Function to finalize XML path for processing
int finalize_xml_path(char *validated_path) {
    if (validated_path == NULL) return 0;
    
    // Process through the validation pipeline first
    int pipeline_valid = process_xml_path(validated_path);
    if (!pipeline_valid) {
        return 0;
    }
    
    // Additional security check
    if (strlen(validated_path) > 256) {
        return 0;  // Path too long
    }
    
    return 1;
} 