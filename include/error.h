#ifndef ERRORS_H
#define ERRORS_H

#include <glib.h>

// Définition des codes d'erreur
typedef enum {
    ERROR_NONE = 0,
    ERROR_DURING_DOWNLOAD_UPDATE,
    ERROR_CREATION_DBUS_CONNECTION,
    ERROR_CREATION_DBUS_PROXY,

    // Ajoutez d'autres codes d'erreur selon vos besoins
} ErrorCode;

// Fonction pour obtenir une description d'erreur
const gchar* getErrorMessage(ErrorCode code);

#endif // ERRORS_H