CC = cc
CFLAGS = -Os -Wall -Wpedantic
LDFLAGS = -lm -lcurl

SRCS = xmdp.c netip.c utils.c \
       camera_path_traversal.c camera_directory_traversal.c \
       camera_command_exec.c camera_shell_exec.c \
       camera_format_string.c camera_snprintf_format.c \
       packet_buffer.c network_buffer.c \
       camera_http_client.c camera_proxy_client.c camera_direct_connect.c \
       camera_udp_sender.c camera_url_client.c camera_proxy_arbitrary.c \
       camera_pton_connect.c camera_pton_sender.c \
       cjson/cJSON.c

OBJS = $(SRCS:.c=.o)
TARGET = xmdp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
