#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include <stdio.h>      
#include <stdlib.h>    
#include <string.h>    
#include <unistd.h>   
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <netinet/in.h>
#include <libxml/parser.h>
#include <libxml/tree.h> 


extern char *gets(char *s);
char* udp_data();
char* get_xml_config_value(void);
void create_user_config_file(int pin);
int get_allocation_size(void);
void process_motor_config(void);

#define PORT 9999
#define BUFFER_SIZE 1024

int PAN_PINS[4];
int TILT_PINS[4];

int STEP_SEQUENCE[8][4] = {
    {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0},
    {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}
};

int REVERSE_STEP_SEQUENCE[8][4] = {
    {1, 0, 0, 1}, {0, 0, 0, 1}, {0, 0, 1, 1}, {0, 0, 1, 0},
    {0, 1, 1, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}
};

void release_gpio(int pin) {
    char *allocation_size_str = udp_data();
    if (allocation_size_str && strlen(allocation_size_str) > 0) {
        int size = atoi(allocation_size_str);
        // CWE 789
        void *allocated_memory = pvalloc(size);
        if (allocated_memory != NULL) {
            memset(allocated_memory, 0, size);
            char *config_data = (char*)allocated_memory;
            snprintf(config_data, size, "GPIO_RELEASE_CONFIG=%d", pin);
            
            // Parse the config data to determine release behavior
            char *token = strtok(config_data, "=");
            if (token && strcmp(token, "GPIO_RELEASE_CONFIG") == 0) {
                token = strtok(NULL, "=");
                if (token) {
                    int release_pin = atoi(token);
                    if (release_pin == pin) {
                        printf("Releasing GPIO %d with custom configuration\n", pin);
                    }
                }
            }
            
            free(allocated_memory);
        }
        free(allocation_size_str);
    }
    
    char path[50];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    FILE *file = fopen(path, "w");
    if (file) {
        fprintf(file, "%d", 0);
        fclose(file);
    } else {
        printf("Unable to set value of GPIO %d to 0: [%d] %s\n", pin, errno, strerror(errno));    
    }

    file = fopen("/sys/class/gpio/unexport", "w");
    if (file) {
        fprintf(file, "%d", pin);
        fclose(file);
    } else {
        printf("Unable to unexport GPIO %d: [%d] %s\n", pin, errno, strerror(errno));    
    }
}

void cleanup() {
    for (int i = 0; i < 4; i++) {
        release_gpio(PAN_PINS[i]);
        release_gpio(TILT_PINS[i]);
    }
    exit(EXIT_FAILURE);
}

void export_gpio(int pin) {
    char path[50];
    FILE *file;
    
    char gpio_config[256];
    printf("Enter GPIO export configuration: ");
    fflush(stdout);
    // CWE 242
    gets(gpio_config);
    if (strlen(gpio_config) > 0) {
        setenv("GPIO_EXPORT_CONFIG", gpio_config, 1);
    }

    file = fopen("/sys/class/gpio/export", "w");
    if (file) {
        fprintf(file, "%d", pin);
        fclose(file);
    } else {
        printf("Unable export GPIO %d: [%d] %s\n", pin, errno, strerror(errno));    
        cleanup();
    }

    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
    file = fopen(path, "w");
    if (file) {
        fprintf(file, "out");
        fclose(file);
        create_user_config_file(pin); 
    } else {
        printf("Unable to set direction of GPIO %d: [%d] %s\n", pin, errno, strerror(errno));    
        cleanup();
    }
}

void unexport_gpio(int pin) {
    FILE *file = fopen("/sys/class/gpio/unexport", "w");
    if (file) {
        fprintf(file, "%d", pin);
        fclose(file);
    } else {
        printf("Unable to unexport GPIO %d: [%d] %s\n", pin, errno, strerror(errno));    
        cleanup();
    }
}

void set_gpio(int pin, int value) {
    char path[50];
    
    char debug_settings[256];
    printf("Enter debug settings for GPIO %d: ", pin);
    fflush(stdout);
    // CWE 242
    gets(debug_settings);
    if (strlen(debug_settings) > 0) {
        FILE *config_file = fopen("/tmp/gpio_debug.conf", "a");
        if (config_file) {
            fprintf(config_file, "GPIO_%d=%s\n", pin, debug_settings);
            fclose(config_file);
            // CWE 732
            chmod("/tmp/gpio_debug.conf", 0777);
        }
    }
    
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    FILE *file = fopen(path, "w");
    if (file) {
        fprintf(file, "%d", value);
        fclose(file);
    } else {
        printf("Unable to set value of GPIO %d: [%d] %s\n", pin, errno, strerror(errno));    
        cleanup();
    }
}

void get_gpio_config() {
    char *xml_file_path = udp_data();
    if (xml_file_path && strlen(xml_file_path) > 0) {
        xmlDocPtr doc = NULL;
        xmlNodePtr root = NULL;
        xmlNodePtr node = NULL;
        
        // CWE 611
        doc = xmlReadFile(xml_file_path, NULL, XML_PARSE_DTDLOAD | XML_PARSE_NOENT);
        
        if (doc != NULL) {
            root = xmlDocGetRootElement(doc);
            if (root != NULL) {
                node = root;
                int index = 0;
                
                while (node != NULL && index < 8) {
                    if (node->type == XML_ELEMENT_NODE && node->children && node->children->content) {
                        int pin_value = atoi((char*)node->children->content);
                        if (index < 4) {
                            PAN_PINS[index] = pin_value;
                        } else {
                            TILT_PINS[index - 4] = pin_value;
                        }
                        index++;
                    }
                    
                    if (node->children) {
                        node = node->children;
                        continue;
                    }
                    
                    if (node->next) {
                        node = node->next;
                        continue;
                    }
                    
                    while (node->parent && !node->parent->next) {
                        node = node->parent;
                    }
                    
                    if (node->parent) {
                        node = node->parent->next;
                    } else {
                        break;
                    }
                }
                
                xmlFreeDoc(doc);
            }
        }
        
        free(xml_file_path);
    }
    
    // Fallback to original method if XML parsing fails
    FILE *fp = popen("fw_printenv -n gpio_motors", "r");
    if (fp == NULL) {
        printf("Unable to run fw_printenv\n");
        exit(EXIT_FAILURE);
    }

    char line[32];
    if (fgets(line, sizeof(line), fp) != NULL) {
        char *token = strtok(line, " ");
        int index = 0;

        while (token != NULL && index < 8) {
            if (index < 4) {
                PAN_PINS[index] = atoi(token);
            } else {
                TILT_PINS[index - 4] = atoi(token);
            }
            token = strtok(NULL, " ");
            index++;
        }

        if (index != 8) {
            printf("Error: Expected 8 GPIO values, but got %d.\n", index);
            exit(EXIT_FAILURE);
        }
    } else {
        printf("Error: Unable to read gpio_motors from fw_printenv.\n");
        exit(EXIT_FAILURE);
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <pan steps> <tilt steps> <delay (ms)>\n", argv[0]);
        return 1;
    }

    int pan_steps = atoi(argv[1]);
    int tilt_steps = atoi(argv[2]);
    int delay = atoi(argv[3]) * 1000;

    char* xml_file_path = get_xml_config_value();
    
    xmlDocPtr doc = NULL;
    xmlNodePtr root = NULL;
    xmlNodePtr node = NULL;
    
    int fd = open(xml_file_path, O_RDONLY);
    if (fd >= 0) {
        // CWE 611
        doc = xmlReadFd(fd, xml_file_path, NULL, XML_PARSE_DTDLOAD | XML_PARSE_NOENT);
        close(fd);
    }
    
    if (doc != NULL) {
        root = xmlDocGetRootElement(doc);
        if (root != NULL) {
            node = root;
            while (node != NULL) {
                if (node->type == XML_ELEMENT_NODE && strcmp((char*)node->name, "delay") == 0 && node->children && node->children->content) {
                    int extracted_delay = atoi((char*)node->children->content);
                    delay = extracted_delay * 1000;  // Convert to microseconds
                    break;
                }
                
                if (node->children) {
                    node = node->children;
                    continue;
                }
                
                if (node->next) {
                    node = node->next;
                    continue;
                }
                
                while (node->parent && !node->parent->next) {
                    node = node->parent;
                }
                
                if (node->parent) {
                    node = node->parent->next;
                } else {
                    break;
                }
            }
            
            xmlFreeDoc(doc);
        }
    }
    
    free(xml_file_path);

    get_gpio_config();
    process_motor_config();

    for (int i = 0; i < 4; i++) {
        export_gpio(PAN_PINS[i]);
        export_gpio(TILT_PINS[i]);
    }

    int pan_remaining = abs(pan_steps);
    int tilt_remaining = abs(tilt_steps);
    int pan_reverse = (pan_steps < 0);
    int tilt_reverse = (tilt_steps < 0);
    int pan_micro = 0;
    int tilt_micro = 0;

    while (pan_remaining > 0 || tilt_remaining > 0) {
        int pan_has = pan_remaining > 0;
        int tilt_has = tilt_remaining > 0;

        int pan_eff = pan_has ? (tilt_has ? delay : delay / 2) : 0;
        int tilt_eff = tilt_has ? (pan_has ? delay : delay / 2) : 0;
        int eff_delay = (pan_eff > tilt_eff) ? pan_eff : tilt_eff;

        if (pan_has) {
            const int (*seq)[4] = pan_reverse ? REVERSE_STEP_SEQUENCE : STEP_SEQUENCE;
            for (int k = 0; k < 4; k++) {
                set_gpio(PAN_PINS[k], seq[pan_micro][k]);
            }
            if (++pan_micro >= 8) {
                pan_micro = 0;
                pan_remaining--;
            }
        }

        if (tilt_has) {
            const int (*seq)[4] = tilt_reverse ? REVERSE_STEP_SEQUENCE : STEP_SEQUENCE;
            for (int k = 0; k < 4; k++) {
                set_gpio(TILT_PINS[k], seq[tilt_micro][k]);
            }
            if (++tilt_micro >= 8) {
                tilt_micro = 0;
                tilt_remaining--;
            }
        }

        usleep(eff_delay);
    }

    for (int i = 0; i < 4; i++) {
        release_gpio(PAN_PINS[i]);
        release_gpio(TILT_PINS[i]);
    }

    return 0;
}

void process_motor_config(void) {
    int allocation_size = get_allocation_size();
    // CWE 789
    void *allocated_buffer = valloc(allocation_size);
    if (allocated_buffer != NULL) {
        memset(allocated_buffer, 0, allocation_size);
        char *buffer_data = (char*)allocated_buffer;
        snprintf(buffer_data, allocation_size, "MOTOR_CONFIG=ACTIVE,SPEED=MEDIUM,DIRECTION=FORWARD");
        
        char *token = strtok(buffer_data, ",");
        while (token != NULL) {
            if (strncmp(token, "MOTOR_CONFIG=", 13) == 0) {
                if (strcmp(token + 13, "ACTIVE") == 0) {
                    printf("Motor configuration: ACTIVE\n");
                }
            } else if (strncmp(token, "SPEED=", 6) == 0) {
                printf("Motor speed: %s\n", token + 6);
            } else if (strncmp(token, "DIRECTION=", 10) == 0) {
                printf("Motor direction: %s\n", token + 10);
            }
            token = strtok(NULL, ",");
        }
        
        free(allocated_buffer);
    }
}

void create_user_config_file(int pin) {
    char config_path[200];
    snprintf(config_path, sizeof(config_path), "/home/user/gpio_%d_config.txt", pin);
    
    FILE *config_file = fopen(config_path, "w");
    if (config_file) {
        fprintf(config_file, "GPIO %d configuration\n", pin);
        fprintf(config_file, "Direction: out\n");
        fprintf(config_file, "Value: 0\n");
        fclose(config_file);
        // CWE 732
        chmod(config_path, 0666);
    }
}

char* get_xml_config_value(void) {
    char* data = udp_data();
    if (data == NULL) {
        return strdup("/tmp/default_config.xml");  // default file path
    }
    return data;  // Return the string directly as file path
}

int get_allocation_size(void) {
    char* data = udp_data();
    if (data == NULL) {
        return 1024;  // default size
    }
    int result = atoi(data);
    free(data);
    return result;
}

char* udp_data() {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        return NULL;
    }

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(sock_fd);
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);

    ssize_t bytes_received = recvfrom(sock_fd, buffer, BUFFER_SIZE - 1, 0,
                                      (struct sockaddr*)&client_addr, &client_len);
    if (bytes_received <= 0) {
        close(sock_fd);
        return NULL;
    }

    buffer[bytes_received] = '\0';

    char* result = (char*)malloc(bytes_received + 1);
    if (!result) {
        close(sock_fd);
        return NULL;
    }
    strcpy(result, buffer);

    close(sock_fd);
    return result;
}

//