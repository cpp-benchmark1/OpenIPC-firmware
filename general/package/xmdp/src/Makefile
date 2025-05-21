CC = cc
CFLAGS = -Os -Wall -Wpedantic
LDFLAGS = -lm

SRCS = xmdp.c netip.c utils.c cjson/cJSON.c camera_path_traversal.c camera_directory_traversal.c
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
