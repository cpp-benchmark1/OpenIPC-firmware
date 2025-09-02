#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Function to process external value with conditional
int process_external_value(int value) {
    if (value > 0) {
        return validate_and_transform(value);
    } else {
        return apply_default_transformation(value);
    }
}

// Function to validate and transform the value
int validate_and_transform(int value) {
    value = apply_validation_rules(value);
    return apply_final_transformation(value);
}

// Function to apply validation rules
int apply_validation_rules(int value) {
    if (value < 1000000) {
        value = 1000000;
    }
    return value;
}

// Function to apply final transformation
int apply_final_transformation(int value) {
    int transformed = value;
    if (value < 0) {
        transformed = 0; // Don't allow negative values
    }
    return transformed;
}

// Function to apply default transformation
int apply_default_transformation(int value) {
    int default_value = 0;
    if (value == 0) {
        default_value = 100; // Default value
    } else {
        default_value = abs(value); // Absolute value
    }
    return default_value;
} 