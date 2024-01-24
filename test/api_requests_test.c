#include <locale.h>
#include "api_requests.h"

static void api_patch_progress_http_ok()
{
    GError *error = NULL;
    gboolean res = FALSE;
    gchar* l_cProgress = "12";

    gchar* l_cJwtToken = api_post_device_signin();
    glong l_lHttpResponse = api_patch_progress(l_cJwtToken, l_cProgress);
    

    g_message("%ld", l_lHttpResponse);

    g_assert_cmpint(l_lHttpResponse, ==, 200);
}

static void api_patch_progress_http_unauthorized()
{
    GError *error = NULL;
    gboolean res = FALSE;
    gchar* l_cProgress = "12";
    gchar* l_cJwtToken = "";

    glong l_lHttpResponse = api_patch_progress(l_cJwtToken, l_cProgress);
    

    g_message("%ld", l_lHttpResponse);
    g_assert_cmpint(l_lHttpResponse, ==, 200);
}

