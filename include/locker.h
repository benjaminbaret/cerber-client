#ifndef LOCKER_H
#define LOCKER_H

#include <glib.h>


gint checkIfFileExists(const char *p_filename);

gint readLockBoot(gchar *p_slotName);

gint writeLockBoot(gchar *p_slotName);

gint removeLockFile(gchar* p_filename);

#endif // LOCKER_H