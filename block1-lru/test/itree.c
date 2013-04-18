#include <check.h>
#include <stdio.h>
#include <stdlib.h>

#include "itree.h"

#define CHK_NODE(n, k1cond, k2cond, vcond, lcond, rcond) \
do { \
    fail_unless(n != NULL); \
    fail_unless(n->k1 k1cond); \
    fail_unless(n->k2 k2cond); \
    fail_unless(n->v vcond); \
    fail_unless(n->l lcond); \
    fail_unless(n->r rcond); \
} while(0);

START_TEST(test_iter_next1)
{
    itree_t t1 = { NULL, NULL, 0, 0, 0 };
    itree_t t2 = { NULL, NULL, 0, 0, 0 };
    itree_t t3 = { &t2, NULL, 0, 0, 0 };
    itree_t t4 = { &t1, &t3, 0, 0, 0 };
    itree_t t5 = { NULL, NULL, 0, 0, 0 };
    itree_t t6 = { NULL, &t5, 0, 0, 0 };
    itree_t t7 = { &t4, &t6, 0, 0, 0 };

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
    itree_insert(10, &root);
    CHK_NODE(root, == 10, == 10, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent1l)
{
    itree_t *root = NULL;
    itree_insert(10, &root);
    itree_insert(9, &root);
    CHK_NODE(root, == 9, == 10, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent1r)
{
    itree_t *root = NULL;
    itree_insert(10, &root);
    itree_insert(11, &root);
    CHK_NODE(root, == 10, == 11, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_not_adjacentl)
{
    itree_t *root = NULL;
    itree_insert(10, &root);
    itree_insert(8, &root);
    CHK_NODE(root, == 10, == 10, == 0, != NULL, == NULL);
    itree_t *l = root->l;
    CHK_NODE(l, == 8, == 8, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_not_adjacentr)
{
    itree_t *root = NULL;
    itree_insert(10, &root);
    itree_insert(12, &root);
    CHK_NODE(root, == 10, == 10, == 1, == NULL, != NULL);
    itree_t *r = root->r;
    CHK_NODE(r, == 12, == 12, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_balance1)
{
    itree_t *root = NULL;
    itree_insert(10, &root);
    itree_insert(15, &root);
    itree_insert(13, &root);
    CHK_NODE(root, == 13, == 13, == 1, != NULL, != NULL);
    itree_t *l = root->l;
    CHK_NODE(l, == 10, == 10, == 0, == NULL, == NULL);
    itree_t *r = root->r;
    CHK_NODE(r, == 15, == 15, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_balance2)
{
    itree_t *root = NULL;
    itree_insert(15, &root);
    itree_insert(10, &root);
    itree_insert(13, &root);
    CHK_NODE(root, == 13, == 13, == 1, != NULL, != NULL);
    itree_t *l = root->l;
    CHK_NODE(l, == 10, == 10, == 0, == NULL, == NULL);
    itree_t *r = root->r;
    CHK_NODE(r, == 15, == 15, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent2)
{
    itree_t *root = NULL;
    itree_insert(10, &root);
    itree_insert(12, &root);
    itree_insert(11, &root);
    CHK_NODE(root, == 10, == 12, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent2_balance1)
{
    itree_t *root = NULL;
    itree_insert(15, &root);
    itree_insert(9, &root);
    itree_insert(13, &root);
    itree_insert(11, &root);
    CHK_NODE(root, == 13, == 13, == 1, != NULL, != NULL);
    itree_t *l = root->l;
    CHK_NODE(l, == 9, == 9, == 1, == NULL, != NULL);
    itree_t *lr = l->r;
    CHK_NODE(lr, == 11, == 11, == 0, == NULL, == NULL);
    itree_t *r = root->r;
    CHK_NODE(r, == 15, == 15, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

START_TEST(test_insert_adjacent2_balance2)
{
    itree_t *root = NULL;
    itree_insert(15, &root);
    itree_insert(9, &root);
    itree_insert(13, &root);
    itree_insert(11, &root);
    itree_insert(12, &root);
    CHK_NODE(root, == 11, == 13, == 1, != NULL, != NULL);
    itree_t *l = root->l;
    CHK_NODE(l, == 9, == 9, == 0, == NULL, == NULL);
    itree_t *r = root->r;
    CHK_NODE(r, == 15, == 15, == 0, == NULL, == NULL);
    itree_free(root);
}
END_TEST

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

    suite_add_tcase(s, tc_core);

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
