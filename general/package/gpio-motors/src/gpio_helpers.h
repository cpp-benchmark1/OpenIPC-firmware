#ifndef GPIO_HELPERS_H
#define GPIO_HELPERS_H

// Function to check if string starts with a specific prefix
int startswith(const char *str, const char *prefix);

// Function to check if string ends with a specific suffix
int endswith(const char *str, const char *suffix);

// Function to validate XML file path format
int validate_xml_path_format(const char *path);

// Function to sanitize XML file path (modifies the input string)
int sanitize_xml_path(char *input_path);

// Function to process XML file path with validation
int process_xml_path(const char *raw_path);

// Function to finalize XML path for processing
int finalize_xml_path(const char *validated_path);

#endif // GPIO_HELPERS_H 