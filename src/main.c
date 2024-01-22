#include <stdio.h>
#include "dbus.h"
#include "locker.h"
#include "api_requests.h"
#include "errorsCode.h"


char *BUNDLE_PATH = "/data/bundle.raucb";

//"https://147.135.129.16:9000/test-nico/bundle.raucb?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=1OYDA48647FG8WHQTB5U%2F20240121%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20240121T103357Z&X-Amz-Expires=43200&X-Amz-Security-Token=eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJhY2Nlc3NLZXkiOiIxT1lEQTQ4NjQ3Rkc4V0hRVEI1VSIsImV4cCI6MTcwNTg3NjQxNywicGFyZW50Ijoicm9vdElzUm9vdCJ9._pxZGHpw6mqPYg-Qe96AEIJpNsQlMpPUaANaBkQR4q0wdfIf5ONPCiQMV_5BFqQl9DZolJSPUeVIE-oo6ic0DQ&X-Amz-SignedHeaders=host&versionId=null&X-Amz-Signature=08cf723dd5ae7a0c3c77f933256979fa154ad32c0faab21352615818bac46ee6"


int main()
{
    ErrorCode errorCode = ERROR_NONE;
    progressBundle progress;
    gchar *slotName;
    gchar* l_cJwtToken;
    GError *error = NULL;
    gchar* l_cstatus;

    
    l_cJwtToken = api_post_device_signin();
    l_cstatus = "online";
    api_patch_status(l_cJwtToken, l_cstatus);



    GDBusConnection *connection = createConnection();
    if (connection == NULL)
    {
        return 1;
    }
    slotName = getSlot(connection, error);

    if (checkIfFileExists("/data/rootfs.txt"))
    {
        //faire un fichier error pour faire un gestion erreur propre
        g_message("Probleme durant la mise a jour");
        errorCode = ERROR_DURING_DOWNLOAD_UPDATE;
        g_warning(" An error occured : %s", getErrorMessage(errorCode));
    }
    else
    {
    }


    if (checkIfFileExists("/data/boot.txt"))
    {
        readLockBoot(slotName);
        removeLockFile("/data/boot.txt");

    }
    else
    {
        errorCode = ERROR_DURING_REBOOT;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
    }


    gchar* l_url = poll_for_updates(l_cJwtToken);



     // Create a proxy for the bundle installer
    GDBusProxy *proxyBundle = createProxy(connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", error);
    if (proxyBundle == NULL)
    {
        errorCode = ERROR_CREATION_DBUS_PROXY;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
    }

    // Call the InstallBundle method
    FILE *lockRootfs = fopen("/data/rootfs.txt", "w");
    fclose(lockRootfs);
    GVariant *bundle = installBundle(proxyBundle,l_url, error);
    if (bundle == NULL)
    {
        errorCode = ERROR_CREATION_DBUS_VARIANT;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
    }else{
        l_cstatus = "Triggered";
        api_patch_status(l_cJwtToken, l_cstatus);
    }
    g_object_unref(proxyBundle);


    while (1)
    {
        api_patch_status(l_cJwtToken, l_cstatus);
        progress = getProgress(connection, error);
        g_print("Progress: %d%% - %s (Nesting Depth: %d)\n", progress.pourcentage, progress.message, progress.nesting_depth);

        if (progress.pourcentage == 100)
        {
            break;
        }
        usleep(100000);
    }

    writeLockBoot(slotName);

    g_print("Installation complete\n");

    removeLockFile("/data/rootfs.txt");

    l_cstatus = "rebooting";
    api_patch_status(l_cJwtToken, l_cstatus);

    sleep(5);

    g_object_unref(connection);

    return 0;
}