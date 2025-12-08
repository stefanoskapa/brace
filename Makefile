CC = gcc
CFLAGS = -Wall -o2
VERSION := $(shell git describe --tags --always --dirty 2>/dev/null || echo "dev")
CFLAGS += -DVERSION=\"$(VERSION)\"

brace: brace.c
	$(CC) $(CFLAGS) -o $@ brace.c

clean:
	rm -f brace

test: brace
	tests/run.sh
