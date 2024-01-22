#include <stdio.h>
#include "include/dbus.h"
#include "include/locker.h"
#include "include/api_requests.h"


char *BUNDLE_PATH = "/data/bundle.raucb";

//"https://147.135.129.16:9000/test-nico/bundle.raucb?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=1OYDA48647FG8WHQTB5U%2F20240121%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20240121T103357Z&X-Amz-Expires=43200&X-Amz-Security-Token=eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJhY2Nlc3NLZXkiOiIxT1lEQTQ4NjQ3Rkc4V0hRVEI1VSIsImV4cCI6MTcwNTg3NjQxNywicGFyZW50Ijoicm9vdElzUm9vdCJ9._pxZGHpw6mqPYg-Qe96AEIJpNsQlMpPUaANaBkQR4q0wdfIf5ONPCiQMV_5BFqQl9DZolJSPUeVIE-oo6ic0DQ&X-Amz-SignedHeaders=host&versionId=null&X-Amz-Signature=08cf723dd5ae7a0c3c77f933256979fa154ad32c0faab21352615818bac46ee6"


int main()
{
    /*
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
        //faire un fichier error pour faire un gestion erreur propre
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
    */

    gchar* l_cJwtToken = api_post_device_signin();
    gchar* l_url = poll_for_updates(l_cJwtToken);
    printf("url : %s\n", l_url);
    g_free(l_url);
    return 0;
}