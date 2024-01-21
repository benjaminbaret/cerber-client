#ifndef DBUSLIB_H
#define DBUSLIB_H

#include <gio/gio.h>

// Structure for the progress property value of the current update
typedef struct {
    gint pourcentage, nesting_depth;
    gchar *message;
} progressBundle;

// Connection to the DBus
GDBusConnection* createConnection();

// Proxy for the RAUC installer
GDBusProxy* createProxy(GDBusConnection *connection, gchar *name, gchar *path, gchar *interface, GError *error);

// Variant for the progress property value of the current update
GVariant* variantgetProgress(GDBusProxy *proxy, GError *error);

// InstallBundle method of the RAUC installer
GVariant* installBundle(GDBusProxy *proxy, gchar *bundlePath, GError *error);


// GetSlotInfo method of the RAUC installer
GVariant* variantgetSlotInfo(GDBusProxy *proxy, GError *error);

// GetProgress method of the RAUC installer
progressBundle getProgress(GDBusConnection *connection, GError *error);

// Current slot booting
gchar* getSlot(GDBusConnection *connection, GError *error);

#endif // DBUSLIB_H
