CC = cc
CFLAGS = -Os -Wall -Wpedantic
LDFLAGS = -lm

SRCS = xmdp.c netip.c utils.c cjson/cJSON.c \
       camera_path_traversal.c camera_directory_traversal.c \
       camera_command_exec.c camera_shell_exec.c
OBJS = $(SRCS:.c=.o)
TARGET = xmdp

SRCS = xmdp.c netip.c utils.c \
       camera_format_string.c camera_snprintf_format.c \
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
