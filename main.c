#include "rauc-installer-generated.h"


int main(void)
{

    // Create a proxy to the RAUC Installer D-Bus service
    GError *ierror = NULL;

    GVariant *slot_status_array;
    RInstaller *proxy = r_installer_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
			G_DBUS_PROXY_FLAGS_GET_INVALIDATED_PROPERTIES,
			"de.pengutronix.rauc", "/", NULL, &ierror);
        
    // Call the GetSlotStatus method
    if(!r_installer_call_get_slot_status_sync(proxy, &slot_status_array, NULL, &ierror))
    {
        g_print("Error: %s\n", ierror->message);
    }
    else
    {
        g_print("Slot status: %s\n", g_variant_print(slot_status_array, TRUE));
    }
    

    return 0;
}