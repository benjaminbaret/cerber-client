CFLAGS_DBUS = $(shell pkg-config --cflags --libs dbus-1)
CFLAGS_DBUS_GLIB = $(shell pkg-config --cflags --libs dbus-glib-1)
CFLAGS_GIO  = $(shell pkg-config --cflags --libs gio-2.0)
CFLAGS_GIO_UNIX  = $(shell pkg-config --cflags --libs gio-unix-2.0)

CFLAGS = -g -Wall -Werror -Wl,-dynamic-linker,/lib/ld-linux-x86-64.so.2 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -ldbus-1 -pthread -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lgio-2.0 -lgobject-2.0 -lglib-2.0 -pthread -I/usr/include/gio-unix-2.0 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -lgio-2.0 -lgobject-2.0 -lglib-2.0
CC=gcc
DEPS = rauc-installer-generated.h

all: cerber-client

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(CFLAGS_DBUS) $(CFLAGS_GIO) $(CFLAGS_GIO_UNIX) 

cerber-client: main.o rauc-installer-generated.o 
	$(CC) -o cerber-client main.o rauc-installer-generated.o $(CFLAGS) $(CFLAGS_DBUS) $(CFLAGS_GIO) $(CFLAGS_GIO_UNIX)



.PHONY: clean

clean:
	rm -f cerber-client
	rm -rf *.o