CC = cc
CFLAGS = -Os -Wall -Wpedantic
LDFLAGS = -lm

SRCS = xmdp.c netip.c utils.c cjson/cJSON.c cwe78_example1.c cwe78_example2.c cwe22_example1.c cwe22_example2.c
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
