#include "api_requests_test.h"
/**
 * @brief Test the function api_post_device_signin
*/
START_TEST(test_api_post_device_signin)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");
    g_free(jwtToken);
}
END_TEST


/**
 * @brief Test the function api_patch_device_status
*/
START_TEST(test_api_patch_device_status)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");

    glong http_code = api_patch_device_status(jwtToken, "new_status");
    ck_assert_msg(http_code == 200, " Failed.");

    g_free(jwtToken);
}
END_TEST


/**
 * @brief Test the function api_patch_device_status http code 401
*/
START_TEST(test_api_patch_device_status_http_code_401)
{
    gchar *jwtToken = "fake_token";
    glong http_code = api_patch_device_status(jwtToken, "new_status");
    ck_assert_msg(http_code == 401, " Failed.");

}
END_TEST


/**
 * @brief Test the function api_patch_update_status
*/
START_TEST(test_api_patch_update_status)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");

    glong http_code = api_patch_update_status(jwtToken, "OK");
    ck_assert_msg(http_code == 200, " Failed.");

    g_free(jwtToken);
}
END_TEST


/**
 * @brief Test the function api_patch_update_status http code 401
*/
START_TEST(test_api_patch_update_status_http_code_401)
{
    gchar *jwtToken = "fake_token";

    glong http_code = api_patch_update_status(jwtToken, "OK");
    ck_assert_msg(http_code == 401, " Failed.");

}
END_TEST


/**
 * @brief Test the function api_patch_progress
*/
START_TEST(test_api_patch_progress)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");

    glong http_code = api_patch_progress(jwtToken, "50");
    ck_assert_msg(http_code == 200, " Failed.");

    g_free(jwtToken);
}
END_TEST


/**
 * @brief Test the function api_patch_progress http code 401
*/
START_TEST(test_api_patch_progress_http_code_401)
{
    gchar *jwtToken = "fake_token";

    glong http_code = api_patch_progress(jwtToken, "50");
    ck_assert_msg(http_code == 401, " Failed.");

}
END_TEST


/**
 * @brief Test the function api_get_update_next
*/
START_TEST(test_api_get_update_next_ok)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");

    http* l_http = api_get_update_next(jwtToken);
    ck_assert_msg(g_strcmp0(l_http->body,"") != 0 , " Failed.");   
}


/**
 * @brief Test the function api_get_update_next http code 401
*/
START_TEST(test_api_get_update_next_401)
{
    gchar *jwtToken = "";

    http* l_http = api_get_update_next(jwtToken);
    ck_assert_msg(g_strcmp0(l_http->body,"") == 0 , " Failed.");
    free(l_http);
}


/**
 * @brief Test the function api_patch
*/
START_TEST(test_api_patch_ok)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");

    gchar *route = "/device/updateStatus";
    gchar *body = "{\"updateStatus\":\"OK\"}";

    glong http_code = api_patch(route, jwtToken, body);
    ck_assert_msg(http_code == 200, " Failed.");
}


/**
 * @brief Test the function api_patch http code 401
*/
START_TEST(test_api_patch_http_code_401)
{
    gchar *jwtToken = "fake_token";

    gchar *route = "/device/updateStatus";
    gchar *body = "{\"updateStatus\":\"OK\"}";

    glong http_code = api_patch(route, jwtToken, body);
    ck_assert_msg(http_code == 401, " Failed.");

}
END_TEST

/**
 * @brief Test the function api_patch http code 400 because of the body
*/
START_TEST(test_api_patch_http_code_400_body)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");

    gchar *route = "/device/updateStatus";
    gchar *body = "{\"updateStatus\": Bad_body }";

    glong http_code = api_patch(route, jwtToken, body);
    ck_assert_msg(http_code == 400, " Failed.");

}
END_TEST


/**
 * @brief Test the function api_patch http code 400 beacause of the param
*/
START_TEST(test_api_patch_http_code_400_param)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");

    gchar *route = "/device/updateStatus";
    gchar *body = "{\"updateStatu\": Bad_body }";

    glong http_code = api_patch(route, jwtToken, body);
    ck_assert_msg(http_code == 400, " Failed.");

}
END_TEST


/**
 * @brief Test the function poll_for_updates
*/
START_TEST(test_poll_for_updates)
{
    gchar *jwtToken = api_post_device_signin();
    ck_assert_msg(jwtToken != NULL, " Failed.");

    http* l_http = api_get_update_next(jwtToken);
    ck_assert_msg(g_strcmp0(l_http->body, "https://147.135.129.16:9000/test-nico/bundle.raucb?X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=7YG0FY882R9MK9GGP3NL%2F20240123%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20240123T214517Z&X-Amz-Expires=43200&X-Amz-Security-Token=eyJhbGciOiJIUzUxMiIsInR5cCI6IkpXVCJ9.eyJhY2Nlc3NLZXkiOiI3WUcwRlk4ODJSOU1LOUdHUDNOTCIsImV4cCI6MTcwNjA4OTM4NywicGFyZW50Ijoicm9vdElzUm9vdCJ9.9T5asO3izoJASWtrNOzBf4Xman8KdQ5TkliQYOm3h_JycjSlfttys9xpzMbFYF524XPYk8NXPzJHfAmFW_3qFQ&X-Amz-SignedHeaders=host&versionId=null&X-Amz-Signature=701efcd5976c5bcb9d3034cb6b49af19839eacaf0c44680634e58c2df7b75fd1")!= 0," Failed."); // Change for the real url
}


/**
 * @brief Suits of tests for the api_requests module
*/
Suite *api_requests_suite(void)
{
    Suite *s;
    TCase *tc;

    s = suite_create("API_Requests");

    // Testez la fonction api_post_device_signin
    tc = tcase_create("");
    tcase_add_test(tc, test_api_post_device_signin);
    suite_add_tcase(s, tc);

    // Testez la fonction api_patch_device_status
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_device_status);
    suite_add_tcase(s, tc);

    // Testez la fonction api_patch_device_status http code 401
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_device_status_http_code_401);
    suite_add_tcase(s, tc);


    // Test the function api_patch_update_status
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_update_status);
    suite_add_tcase(s, tc);

    // Test the function api_patch_update_status http code 401
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_update_status_http_code_401);
    suite_add_tcase(s, tc);

    // Test the function api_patch_progress
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_progress);
    suite_add_tcase(s, tc);

    // Test the function api_patch_progress http code 401
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_progress_http_code_401);
    suite_add_tcase(s, tc);

    // Test the function api_get_update_next
    tc = tcase_create("");
    tcase_add_test(tc, test_api_get_update_next_ok);
    suite_add_tcase(s, tc);

    // Test the function api_get_update_next http code 401
    tc = tcase_create("");
    tcase_add_test(tc, test_api_get_update_next_401);
    suite_add_tcase(s, tc);

    // Test the function api_patch
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_ok);
    suite_add_tcase(s, tc);

    // Test the function api_patch http code 401
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_http_code_401);
    suite_add_tcase(s, tc);

    // Test the function api_patch http code 403 body
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_http_code_400_body);
    suite_add_tcase(s, tc);

    // Test the function api_patch http code 403 param
    tc = tcase_create("");
    tcase_add_test(tc, test_api_patch_http_code_400_param);
    suite_add_tcase(s, tc);

    // Test the function poll_for_updates
    tc = tcase_create("");
    tcase_add_test(tc, test_poll_for_updates);
    suite_add_tcase(s, tc);
    
    return s;
}