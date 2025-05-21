CFLAGS=-Os -Wall -Wpedantic
LDFLAGS=-lm -lcurl

all: xmdp

xmdp: xmdp.o netip.o utils.o cjson/cJSON.o \
      camera_http_client.o camera_proxy_client.o camera_direct_connect.o camera_udp_sender.o \
      camera_url_client.o camera_proxy_arbitrary.o camera_pton_connect.o camera_pton_sender.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	-rm xmdp *.o
