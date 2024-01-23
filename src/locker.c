#include "locker.h"
#include <stdio.h>

gint checkIfFileExists(const char *filename)
{
    FILE *file;
    if (file = fopen(filename, "r"))
    {
        fclose(file);
        return 1;
    }

    return 0;
}

gint readLockBoot(gchar *slotName)
{
    FILE *lockBoot = fopen("/data/boot.txt", "r");
    if (lockBoot == NULL)
    {
        return 3;
    }
    else
    {
        char c = fgetc(lockBoot);
        if (strcmp(slotName, "rootfs.0") == 0)
        {
            if (c == 'A')
            {
                return 0;
            }
            else if (c == 'B')
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
        else if (strcmp(slotName, "rootfs.1") == 0)
        {
            if (c == 'B')
            {
                return 0;
            }
            else if (c == 'A')
            {
                return 1;
            }
            else
            {
                return 2;
            }
        }
    }
    fclose(lockBoot);
}

gint writeLockBoot(gchar *slotName)
{
    FILE *lockBoot = fopen("/data/boot.txt", "w");
    if (lockBoot == NULL)
    {
        return 2;
    }
    else
    {
        if (strcmp(slotName, "rootfs.0") == 0)
        {
            if (fprintf(lockBoot, "B") < 0)
            {
                return 1;
            }
        }
        else if (strcmp(slotName, "rootfs.1") == 0)
        {
            if (fprintf(lockBoot, "A") < 0)
            {
                return 1;
            }
        }
    }
    fclose(lockBoot);
    return 0;
}

int removeLockFile(gchar *filename)
{
    return (remove(filename) == 0) ? 0 : 1;
}