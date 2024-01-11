#include "rauc-installer-generated.h"

/**
* Generate JSON containing status information anout slots and print it to
* stdout
*/
/* static gboolean slot_status(RInstaller *installer, GError **error)
{

    GVariant *slot_status_array = NULL;
    gboolean res = FALSE;

    res = r_installer_call_get_slot_status_sync(installer, &slot_status_array, NULL, error);
    
    if(!res)
        goto out;

    g_print("Slot status: %s\n", g_variant_print(slot_status_array, TRUE));


out:
    return res;
}
 */

// same method as slot_statusm but with a different return type (string here)
/* static char *slot_status_string(RInstaller *installer, GError **error)
{

    GVariant *slot_status_array = NULL;
    gboolean res = FALSE;

    res = r_installer_call_get_slot_status_sync(installer, &slot_status_array, NULL, error);
    
    if(!res)
        goto out;

    return g_variant_print(slot_status_array, TRUE);


out:
    return "0";
} */