#include "dbus.h"
#include "errorsCode.h"




/**
 * @brief Create a connection to the D-Bus
 * @return GDBusConnection : TThe connection to the D-Bus
*/
GDBusConnection* createConnection(){
    GError *l_error = NULL;  

    // Connect to the D-Bus
    GDBusConnection *l_connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &l_error);
    if (l_connection == NULL) {
        g_printerr("Error connecting to D-Bus: %s\n", l_error->message);
        g_error_free(l_error);
        return NULL;
    }
    return l_connection;

}


/**
 * @brief Create a proxy for the RAUC installer
 * @param p_connection : The connection to the D-Bus
 * @param p_name : The name of the proxy
 * @param p_path : The path of the proxy
 * @param p_interface : The interface of the proxy
 * @param p_error : The error
 * @return GDBusProxy : The proxy
*/
GDBusProxy* createProxy(GDBusConnection *p_connection, gchar *p_name, gchar *p_path, gchar *p_interface, GError *p_error){

    // Create a proxy for the RAUC installer
    GDBusProxy *l_proxy = g_dbus_proxy_new_sync(
        p_connection,
        G_DBUS_PROXY_FLAGS_NONE,
        NULL,
        p_name,
        p_path,
        p_interface,
        NULL,
        &p_error
    );
    if (l_proxy == NULL) {
        g_printerr("Error creating D-Bus proxy: %s\n", p_error->message);
        g_error_free(p_error);
        g_object_unref(p_connection);
        return NULL;
    }
    return l_proxy;
}

/**
 * @brief Create a variant for progress value
 * @param p_proxy : The proxy
 * @param p_error : The error
 * @return GVariant : The variant
*/
GVariant* variantgetProgress(GDBusProxy *p_proxy, GError *p_error){
    GVariant *l_progressBundle = g_dbus_proxy_get_cached_property(p_proxy, "Progress");
    if (l_progressBundle == NULL) {
        g_printerr("Error getting property value: %s\n", p_error->message);
        g_error_free(p_error);
        g_object_unref(p_proxy);
        return NULL;
    }
    return l_progressBundle;
}

/**
 * @brief Create a variant for last error value
 * @param p_proxy : The proxy
 * @param p_error : The error
 * @return GVariant : The variant
*/
GVariant* variantgetLastError(GDBusProxy *p_proxy, GError *p_error){
    GVariant *l_lastError = g_dbus_proxy_get_cached_property(p_proxy, "LastError");
    if (l_lastError == NULL) {
        g_printerr("Error getting property value: %s\n", p_error->message);
        g_error_free(p_error);
        g_object_unref(p_proxy);
        return NULL;
    }
    return l_lastError;
}

/**
 * @brief Create a variant to install the bundle
 * @param p_proxy : The proxy
 * @param p_bundlePath : The path of the bundle
 * @param p_error : The error
 * @return GVariant : The variant
*/
GVariant* installBundle(GDBusProxy *p_proxy, gchar *p_bundlePath, GError *p_error){
    GVariantDict *l_args_dict = g_variant_dict_new(NULL);
    g_variant_dict_insert(l_args_dict, "tls-no-verify", "b", TRUE);
    GVariant *l_args = g_variant_dict_end(l_args_dict);

    
    GVariant *l_installBundleVariant = g_dbus_proxy_call_sync(
        p_proxy,
        "InstallBundle",  // Method name
        g_variant_new("(s@a{sv})", p_bundlePath, l_args),  // Method parameters
        G_DBUS_CALL_FLAGS_NONE,
        -1,  // Timeout (negative means default timeout)
        NULL,  // GCancellable
        &p_error
    );
    if (l_installBundleVariant == NULL) {
        g_printerr("Error calling InstallBundle: %s\n", p_error->message);
        g_error_free(p_error);
        g_object_unref(p_proxy);
        return NULL;
    }
    return l_installBundleVariant;
}

/**
 * @brief Create a variant to get the slot info
 * @param p_proxy : The proxy
 * @param p_error : The error
 * @return GVariant : The variant
*/
GVariant* variantgetSlotInfo(GDBusProxy *p_proxy, GError *p_error){

    GVariant *l_slotInfo = g_dbus_proxy_call_sync(
        p_proxy,
        "GetPrimary",
        NULL,
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        NULL
    );
    if (l_slotInfo == NULL) {
        g_printerr("Error calling GetPrimary: %s\n", p_error->message);
        g_error_free(p_error);
        g_object_unref(p_proxy);
        return NULL;
    }

    return l_slotInfo;
}

/**
 * @brief Get the progress value
 * @param p_connection : The connection to the D-Bus
 * @param p_error : The error
 * @return progressBundle : The progress struc (pourcentage, message, nesting_depth)
*/
progressBundle getProgress (GDBusConnection *p_connection, GError *p_error){
    ErrorCode errorCode;
    progressBundle l_progress;
    GDBusProxy *l_proxy = createProxy(p_connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", p_error);
    if(l_proxy == NULL){
        errorCode = ERROR_CREATION_DBUS_PROXY;
        g_warning("An error occured : %s", getErrorMessage(errorCode));

    }
    GVariant *l_bundleVariant = variantgetProgress(l_proxy, p_error);
    if(l_bundleVariant == NULL){
        errorCode = ERROR_CREATION_DBUS_VARIANT;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
    }
    g_variant_get(l_bundleVariant, "(isi)", &l_progress.pourcentage, &l_progress.message, &l_progress.nesting_depth);
    g_variant_unref(l_bundleVariant);
    g_object_unref(l_proxy);
    return l_progress;
}

/**
 * @brief Get the last error value
 * @param p_connection : The connection to the D-Bus
 * @param p_error : The error
 * @return gchar* : The last error
*/
gchar* getLastError(GDBusConnection *p_connection, GError *p_error){
    ErrorCode errorCode;
    gchar* l_cLastError;
    GDBusProxy *l_proxy = createProxy(p_connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", p_error);
    if(l_proxy == NULL){
        errorCode = ERROR_CREATION_DBUS_PROXY;
        g_warning("An error occured : %s", getErrorMessage(errorCode));

    }
    GVariant *l_lastErrorVariant = variantgetLastError(l_proxy, p_error);
    if(l_lastErrorVariant == NULL){
        errorCode = ERROR_CREATION_DBUS_VARIANT;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
    }
    g_variant_get(l_lastErrorVariant, "s", &l_cLastError);
    g_variant_unref(l_lastErrorVariant);
    g_object_unref(l_proxy);
    return l_cLastError;

}

/**
 * @brief Get the current slot booting
 * @param p_connection : The connection to the D-Bus
 * @param p_error : The error
 * @return gchar* : The current slot booting
*/
gchar* getSlot(GDBusConnection *p_connection, GError *p_error){
    ErrorCode errorCode = ERROR_NONE;
    gchar *l_cSlotName;
    GDBusProxy *l_proxy = createProxy(p_connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", p_error);
    if(l_proxy == NULL){
        errorCode = ERROR_CREATION_DBUS_PROXY;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
    }
    GVariant *l_slotInfo = variantgetSlotInfo(l_proxy, p_error);
    if(l_slotInfo == NULL){
        errorCode = ERROR_CREATION_DBUS_VARIANT;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
    }
    g_variant_get(l_slotInfo, "(s)", &l_cSlotName);
    g_variant_unref(l_slotInfo);
    g_object_unref(l_proxy);
    return l_cSlotName;
}