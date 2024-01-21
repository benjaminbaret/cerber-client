#include <stdio.h>
#include "include/dbus.h"

char *BUNDLE_PATH = "/data/bundle.raucb";

//"https://147.135.129.16:9000/test-nico/bundle.raucb?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=1OYDA48647FG8WHQTB5U%2F20240121%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20240121T103357Z&X-Amz-Expires=43200&X-Amz-Security-Token=eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJhY2Nlc3NLZXkiOiIxT1lEQTQ4NjQ3Rkc4V0hRVEI1VSIsImV4cCI6MTcwNTg3NjQxNywicGFyZW50Ijoicm9vdElzUm9vdCJ9._pxZGHpw6mqPYg-Qe96AEIJpNsQlMpPUaANaBkQR4q0wdfIf5ONPCiQMV_5BFqQl9DZolJSPUeVIE-oo6ic0DQ&X-Amz-SignedHeaders=host&versionId=null&X-Amz-Signature=08cf723dd5ae7a0c3c77f933256979fa154ad32c0faab21352615818bac46ee6"


int checkIfFileExists(const char *filename)
{
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return 1;
    }
 
    return 0;
} 

int readLockBoot(gchar *slotName)
{
    FILE *lockBoot = fopen("/data/boot.txt", "r");
    if (lockBoot == NULL)
    {
        g_print("Erreur ouverture fichier boot");
    }
    else
    {
        char c = fgetc(lockBoot);
        if (strcmp(slotName, "rootfs.0") == 0)
        {
            if (c == 'A')
            {
                printf("Pas de probleme durant le boot");
                return 0;
            }
            else if (c == 'B')
            {
                printf("Le fichier boot existe");
                printf("probleme durant le boot");
                return 1;
            }
            else
            {
                printf("Partition inconnue");
                return 2;
            }
        }
        else if(strcmp(slotName, "rootfs.1") == 0)
        {
            if (c == 'B')
            {
                printf("Pas de probleme durant le boot");
                return 0;
            }
            else if (c == 'A')
            {
                printf("Le fichier boot existe");
                printf("probleme durant le boot");
                return 1;
            }
            else
            {
                printf("Partition inconnue");
                return 2;
            }
        }
    }
    fclose(lockBoot);
}

int writeLockBoot(gchar *slotName)
{
    FILE *lockBoot = fopen("/data/boot.txt", "w");
    if (lockBoot == NULL)
    {
        g_print("Erreur ouverture fichier boot");
    }
    else
    {
        if (strcmp(slotName, "rootfs.0") == 0)
        {
            fprintf(lockBoot, "B");
        }
        else if (strcmp(slotName, "rootfs.1") == 0)
        {
            fprintf(lockBoot, "A");
        }
    }
    fclose(lockBoot);
}

int removeLockFile(gchar* filename){
    if (remove(filename) == 0)
    {
        printf("Le fichier %s a ete supprime avec succes\n", filename);
        return 0;
    }
    else
    {
        printf("Le fichier %s n'a pas ete supprime\n", filename);
        return 1;
    }

}

int main()
{

    progressBundle progress;
    gchar *slotName;

    GError *error = NULL;
    GDBusConnection *connection = createConnection();
    if (connection == NULL)
    {
        return 1;
    }

    slotName = getSlot(connection, error);

    // Check if lock for update exists
    if (checkIfFileExists("/data/rootfs.txt"))
    {
        printf("Le fichier rootfs existe \n");
        printf("probleme durant la mise a jour\n");
    }
    else
    {
        printf("Le fichier rootfs n'existe pas\n");
        printf("pas de probleme durant la mise a jour\n");
    }

    // Check if lock for boot exist and if it's the right partition

    if (checkIfFileExists("/data/boot.txt"))
    {
        readLockBoot(slotName);
        removeLockFile("/data/boot.txt");

    }
    else
    {
        printf("Le fichier boot n'existe pas\n");
    }

    

    // Create a proxy for the bundle installer
    GDBusProxy *proxyBundle = createProxy(connection, "de.pengutronix.rauc", "/", "de.pengutronix.rauc.Installer", error);
    if (proxyBundle == NULL)
    {
        return 1;
    }

    // Call the InstallBundle method
    FILE *lockRootfs = fopen("/data/rootfs.txt", "w");
    fclose(lockRootfs);
    GVariant *bundle = installBundle(proxyBundle, BUNDLE_PATH,error);
    if (bundle == NULL)
    {
        return 1;
    }

    sleep(1);

    g_object_unref(proxyBundle);

    while (1)
    {
        progress = getProgress(connection, error);
        g_print("Progress: %d%% - %s (Nesting Depth: %d)\n", progress.pourcentage, progress.message, progress.nesting_depth);

        if (progress.pourcentage == 100)
        {

            break;
        }
        sleep(1);
    }

    writeLockBoot(slotName);

    g_print("Installation complete\n");

    removeLockFile("/data/rootfs.txt");

    sleep(5);

    g_object_unref(connection);

    system("reboot");
    

    return 0;
}