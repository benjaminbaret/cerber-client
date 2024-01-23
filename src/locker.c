#include "locker.h"
#include <stdio.h>

/**
 * @brief Check if a file exists
 * @param p_filename : The file to check
 * @return gint : 1 if the file exists, 0 if not
*/
gint checkIfFileExists(const char *p_filename)
{
    FILE *file;
    if (file = fopen(p_filename, "r"))
    {
        fclose(file);
        return 1;
    }

    return 0;
}

/**
 * @brief Read the lock boot file
 * @param p_slotName : The slot name actualy booted
 * @return gint : The compare result
*/
gint readLockBoot(gchar *p_slotName)
{
    FILE *l_fLockBoot = fopen("/data/boot.txt", "r");
    if (l_fLockBoot == NULL)
    {
        return 3;
    }
    else
    {
        char c = fgetc(l_fLockBoot);
        if (strcmp(p_slotName, "rootfs.0") == 0)
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
        else if (strcmp(p_slotName, "rootfs.1") == 0)
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
        else{ // slotName is not rootfs.0 or rootfs.1 (should not happen)
            return 4;
        }
    }
    fclose(l_fLockBoot);
}

/**
 * @brief Write the futur boot slot in the lock boot file
 * @param p_slotName : The slot name actualy booted
 * @return gint : 0 if no error, else error code
*/
gint writeLockBoot(gchar *p_slotName)
{
    FILE *l_fLockBoot = fopen("/data/boot.txt", "w");
    if (l_fLockBoot == NULL)
    {
        return 2;
    }
    else
    {
        if (strcmp(p_slotName, "rootfs.0") == 0)
        {
            if (fprintf(l_fLockBoot, "B") < 0)
            {
                return 1;
            }
        }
        else if (strcmp(p_slotName, "rootfs.1") == 0)
        {
            if (fprintf(l_fLockBoot, "A") < 0)
            {
                return 1;
            }
        }
        else{ // slotName is not rootfs.0 or rootfs.1 (should not happen)
            return 3;
        }
    }
    fclose(l_fLockBoot);
    return 0;
}

/**
 * @brief Delete a file
 * @param p_slotName : The slot name actualy booted
 * @return gint : 0 if no error, else error code
*/
int removeLockFile(gchar *p_filename)
{
    return (remove(p_filename) == 0) ? 0 : 1;
}