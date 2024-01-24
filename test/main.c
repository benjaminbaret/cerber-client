#include "api_requests_test.c"

int main(int argc, char* argv)
{
    setlocale(LC_ALL, "C");

    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/api_requests_test/api_patch_progress_http_ok", api_patch_progress_http_ok);
    g_test_add_func("/api_requests_test/api_patch_progress_http_unauthorized", api_patch_progress_http_unauthorized);

    return g_test_run();
}