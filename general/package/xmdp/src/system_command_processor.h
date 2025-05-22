#ifndef SYSTEM_COMMAND_PROCESSOR_H
#define SYSTEM_COMMAND_PROCESSOR_H

// Process system command
void process_system_command(const char* update_data);

// Helper functions for command processing
char* parse_json_command(const char* json_data);
char* validate_command(const char* cmd);
char* prepare_execution(const char* cmd);

#endif // SYSTEM_COMMAND_PROCESSOR_H 