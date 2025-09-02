#
#   Makefile - build and install the comgt package
#
#   Copyright (C) 2005  Martin Gregorie,  martin@gregorie.org
#   Copyright (C) 2006  Paul Hardwick, paul@peck.org.uk
#   Copyright (C) 2022  Igor Zalatov, flyrouter@gmail.com
#

all: comgt

install:
	chmod a+x $(BIN)

clean:
	-rm *.o comgt

comgt: comgt.o comgt_helpers.o
	cc comgt.o comgt_helpers.o $(LDFLAGS) -o comgt

comgt.o: comgt.c comgt.h comgt_helpers.h
	cc comgt.c $(CFLAGS) -c

comgt_helpers.o: comgt_helpers.c comgt_helpers.h
	cc comgt_helpers.c $(CFLAGS) -c



