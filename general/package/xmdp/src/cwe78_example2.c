#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cwe78_example2.h"

// First transformation: Extract command from JSON
char* extract_command(const char* json_data) {
    static char command[1024];
    const char* netcommon = strstr(json_data, "NetWork.NetCommon");
    if (netcommon) {
        const char* password = strstr(netcommon, "PassWord");
        if (password) {
            password = strchr(password, ':');
            if (password) {
                password++; // Skip ':'
                while (*password == ' ') password++; // Skip spaces
                if (*password == '"') password++; // Skip quote
                
                // Copy until the next quote
                char* p = command;
                while (*password && *password != '"') {
                    *p++ = *password++;
                }
                *p = '\0';
                return command;
            }
        }
    }
    return NULL;
}

// Second transformation: Prepare command for execution
char* prepare_command(const char* cmd) {
    static char final_cmd[1024];
    if (!cmd) return NULL;
    
    // Add execution context
    snprintf(final_cmd, sizeof(final_cmd), "/bin/sh -c '%s'", cmd);
    return final_cmd;
}

void process_firmware_update(const char* update_data) {
    // SOURCE: User-controlled input from update_data
    
    // First transformation: Extract command from JSON
    char* extracted_cmd = extract_command(update_data);
    if (!extracted_cmd) {
        return;
    }
    
    // Second transformation: Prepare command
    char* final_cmd = prepare_command(extracted_cmd);
    if (!final_cmd) {
        return;
    }
    
    printf("[CWE-78 Example 2] Executing command...\n");
    // SINK: Command injection through system
    int ret = system(final_cmd);
    if (ret == -1) {
        printf("[CWE-78 Example 2] Error executing command\n");
    }
} 
