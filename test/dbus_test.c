#include <locale.h>
#include <glib.h>
#include "../src/dbus_service.c"

static void test_slot_status()
{
    GError *error = NULL;
    gboolean res = FALSE;

    RInstaller *installer = r_installer_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
			G_DBUS_PROXY_FLAGS_GET_INVALIDATED_PROPERTIES,
			"de.pengutronix.rauc", "/", NULL, &error);

    g_assert_no_error(error);
    g_assert_nonnull(installer);

    res = slot_status(installer, &error);

    g_assert_no_error(error);
    g_assert_true(res);
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "C");

    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/main/slot_status", test_slot_status);

    return g_test_run();
}