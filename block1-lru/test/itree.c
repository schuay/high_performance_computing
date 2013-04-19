#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "itree.h"

#define CHK_NODE(n, k1cond, k2cond, vcond, lcond, rcond, hcond) \
do { \
    fail_unless(n != NULL); \
    fail_unless(n->k1 k1cond); \
    fail_unless(n->k2 k2cond); \
    fail_unless(n->v vcond); \
    fail_unless(n->l lcond); \
    fail_unless(n->r rcond); \
    fail_unless(n->h hcond); \
} while(0);

#define TEST_SINGLE_EXTENSION(insert, expected_holes) \
START_TEST(test_insert_single_extension_holes##insert) \
{ \
    itree_t *root = dense_tree_with_interval(3); \
    uint32_t holes; \
    fail_unless(itree_insert(insert, &root, &holes) == 0); \
    fail_unless(holes == expected_holes); \
    itree_free(root); \
} \
END_TEST

/**
 * Creates a dense tree without need for rebalancing with 
 * depth 3 (= 15 nodes). Each node has a singleton interval
 * which is separated from the next closest intervals by
 * the interval argument.
 */
static itree_t *
dense_tree_with_interval(int interval)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(8 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(4 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(12 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(2 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(6 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(10 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(14 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(1 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(3 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(5 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(7 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(9 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(11 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(13 * interval, &root, &holes) == 0);
    fail_unless(itree_insert(15 * interval, &root, &holes) == 0);
    return root;
}

START_TEST(test_iter_next1)
{
    itree_t t1 = { NULL, NULL, 0, 0, 0, 0 };
    itree_t t2 = { NULL, NULL, 0, 0, 0, 0 };
    itree_t t3 = { &t2, NULL, 0, 0, 0, 0 };
    itree_t t4 = { &t1, &t3, 0, 0, 0, 0 };
    itree_t t5 = { NULL, NULL, 0, 0, 0, 0 };
    itree_t t6 = { NULL, &t5, 0, 0, 0, 0 };
    itree_t t7 = { &t4, &t6, 0, 0, 0, 0 };

    itree_iter_t *iter = itree_iter_init(&t7);

    fail_unless(itree_iter_next(iter) == &t1);
    fail_unless(itree_iter_next(iter) == &t4);
    fail_unless(itree_iter_next(iter) == &t2);
    fail_unless(itree_iter_next(iter) == &t3);
    fail_unless(itree_iter_next(iter) == &t7);
    fail_unless(itree_iter_next(iter) == &t6);
    fail_unless(itree_iter_next(iter) == &t5);
    fail_unless(itree_iter_next(iter) == NULL);

    itree_iter_free(iter);
}
END_TEST

START_TEST(test_insert_into_empty)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(10, &root, &holes) == 0);
    fail_unless(holes == 0);
    CHK_NODE(root, == 10, == 10, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent1l)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(10, &root, &holes) == 0);
    fail_unless(itree_insert(9, &root, &holes) == 0);
    fail_unless(holes == 1);
    CHK_NODE(root, == 9, == 10, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent1r)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(10, &root, &holes) == 0);
    fail_unless(itree_insert(11, &root, &holes) == 0);
    fail_unless(holes == 0);
    CHK_NODE(root, == 10, == 11, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_not_adjacentl)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(10, &root, &holes) == 0);
    fail_unless(itree_insert(8, &root, &holes) == 0);
    fail_unless(holes == 1);
    CHK_NODE(root, == 10, == 10, == 0, != NULL, == NULL, == 1);
    itree_t *l = root->l;
    CHK_NODE(l, == 8, == 8, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_not_adjacentr)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(10, &root, &holes) == 0);
    fail_unless(itree_insert(12, &root, &holes) == 0);
    fail_unless(holes == 0);
    CHK_NODE(root, == 10, == 10, == 1, == NULL, != NULL, == 1);
    itree_t *r = root->r;
    CHK_NODE(r, == 12, == 12, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_balance1)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(10, &root, &holes) == 0);
    fail_unless(itree_insert(15, &root, &holes) == 0);
    fail_unless(itree_insert(13, &root, &holes) == 0);
    fail_unless(holes == 1);
    CHK_NODE(root, == 13, == 13, == 1, != NULL, != NULL, == 1);
    itree_t *l = root->l;
    CHK_NODE(l, == 10, == 10, == 0, == NULL, == NULL, == 0);
    itree_t *r = root->r;
    CHK_NODE(r, == 15, == 15, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_balance2)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(15, &root, &holes) == 0);
    fail_unless(itree_insert(10, &root, &holes) == 0);
    fail_unless(itree_insert(13, &root, &holes) == 0);
    fail_unless(holes == 1);
    CHK_NODE(root, == 13, == 13, == 1, != NULL, != NULL, == 1);
    itree_t *l = root->l;
    CHK_NODE(l, == 10, == 10, == 0, == NULL, == NULL, == 0);
    itree_t *r = root->r;
    CHK_NODE(r, == 15, == 15, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent2)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(10, &root, &holes) == 0);
    fail_unless(itree_insert(12, &root, &holes) == 0);
    fail_unless(itree_insert(11, &root, &holes) == 0);
    fail_unless(holes == 1);
    CHK_NODE(root, == 10, == 12, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent2_balance1)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(15, &root, &holes) == 0);
    fail_unless(itree_insert(9, &root, &holes) == 0);
    fail_unless(itree_insert(13, &root, &holes) == 0);
    fail_unless(itree_insert(11, &root, &holes) == 0);
    fail_unless(holes == 2);
    CHK_NODE(root, == 13, == 13, == 1, != NULL, != NULL, == 2);
    itree_t *l = root->l;
    CHK_NODE(l, == 9, == 9, == 1, == NULL, != NULL, == 1);
    itree_t *lr = l->r;
    CHK_NODE(lr, == 11, == 11, == 0, == NULL, == NULL, == 0);
    itree_t *r = root->r;
    CHK_NODE(r, == 15, == 15, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent2_balance2)
{
    itree_t *root = NULL;
    uint32_t holes;
    fail_unless(itree_insert(15, &root, &holes) == 0);
    fail_unless(itree_insert(9, &root, &holes) == 0);
    fail_unless(itree_insert(13, &root, &holes) == 0);
    fail_unless(itree_insert(11, &root, &holes) == 0);
    fail_unless(itree_insert(12, &root, &holes) == 0);
    fail_unless(holes == 2);
    CHK_NODE(root, == 11, == 13, == 1, != NULL, != NULL, == 1);
    itree_t *l = root->l;
    CHK_NODE(l, == 9, == 9, == 0, == NULL, == NULL, == 0);
    itree_t *r = root->r;
    CHK_NODE(r, == 15, == 15, == 0, == NULL, == NULL, == 0);
    itree_free(root);
}
END_TEST

TEST_SINGLE_EXTENSION(2, 15)
TEST_SINGLE_EXTENSION(4, 14)
TEST_SINGLE_EXTENSION(5, 14)
TEST_SINGLE_EXTENSION(7, 13)
TEST_SINGLE_EXTENSION(8, 13)
TEST_SINGLE_EXTENSION(10, 12)
TEST_SINGLE_EXTENSION(11, 12)
TEST_SINGLE_EXTENSION(13, 11)
TEST_SINGLE_EXTENSION(14, 11)
TEST_SINGLE_EXTENSION(16, 10)
TEST_SINGLE_EXTENSION(17, 10)
TEST_SINGLE_EXTENSION(19, 9)
TEST_SINGLE_EXTENSION(20, 9)
TEST_SINGLE_EXTENSION(22, 8)
TEST_SINGLE_EXTENSION(23, 8)
TEST_SINGLE_EXTENSION(25, 7)
TEST_SINGLE_EXTENSION(26, 7)
TEST_SINGLE_EXTENSION(28, 6)
TEST_SINGLE_EXTENSION(29, 6)
TEST_SINGLE_EXTENSION(31, 5)
TEST_SINGLE_EXTENSION(32, 5)
TEST_SINGLE_EXTENSION(34, 4)
TEST_SINGLE_EXTENSION(35, 4)
TEST_SINGLE_EXTENSION(37, 3)
TEST_SINGLE_EXTENSION(38, 3)
TEST_SINGLE_EXTENSION(40, 2)
TEST_SINGLE_EXTENSION(41, 2)
TEST_SINGLE_EXTENSION(43, 1)
TEST_SINGLE_EXTENSION(44, 1)
TEST_SINGLE_EXTENSION(46, 0)

static Suite *
create_suite(void)
{
    Suite *s = suite_create("itree");
    TCase *tc_core = tcase_create("core");

    tcase_add_test(tc_core, test_insert_into_empty);
    tcase_add_test(tc_core, test_insert_adjacent1l);
    tcase_add_test(tc_core, test_insert_adjacent1r);
    tcase_add_test(tc_core, test_insert_not_adjacentl);
    tcase_add_test(tc_core, test_insert_not_adjacentr);
    tcase_add_test(tc_core, test_insert_adjacent2);
    tcase_add_test(tc_core, test_iter_next1);
    tcase_add_test(tc_core, test_insert_balance1);
    tcase_add_test(tc_core, test_insert_balance2);
    tcase_add_test(tc_core, test_insert_adjacent2_balance1);
    tcase_add_test(tc_core, test_insert_adjacent2_balance2);

    TCase *tc_sext = tcase_create("sext");
    tcase_add_test(tc_sext, test_insert_single_extension_holes2);
    tcase_add_test(tc_sext, test_insert_single_extension_holes4);
    tcase_add_test(tc_sext, test_insert_single_extension_holes5);
    tcase_add_test(tc_sext, test_insert_single_extension_holes7);
    tcase_add_test(tc_sext, test_insert_single_extension_holes8);
    tcase_add_test(tc_sext, test_insert_single_extension_holes10);
    tcase_add_test(tc_sext, test_insert_single_extension_holes11);
    tcase_add_test(tc_sext, test_insert_single_extension_holes13);
    tcase_add_test(tc_sext, test_insert_single_extension_holes14);
    tcase_add_test(tc_sext, test_insert_single_extension_holes16);
    tcase_add_test(tc_sext, test_insert_single_extension_holes17);
    tcase_add_test(tc_sext, test_insert_single_extension_holes19);
    tcase_add_test(tc_sext, test_insert_single_extension_holes20);
    tcase_add_test(tc_sext, test_insert_single_extension_holes22);
    tcase_add_test(tc_sext, test_insert_single_extension_holes23);
    tcase_add_test(tc_sext, test_insert_single_extension_holes25);
    tcase_add_test(tc_sext, test_insert_single_extension_holes26);
    tcase_add_test(tc_sext, test_insert_single_extension_holes28);
    tcase_add_test(tc_sext, test_insert_single_extension_holes29);
    tcase_add_test(tc_sext, test_insert_single_extension_holes31);
    tcase_add_test(tc_sext, test_insert_single_extension_holes32);
    tcase_add_test(tc_sext, test_insert_single_extension_holes34);
    tcase_add_test(tc_sext, test_insert_single_extension_holes35);
    tcase_add_test(tc_sext, test_insert_single_extension_holes37);
    tcase_add_test(tc_sext, test_insert_single_extension_holes38);
    tcase_add_test(tc_sext, test_insert_single_extension_holes40);
    tcase_add_test(tc_sext, test_insert_single_extension_holes41);
    tcase_add_test(tc_sext, test_insert_single_extension_holes43);
    tcase_add_test(tc_sext, test_insert_single_extension_holes44);
    tcase_add_test(tc_sext, test_insert_single_extension_holes46);

    suite_add_tcase(s, tc_core);
    suite_add_tcase(s, tc_sext);

    return s;
}

int
main(int argc __attribute__ ((unused)),
     char **argv __attribute__ ((unused)))
{
    int number_failed;
    Suite *s = create_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
