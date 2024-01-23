#ifndef DBUS_H
#define DBUS_H

#include <gio/gio.h>

// Structure for the progress property value of the current update
typedef struct {
    gint pourcentage, nesting_depth;
    gchar *message;
} progressBundle;



// Connection to the DBus
GDBusConnection* createConnection();

// Proxy for the RAUC installer
GDBusProxy* createProxy(GDBusConnection *p_connection, gchar *p_name, gchar *p_path, gchar *p_interface, GError *p_error);

// Variant for the progress property value of the current update
GVariant* variantgetProgress(GDBusProxy *p_proxy, GError *p_error);

GVariant* variantgetLastError(GDBusProxy *p_proxy, GError *p_error);

// InstallBundle method of the RAUC installer
GVariant* installBundle(GDBusProxy *p_proxy, gchar *p_bundlePath, GError *p_error);


// GetSlotInfo method of the RAUC installer
GVariant* variantgetSlotInfo(GDBusProxy *p_proxy, GError *p_error);

// GetProgress method of the RAUC installer
progressBundle getProgress(GDBusConnection *p_connection, GError *p_error);

// Current slot booting
gchar* getSlot(GDBusConnection *p_connection, GError *p_error);

gchar* getLastError(GDBusConnection *p_connection, GError *p_error);

#endif // DBUS_H
