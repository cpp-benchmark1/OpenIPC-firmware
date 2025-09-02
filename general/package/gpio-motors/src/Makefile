gpio-motors:
	mkdir -p output
	$(CC) gpio-motors.c gpio_helpers.c -o output/$@ -s -Wall $(shell pkg-config --cflags --libs libxml-2.0 libssh mysqlclient)
