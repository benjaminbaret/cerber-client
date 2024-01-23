#ifndef ERRORS_CODE_H
#define ERRORS_CODE_H

#include <glib.h>

// Définition des codes d'erreur
typedef enum {
    ERROR_NONE = 0,
    ERROR_DURING_DOWNLOAD_UPDATE,
    ERROR_CREATION_DBUS_CONNECTION,
    ERROR_CREATION_DBUS_PROXY,
    ERROR_CREATION_DBUS_VARIANT,
    ERROR_DURING_REBOOT_SLOT_FILE,
    ERROR_DURING_WRITE_LOCK_BOOT,
    ERROR_DURING_READ_LOCK_BOOT,
    ERROR_DURING_REBOOT,
    ERROR_DURING_REMOVE_LOCK_FILE,
    ERROR_DURING_POLL_FOR_UPDATES,
    ERROR_FILE_NOT_FOUND,
    ERROR_DURING_INSTALLATION,


    // Ajoutez d'autres codes d'erreur selon vos besoins
} ErrorCode;

// Fonction pour obtenir une description d'erreur
const gchar* getErrorMessage(ErrorCode code);

#endif // ERRORS_CODE_H