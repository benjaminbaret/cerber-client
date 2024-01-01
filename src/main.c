#include "dbus_service.c"

// remove me later : 
#include "rauc-installer-generated.h"


int main(void)
{
    
    GError *error = NULL;

    RInstaller *installer = r_installer_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
			G_DBUS_PROXY_FLAGS_GET_INVALIDATED_PROPERTIES,
			"de.pengutronix.rauc", "/", NULL, &error);

    if(!installer)
        goto error;

    slot_status(installer, &error);

error:
    if(error)
    {
        g_print("Error : %d\n", error->code);
    } 
    
    return 0;
}