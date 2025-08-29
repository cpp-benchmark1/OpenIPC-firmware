gpio-motors:
	mkdir -p output
	$(CC) $@.c -o output/$@ -s -Wall $(shell pkg-config --cflags --libs libxml-2.0)
