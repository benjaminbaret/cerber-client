#include "api_requests_test.h"
#include "locker_test.h"

// Programme principal
int main(void)
{
    SRunner *sr;
    int nombre_erreurs;

    sr = srunner_create(api_requests_suite());

    srunner_add_suite(sr, locker_suite());

    srunner_run_all(sr, CK_NORMAL);
    nombre_erreurs = srunner_ntests_failed(sr);

    srunner_free(sr);

    return (nombre_erreurs == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
