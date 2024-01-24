#include <stdio.h>
#include "dbus.h"
#include "locker.h"
#include "api_requests.h"
#include "errorsCode.h"

char *BUNDLE_PATH = "/data/bundle.raucb";

int main()
{
    ErrorCode errorCode = ERROR_NONE;
    progressBundle progress;
    gchar *l_slotName;
    gchar *l_cJwtToken;
    GError *error = NULL;
    gchar *l_cDeviceStatus;
    gchar* l_cUpdateStatus;
    gchar *l_cPourcentage;
    gchar *l_cLastError;

    l_cJwtToken = api_post_device_signin();
    l_cDeviceStatus = "online";
    api_patch_device_status(l_cJwtToken, l_cDeviceStatus);

    GDBusConnection *connection = createConnection();
    if (connection == NULL)
    {
        errorCode = ERROR_CREATION_DBUS_CONNECTION;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
        return 1;
    }

    GDBusProxy *proxy = createProxy(connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", error);
    if (proxy == NULL)
    {
        errorCode = ERROR_CREATION_DBUS_PROXY;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
    }

    l_slotName = getSlot(connection, error);

    if (checkIfFileExists("/data/rootfs.txt"))
    {
        errorCode = ERROR_DURING_DOWNLOAD_UPDATE;
        g_warning(" An error occured : %s", getErrorMessage(errorCode));
    }
    else
    {
        g_message("No error during download update");
    }

    if (checkIfFileExists("/data/boot.txt"))
    {
        gint l_iBoot = 0;
        gint l_iRemoveLockFile = 0;
        l_iBoot = readLockBoot(l_slotName, "/data/boot.txt");


        switch (l_iBoot)
        {
        case 0:
            g_message("No error during boot");
            l_cUpdateStatus = "Done";
            api_patch_update_status(l_cJwtToken, l_cUpdateStatus);

            break;
        case 1:
            errorCode = ERROR_DURING_REBOOT;
            g_warning("An error occured : %s", getErrorMessage(errorCode));
            break;
        case 2:
            errorCode = ERROR_UNKNOW_SLOT_BOOT_FILE;
            g_warning("An error occured : %s", getErrorMessage(errorCode));
            break;
        default:
            errorCode = ERROR_FILE_NOT_FOUND;
            g_warning("An error occured : %s", getErrorMessage(errorCode));
            break;
        }


        l_iRemoveLockFile = removeLockFile("/data/boot.txt");
        switch (l_iRemoveLockFile)
        {
        case 0:
            g_message("No error during remove lock file");
            break;
        case 1:
            errorCode = ERROR_DURING_REMOVE_LOCK_FILE;
            g_warning("An error occured : %s", getErrorMessage(errorCode));
            break;
        }
    }

    else
    {
        g_message("No update before reboot");
    }

    do
    {
        gchar *l_url = poll_for_updates(l_cJwtToken);
        l_url = "http://nicoals/coucou";

        // Create a proxy for the bundle installer
        GDBusProxy *proxyBundle = createProxy(connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", error);
        if (proxyBundle == NULL)
        {
            errorCode = ERROR_CREATION_DBUS_PROXY;
            g_warning("An error occured : %s", getErrorMessage(errorCode));
        }

        FILE *lockRootfs = fopen("/data/rootfs.txt", "w");
        fclose(lockRootfs);

        GVariant *bundle = installBundle(proxyBundle, l_url, error);
        if (bundle == NULL)
        {
            errorCode = ERROR_CREATION_DBUS_VARIANT;
            g_warning("An error occured : %s", getErrorMessage(errorCode));
        }
        else
        {
            l_cUpdateStatus = "Triggered";
            api_patch_update_status(l_cJwtToken, l_cUpdateStatus);
        }
        sleep(3);
        g_object_unref(proxyBundle);

        while (1)
        {
            api_patch_update_status(l_cJwtToken, l_cUpdateStatus);

            progress = getProgress(connection, error);
            l_cPourcentage = g_strdup_printf("%d", progress.pourcentage);
            api_patch_progress(l_cJwtToken, l_cPourcentage);

            l_cLastError = getLastError(connection, error);

            if (g_strcmp0(l_cLastError, "") != 0)
            {
                errorCode = ERROR_DURING_INSTALLATION;
                g_warning("An error occured : %s", l_cLastError);
                removeLockFile("/data/rootfs.txt");

                break;
            }

            if (progress.pourcentage == 100)
            {
                break;
            }
            usleep(500000);
        }

    } while (g_strcmp0(l_cLastError, "") != 0);

    gint l_iWriteLockBoot = 0;
    l_iWriteLockBoot = writeLockBoot(l_slotName, "/data/boot.txt");


    switch (l_iWriteLockBoot)
    {
    case 0:
        g_message("No error during write lock boot");
        break;
    case 1:
        errorCode = ERROR_DURING_WRITE_LOCK_BOOT;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
        break;
    case 2: 
        errorCode = ERROR_FILE_NOT_FOUND;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
        break;
    }

    g_print("Installation complete\n");

    gint l_iRemoveLockFile = 0;
    l_iRemoveLockFile = removeLockFile("/data/rootfs.txt");
    switch (l_iRemoveLockFile)
    {
    case 0:
        g_message("No error during remove lock file");
        break;
    case 1:
        errorCode = ERROR_DURING_REMOVE_LOCK_FILE;
        g_warning("An error occured : %s", getErrorMessage(errorCode));
        break;
    }

    l_cUpdateStatus = "rebooting";
    api_patch_update_status(l_cJwtToken, l_cUpdateStatus);

    sleep(5);

    g_object_unref(proxy);
    g_object_unref(connection);
   
   system("reboot");

    return 0;
}