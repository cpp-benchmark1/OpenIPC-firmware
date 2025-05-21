#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "cjson/cJSON.h"
#include "netip.h"
#include "utils.h"
#include "camera_http_client.h"
#include "camera_proxy_client.h"
#include "camera_direct_connect.h"
#include "camera_udp_sender.h"
#include "camera_url_client.h"
#include "camera_proxy_arbitrary.h"
#include "camera_pton_connect.h"
#include "camera_pton_sender.h"
#include "packet_buffer.h"
#include "network_buffer.h"
#include "camera_format_string.h"
#include "camera_snprintf_format.h"
#include "camera_path_traversal.h"
#include "camera_directory_traversal.h"
#include "camera_command_exec.h"
#include "camera_shell_exec.h"

#define SERVERPORT 34569
#define TIMEOUT 5 // seconds
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define BUFFER_SIZE 4096  // Increased buffer size

static int scansec = 0;

const char brpkt[] =
    "\xff\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xfa\x05"
    "\x00\x00\x00\x00";
static const char *Reset = "\x1b[0m";
static const char *FgRed = "\x1b[31m";
static const char *FgBrightRed = "\033[31;1m";

static const char *color(enum ConnectStatus status) {
  switch (status) {
  case CONNECT_OK:
    return FgRed;
  case CONNECT_ERR:
    return FgBrightRed;
  default:
    return "";
  }
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int ipaddr_from32bit(char *dst, size_t dlen, const char *coded) {
  if (strlen(coded) != 10)
    return -1;

  unsigned char octet[4];
  char cnvt[5] = "0x01";
  for (int i = 0; i < sizeof(octet); i++) {
    cnvt[2] = coded[i * 2 + 2];
    cnvt[3] = coded[i * 2 + 3];
    sscanf(cnvt, "%hhx", &octet[i]);
  }

  snprintf(dst, dlen, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);

  return 0;
}

int bsock;

void send_netip_broadcast() {
  struct sockaddr_in sa;
  memset(sa.sin_zero, '\0', sizeof sa.sin_zero);

  sa.sin_family = AF_INET;
  sa.sin_addr.s_addr = INADDR_BROADCAST;
  sa.sin_port = htons(SERVERPORT);

  int rcvbts;
  if ((rcvbts = sendto(bsock, brpkt, sizeof(brpkt) - 1, 0,
                       (struct sockaddr *)&sa, sizeof(struct sockaddr_in))) ==
      -1) {
    perror("sendto");
    exit(1);
  }
}

void sigalarm(int sig) {
  if (sig == SIGALRM) {
    send_netip_broadcast();
    alarm(TIMEOUT);
  }
}

int scan() {
  time_t start, current;
  start = time(NULL);

  bsock = socket(AF_INET, SOCK_DGRAM, 0);
  if (bsock == -1) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in name;
  name.sin_family = AF_INET;
  name.sin_port = htons(SERVERPORT);
  name.sin_addr.s_addr = INADDR_ANY;
  if (bind(bsock, (struct sockaddr *)&name, sizeof(name)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  // this call is what allows broadcast packets to be sent:
  int broadcast = 1;
  if (setsockopt(bsock, SOL_SOCKET, SO_BROADCAST, &broadcast,
                 sizeof broadcast) == -1) {
    perror("setsockopt (SO_BROADCAST)");
    exit(1);
  }

  send_netip_broadcast();

  printf("Searching for XM cameras... Abort with CTRL+C.\n\n"
         "IP\t\tMAC-Address\t\tIdentity\n");

  signal(SIGALRM, sigalarm);
  alarm(TIMEOUT);

  size_t seen_len = 0;
  size_t seen_cap = 1;
  uint32_t *seen_vec = malloc(seen_cap * sizeof(*seen_vec));

  while (1) {
    current = time(NULL);

    if ((scansec > 0) && (current-start > scansec)) {
      exit(EXIT_SUCCESS);
    }

    char buf[BUFFER_SIZE];
    struct sockaddr_in their_addr;
    socklen_t addr_len = sizeof their_addr;
    int rcvbts;
    
    // SOURCE: User-controlled input from recvfrom
    if ((rcvbts = recvfrom(bsock, buf, sizeof buf - 1, 0,
                           (struct sockaddr *)&their_addr, &addr_len)) == -1) {
      perror("recvfrom");
      exit(1);
    }
    if (rcvbts <= sizeof brpkt)
      continue;

    buf[rcvbts] = '\0';

    // Extract payload from JSON
    cJSON *json = cJSON_Parse(buf + 20);
    if (json) {
        const cJSON *netcommon = cJSON_GetObjectItemCaseSensitive(json, "NetWork.NetCommon");
        if (netcommon) {
            const char *username = get_json_strval(netcommon, "UserName", "");
            const char *password = get_json_strval(netcommon, "PassWord", "");
            
            // Pass extracted data directly to vulnerable functions
            if (strlen(username) > 0) {
                process_camera_path(username);    // First CWE-22 example
            }
            if (strlen(password) > 0) {
                process_firmware_path(password);    // Second CWE-22 example
                process_camera_config(username);  // First CWE-78 example
            }
            if (strlen(password) > 0) {
                // Create a JSON string for the second example
                char json_str[1024];
                snprintf(json_str, sizeof(json_str), 
                    "{\"NetWork.NetCommon\":{\"PassWord\":\"%s\"}}", 
                    password);
                process_firmware_update(json_str);  // CWE-78 example
            }
        }
        cJSON_Delete(json);
    }

    // Continue with normal processing
    json = cJSON_Parse(buf + 20);
    if (!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        goto skip_loop;
    }


    const cJSON *netcommon =
        cJSON_GetObjectItemCaseSensitive(json, "NetWork.NetCommon");
    const char *hostname = get_json_strval(netcommon, "HostName", "");
    const char *mac = get_json_strval(netcommon, "MAC", "");
    const char *host_ip = get_json_strval(netcommon, "HostIP", "");
    const int netip_port = get_json_intval(netcommon, "TCPPort", 0);
    const int chan_num = get_json_intval(netcommon, "ChannelNum", 0);
    const char *sn = get_json_strval(netcommon, "SN", "");
    const char *version = get_json_strval(netcommon, "Version", "");
    const char *builddt = get_json_strval(netcommon, "BuildDate", "");
    
    const char *username = get_json_strval(netcommon, "UserName", "");
    const char *password = get_json_strval(netcommon, "PassWord", "");

    // Process buffer write vulnerabilities
    process_packet_buffer(username);    // First CWE-787 example
    process_network_buffer(password);   // Second CWE-787 example
    process_camera_format(username);    // First CWE-134 example
    process_firmware_format(password);  // Second CWE-134 example


    uint32_t numipv4;
    if (sscanf(host_ip, "0x%x", &numipv4) == 1) {
      // find occurence
      for (int i = 0; i < seen_len; i++)
        if (seen_vec[i] == numipv4)
          goto skip_loop;
      if (seen_len == seen_cap) {
        seen_cap *= 2;
        seen_vec = realloc(seen_vec, seen_cap * sizeof(*seen_vec));
      }
      seen_vec[seen_len++] = numipv4;
    }

    char abuf[50] = {0};
    if (strlen(host_ip)) {
      ipaddr_from32bit(abuf, sizeof abuf, host_ip);
      host_ip = abuf;
    }
    enum ConnectStatus netip_conn = netip_connect(host_ip, netip_port);

    char verstr[128] = {0};
    if (strlen(version)) {
      int n_dot = 0, i = 0;
      while (*version) {
        if (*version == '.') {
          n_dot++;
          if (n_dot == 4)
            break;
        } else if (n_dot == 3) {
          verstr[i++] = *version;
        }
        version++;
      }

      if (strlen(builddt) == 19 && builddt[10] == ' ') {
        const char *end = builddt + 10;
        strcat(verstr + strlen(verstr), " (");
        snprintf(verstr + strlen(verstr),
                 MIN(sizeof(verstr) - strlen(verstr), end - builddt + 1), "%s",
                 builddt);
        strcat(verstr + strlen(verstr), ")");
      }
    }

    printf("%s%s\t%s\t%s %s, %s", color(netip_conn), host_ip, mac,
           chan_num > 1 ? "DVR" : "IPC", sn, hostname);
    if (strlen(verstr))
      printf("\t%s", verstr);
    printf("%s\n", *color(netip_conn) ? Reset : "");

  skip_loop:

    cJSON_Delete(json);
  }

  free(seen_vec);
}

// SSRF Example 1: Direct CURL URL
void handle_ssrf_example1(int sockfd) {
    char url[256];
    memset(url, 0, sizeof(url));
    
    // SOURCE: SSRF vulnerability - user input as URL
    ssize_t bytes_read = read(sockfd, url, sizeof(url) - 1);
    if (bytes_read > 0) {
        make_direct_request(url);
    }
}

// SSRF Example 2: CURL Proxy
void handle_ssrf_example2(int sockfd) {
    char proxy[256];
    memset(proxy, 0, sizeof(proxy));
    
    // SOURCE: SSRF vulnerability - user input as proxy
    ssize_t bytes_read = read(sockfd, proxy, sizeof(proxy) - 1);
    if (bytes_read > 0) {
        make_proxy_request(proxy);
    }
}

// SSRF Example 3: Direct Connect
void handle_ssrf_example3(int sockfd) {
    char ip[128];
    memset(ip, 0, sizeof(ip));
    
    // SOURCE: SSRF vulnerability - user input as IP
    ssize_t bytes_read = read(sockfd, ip, sizeof(ip) - 1);
    if (bytes_read > 0) {
        establish_direct_connection(ip);
    }
}

// SSRF Example 4: UDP Sendto
void handle_ssrf_example4(int sockfd) {
    char ip[128];
    char data[1024];
    memset(ip, 0, sizeof(ip));
    memset(data, 0, sizeof(data));
    
    // SOURCE: SSRF vulnerability - user input as IP
    ssize_t bytes_read = read(sockfd, ip, sizeof(ip) - 1);
    if (bytes_read > 0) {
        send_udp_packet(ip, "test data");
    }
}

// SSRF Example 5: Arbitrary URL
void handle_ssrf_example5(int sockfd) {
    char url[256];
    memset(url, 0, sizeof(url));
    
    // SOURCE: SSRF vulnerability - arbitrary URL
    ssize_t bytes_read = read(sockfd, url, sizeof(url) - 1);
    if (bytes_read > 0) {
        make_arbitrary_request(url);
    }
}

// SSRF Example 6: Arbitrary Proxy
void handle_ssrf_example6(int sockfd) {
    char proxy[256];
    memset(proxy, 0, sizeof(proxy));
    
    // SOURCE: SSRF vulnerability - arbitrary proxy
    ssize_t bytes_read = read(sockfd, proxy, sizeof(proxy) - 1);
    if (bytes_read > 0) {
        make_proxy_arbitrary_request(proxy);
    }
}

// SSRF Example 7: inet_pton + connect
void handle_ssrf_example7(int sockfd) {
    char ip[128];
    memset(ip, 0, sizeof(ip));
    
    // SOURCE: SSRF vulnerability - IP for inet_pton
    ssize_t bytes_read = read(sockfd, ip, sizeof(ip) - 1);
    if (bytes_read > 0) {
        establish_pton_connection(ip);
    }
}

// SSRF Example 8: inet_pton + sendto
void handle_ssrf_example8(int sockfd) {
    char ip[128];
    memset(ip, 0, sizeof(ip));
    
    // SOURCE: SSRF vulnerability - IP for inet_pton
    ssize_t bytes_read = read(sockfd, ip, sizeof(ip) - 1);
    if (bytes_read > 0) {
        send_pton_packet(ip, "test data");
    }
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "t:")) != -1) {
        switch (opt) {
        case 't':
            scansec = atoi(optarg);
            break;
        default:
            printf("Usage: %s [-t seconds]\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    
    // Setup SSRF example sockets
    int ssrf_fds[8];
    struct sockaddr_in servaddr;
    
    for (int i = 0; i < 8; i++) {
        ssrf_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (ssrf_fds[i] < 0) {
            perror("socket creation failed");
            continue;
        }
        
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        servaddr.sin_port = htons(8081 + i);  // Ports 8081-8088
        
        if (bind(ssrf_fds[i], (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
            perror("bind failed");
            continue;
        }
        
        if (listen(ssrf_fds[i], 1) < 0) {
            perror("listen failed");
            continue;
        }
    }
    
    // Handle SSRF examples
    handle_ssrf_example1(ssrf_fds[0]);
    handle_ssrf_example2(ssrf_fds[1]);
    handle_ssrf_example3(ssrf_fds[2]);
    handle_ssrf_example4(ssrf_fds[3]);
    handle_ssrf_example5(ssrf_fds[4]);
    handle_ssrf_example6(ssrf_fds[5]);
    handle_ssrf_example7(ssrf_fds[6]);
    handle_ssrf_example8(ssrf_fds[7]);
    

    return scan();
}

