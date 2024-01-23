#include <glib.h>
#include "errorsCode.h"

/**
 * @brief give the error message
 * @param p_code : Error code
 * @return gchar* : The error message
*/
const gchar *getErrorMessage(ErrorCode p_code)
{
    switch (p_code)
    {
    case 0:
        return "No error";
    case 1:
        return "Error during downloading/installing update";
    case 2:
        return "Error during creation dbus connection";
    case 3:
        return "Error during creation dbus proxy";
    case 4:
        return "Error during creation dbus variant";
    case 5:
        return "Error unknow slot boot file";
    case 6:
        return "Error during write lock boot";
    case 7:
        return "Error during read lock boot";
    case 8:
        return "Error during reboot";
    case 9:
        return "Error during remove lock file";
    case 10:
        return "Error during poll for updates";
    case 11:
        return "Error file not found";
    case 12:
        return "Error during installation";
    case 13:
        return "Error bad partition name rootfs";
    default:
        return "Unknown error";
    }
}
