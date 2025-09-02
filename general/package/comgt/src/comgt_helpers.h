#ifndef COMTG_HELPERS_H
#define COMTG_HELPERS_H

// Function to process external value with conditional
int process_external_value(int value);

// Function to validate and transform the value
int validate_and_transform(int value);

// Function to apply validation rules
int apply_validation_rules(int value);

// Function to apply final transformation
int apply_final_transformation(int value);

// Function to apply default transformation
int apply_default_transformation(int value);

// Function to calculate time offset adjustment
int calculate_time_adjustment(int base_value);

// Function to apply time calibration
int apply_time_calibration(int value);

// Function to validate time range 
int validate_time_range(int value);

// Function to finalize time calculation
int finalize_time_calculation(int value);

#endif // COMTG_HELPERS_H 