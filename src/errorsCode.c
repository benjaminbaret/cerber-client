#include <glib.h>
#include "errorsCode.h"


const gchar* getErrorMessage(ErrorCode code)
{
    switch (code)
    {
    case 0:
        return "No error";
    case 1:
        return "An error occured code : Error during downloading/installing update";
    }
}
