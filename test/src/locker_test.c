#include "locker_test.h"


START_TEST(test_check_file_exist_ok)
{
    gint result = checkIfFileExists("../test/file/boot.txt");
    ck_assert_msg(result == 1, "test_check_file_exist_ok failed.");
}
END_TEST


START_TEST(test_check_file_exist_ko)
{
    gint result = checkIfFileExists("test/myFile.c");
    ck_assert_msg(result == 0, "test_check_file_exist_ok failed.");
}

START_TEST(test_read_lock_boot_file_ok)
{
    gint result = readLockBoot("rootfs.0", "../test/file/boot.txt");
    ck_assert_msg(result == 0, "test_read_lock_boot_file_ok failed.");
}

START_TEST(test_read_lock_boot_file_not_found)
{
    gint result = readLockBoot("rootfs.0", "../test/file/noBoot.txt");
    ck_assert_msg(result == 3, "test_read_lock_boot_file_not_found failed.");
}
END_TEST

START_TEST(test_read_lock_boot_file_wrong_slot_name)
{
    gint result = readLockBoot("rootfs.1", "../test/file/boot.txt");
    ck_assert_msg(result == 1 , "test_read_lock_boot_file_not_found failed.");
}
END_TEST

START_TEST(test_read_lock_boot_file_uknown_slot_name_in_file)
{
    gint result = readLockBoot("rootfs.0", "../test/file/wrongBoot.txt");
    ck_assert_msg(result == 2 , "test_read_lock_boot_file_not_found failed.");
}
END_TEST

START_TEST(test_read_lock_boot_file_uknown_slot_name)
{
    gint result = readLockBoot("rootfs.2", "../test/file/wrongBoot.txt");
    ck_assert_msg(result == 4 , "test_read_lock_boot_file_not_found failed.");
}
END_TEST

Suite *locker_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Locker");

    // Test the function test_check_file_exist_ko
    tc_core = tcase_create("check_file_exist_ok");
    tcase_add_test(tc_core, test_check_file_exist_ok);
    suite_add_tcase(s, tc_core);

    // Test the function test_check_file_exist_ko
    tc_core = tcase_create("check_file_exist_ko");
    tcase_add_test(tc_core, test_check_file_exist_ko);
    suite_add_tcase(s, tc_core);



    // Test the function test_read_lock_boot_file_not_found
    tc_core = tcase_create("read_lock_boot_file_not_found");
    tcase_add_test(tc_core, test_read_lock_boot_file_not_found);
    suite_add_tcase(s, tc_core);

    // Test the function test_read_lock_boot_file_ok
    tc_core = tcase_create("read_lock_boot_file_ok");
    tcase_add_test(tc_core, test_read_lock_boot_file_ok);
    suite_add_tcase(s, tc_core);

    // Test the function test_read_lock_boot_file_wrong_slot_name
    tc_core = tcase_create("read_lock_boot_file_wrong_slot_name");
    tcase_add_test(tc_core, test_read_lock_boot_file_wrong_slot_name);
    suite_add_tcase(s, tc_core);

    // Test the function test_read_lock_boot_file_uknown_slot_name_in_file
    tc_core = tcase_create("read_lock_boot_file_uknown_slot_name_in_file");
    tcase_add_test(tc_core, test_read_lock_boot_file_uknown_slot_name_in_file);
    suite_add_tcase(s, tc_core);

    // Test the function test_read_lock_boot_file_uknown_slot_name
    tc_core = tcase_create("read_lock_boot_file_uknown_slot_name");
    tcase_add_test(tc_core, test_read_lock_boot_file_uknown_slot_name);
    suite_add_tcase(s, tc_core);



    return s;
}