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
int process_xml_path(char *raw_path);

// Function to finalize XML path for processing
int finalize_xml_path(char *validated_path);

// Function to validate allocation size range
int validate_allocation_range(int size);

// Function to apply memory constraints
int apply_memory_constraints(int base_size);

// Function to process allocation size with validation
int process_allocation_size(int raw_size);

// Function to finalize allocation parameter
int finalize_allocation_parameter(int input_size);

#endif // GPIO_HELPERS_H 