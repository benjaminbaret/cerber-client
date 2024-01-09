#include "dbus_service.c"
#include "http.c"

// remove me later : 
#include "rauc-installer-generated.h"


int main(void)
{
    if(testGetHttp() == 0)
    {
        g_print("Nothing to do\n");
    }
    else
    {
        g_print("Sending slot-status");
        
        GError *error = NULL;

        RInstaller *installer = r_installer_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                G_DBUS_PROXY_FLAGS_GET_INVALIDATED_PROPERTIES,
                "de.pengutronix.rauc", "/", NULL, &error);

        if(!installer)
            goto error;

        char *c = slot_status_string(installer, &error);
        testPostHttp(c);
    

    error:
        if(error)
        {
            g_print("Error : %d\n", error->code);
        } 
    }
    
    return 0;
}