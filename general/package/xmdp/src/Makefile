CFLAGS=-Os -Wall -Wpedantic
LDFLAGS=-lm

all: xmdp

xmdp: xmdp.o netip.o utils.o cwe134_example1.o cwe134_example2.o cjson/cJSON.c
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	-rm xmdp *.o
