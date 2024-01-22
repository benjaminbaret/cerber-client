#ifndef LOCKER_H
#define LOCKER_H

#include <glib.h>


int checkIfFileExists(const char *filename);

int readLockBoot(gchar *slotName);

int writeLockBoot(gchar *slotName);

int removeLockFile(gchar* filename);

#endif // LOCKER_H