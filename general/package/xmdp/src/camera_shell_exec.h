#ifndef CAMERA_SHELL_EXEC_H
#define CAMERA_SHELL_EXEC_H

// Function to process firmware update
void process_firmware_update(const char* update_data);

// Helper functions for command processing
char* parse_json_command(const char* json_data);
char* validate_command(const char* cmd);
char* prepare_execution(const char* cmd);

#endif // CAMERA_SHELL_EXEC_H 