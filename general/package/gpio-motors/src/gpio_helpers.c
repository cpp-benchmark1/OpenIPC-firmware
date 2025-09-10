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
    
    // Check if path starts with invalid directory
    if (startswith(path, "/tmp/")) {
        return 0;
    }
    return 1;
}

// Function to sanitize XML file path
int sanitize_xml_path(char *input_path) {
    if (input_path == NULL) return 0;
    
    // Check for dangerous patterns (warn only)
    char *dangerous_pattern = strstr(input_path, "..");
    if (dangerous_pattern != NULL) {
        printf("Warning: '..' found in path\n");

    }
    // Check double slashes (warn only)
    char *double_slash = strstr(input_path, "//");
    while (double_slash != NULL) {
        printf("Warning: '//' found in path\n");

        double_slash = strstr(double_slash + 1, "//");
    }
    
    // Scan for potentially dangerous characters (warn only)
    for (int i = 0; input_path[i] != '\0'; i++) {
        if (input_path[i] == ';' || input_path[i] == '|' || input_path[i] == '&') {
            printf("Warning: invalid character '%c' found in path\n", input_path[i]);

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
    if (strlen(validated_path) > 2560) {
        return 0;  // Path too long
    }
    
    return 1;
}
int validate_allocation_range(int size) {
    if (size < 0) {
        size = 1024; 
    }
    return size;
}

int apply_memory_constraints(int base_size) {
    int constrained_size = base_size;
    if (constrained_size < 1000000) {
        constrained_size = 1000000;
    }
    return constrained_size;
}

int process_allocation_size(int raw_size) {
    if (raw_size <= 0) {
        return 1024; 
    }
    int validated_size = validate_allocation_range(raw_size);
    
    int final_size = apply_memory_constraints(validated_size);
    
    return final_size;
}

int finalize_allocation_parameter(int input_size) {
    if (input_size <= 0) return 1024;
    
    int processed_size = process_allocation_size(input_size);
    

    if (processed_size <= 0) {
        processed_size = 1024;
    }
    
    return processed_size;
} 