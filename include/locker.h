#ifndef LOCKER_H
#define LOCKER_H

#include <glib.h>


gint checkIfFileExists(const char *filename);

gint readLockBoot(gchar *slotName);

gint writeLockBoot(gchar *slotName);

gint removeLockFile(gchar* filename);

#endif // LOCKER_H