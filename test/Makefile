CFLAGS_DBUS = $(shell pkg-config --cflags --libs dbus-1)
CFLAGS_GIO  = $(shell pkg-config --cflags --libs gio-2.0)
CFLAGS_GIO_UNIX  = $(shell pkg-config --cflags --libs gio-unix-2.0)


CFLAGS += -g -Wall -Werror
CFLAGS += $(CFLAGS_DBUS)
CFLAGS += $(CFLAGS_GIO)
CFLAGS += $(CFLAGS_GIO_UNIX)

CC?=gcc
DEPS = rauc-installer-generated.h

all: cerber-client-test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cerber-client-test: dbus_test.o rauc-installer-generated.o
	$(CC) -o cerber-client-test dbus_test.o rauc-installer-generated.o $(CFLAGS)



.PHONY: clean

clean:
	rm -f cerber-client-test
	rm -rf *.o