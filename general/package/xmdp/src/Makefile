CFLAGS=-Os -Wall -Wpedantic
LDFLAGS=-lm -lcurl

all: xmdp

xmdp: xmdp.o netip.o utils.o cjson/cJSON.o \
      cwe918_example1.o cwe918_example2.o cwe918_example3.o cwe918_example4.o \
      cwe918_example5.o cwe918_example6.o cwe918_example7.o cwe918_example8.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	-rm xmdp *.o
