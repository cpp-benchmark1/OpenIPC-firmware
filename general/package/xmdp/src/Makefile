CC = cc
CFLAGS = -Os -Wall -Wpedantic
LDFLAGS = -lm -lcurl

SRCS = xmdp.c netip.c utils.c \
       config_file_handler.c firmware_directory_handler.c \
       device_config_manager.c system_command_processor.c \
       camera_format_string.c config_string_formatter.c \
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
