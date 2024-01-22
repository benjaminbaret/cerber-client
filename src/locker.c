#include "include/locker.h"
#include <stdio.h>

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