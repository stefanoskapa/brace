CC = gcc
CFLAGS = -Wall -Wextra -O2
VERSION := $(shell git describe --tags --always --dirty 2>/dev/null || echo "dev")
CFLAGS += -DVERSION=\"$(VERSION)\"

PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
DESTDIR ?=

TARGET = brace
SRC = brace.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

install: $(TARGET)
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 $(TARGET) $(DESTDIR)$(BINDIR)/$(TARGET)

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET)

test: brace
	tests/run.sh

.PHONY: all install uninstall clean test
