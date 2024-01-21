#include "include/dbus.h"

/*
 * Create a connection to the D-Bus.
 */
GDBusConnection* createConnection(){
    GError *error = NULL;

    // Connect to the D-Bus
    GDBusConnection *connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (connection == NULL) {
        g_printerr("Error connecting to D-Bus: %s\n", error->message);
        g_error_free(error);
        return NULL;
    }
    return connection;

}

/*
 * Create a proxy for the RAUC installer.
 */
GDBusProxy* createProxy(GDBusConnection *connection, gchar *name, gchar *path, gchar *interface, GError *error){

    // Create a proxy for the RAUC installer
    GDBusProxy *proxy = g_dbus_proxy_new_sync(
        connection,
        G_DBUS_PROXY_FLAGS_NONE,
        NULL,
        name,
        path,
        interface,
        NULL,
        &error
    );
    if (proxy == NULL) {
        g_printerr("Error creating D-Bus proxy: %s\n", error->message);
        g_error_free(error);
        g_object_unref(connection);
        return NULL;
    }
    return proxy;
}

/*
 * Create the variant for the progress property value of the current update.
 */
GVariant* variantgetProgress(GDBusProxy *proxy, GError *error){
    GVariant *progressBundle = g_dbus_proxy_get_cached_property(proxy, "Progress");
    if (progressBundle == NULL) {
        g_printerr("Error getting property value: %s\n", error->message);
        g_error_free(error);
        g_object_unref(proxy);
        return NULL;
    }
    return progressBundle;
}

/*
 * Call the InstallBundle method of the RAUC installer.
 */
GVariant* installBundle(GDBusProxy *proxy, gchar *bundlePath, GError *error){
    GVariant *installBundle = g_dbus_proxy_call_sync(
        proxy,
        "InstallBundle",  // Method name
        g_variant_new("(sa{sv})", bundlePath, NULL),  // Method parameters
        G_DBUS_CALL_FLAGS_NONE,
        -1,  // Timeout (negative means default timeout)
        NULL,  // GCancellable
        &error
    );
    if (installBundle == NULL) {
        g_printerr("Error calling InstallBundle: %s\n", error->message);
        g_error_free(error);
        g_object_unref(proxy);
        return NULL;
    }
    return installBundle;
}

/*
 * Create the variant for the slot info.
 */
GVariant* variantgetSlotInfo(GDBusProxy *proxy, GError *error){

    GVariant *slotInfo = g_dbus_proxy_call_sync(
        proxy,
        "GetPrimary",
        NULL,
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        NULL
    );
    if (slotInfo == NULL) {
        g_printerr("Error calling GetPrimary: %s\n", error->message);
        g_error_free(error);
        g_object_unref(proxy);
        return NULL;
    }

    return slotInfo;
}

/*
 * Get the progress property value of the current update. (pourcentage, message, nesting_depth)
 */
progressBundle getProgress (GDBusConnection *connection, GError *error){
    progressBundle progress;
    GDBusProxy *proxyProgress = createProxy(connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", error);
    if(proxyProgress == NULL){
        // Erreur a traiter
    }
    GVariant *progressBundle = variantgetProgress(proxyProgress, error);
    if(progressBundle == NULL){
        // Erreur a traiter
    }
    g_variant_get(progressBundle, "(isi)", &progress.pourcentage, &progress.message, &progress.nesting_depth);
    g_variant_unref(progressBundle);
    g_object_unref(proxyProgress);
    return progress;
}

/*
 * Get the slot name currently booted . 
 */
gchar* getSlot(GDBusConnection *connection, GError *error){
    gchar *slotName;
    GDBusProxy *proxySlot = createProxy(connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", error);
    if(proxySlot == NULL){
        // Erreur a traiter
    }
    GVariant *slotInfo = variantgetSlotInfo(proxySlot, error);
    if(slotInfo == NULL){
        // Erreur a traiter
    }
    g_variant_get(slotInfo, "(s)", &slotName);
    g_variant_unref(slotInfo);
    g_object_unref(proxySlot);
    return slotName;
}