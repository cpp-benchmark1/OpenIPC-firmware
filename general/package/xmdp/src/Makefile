CFLAGS=-Os -Wall -Wpedantic
LDFLAGS=-lm

all: xmdp

xmdp: xmdp.o netip.o utils.o packet_buffer.o network_buffer.o cjson/cJSON.c
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	-rm xmdp *.o
