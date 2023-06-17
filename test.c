#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "dll.h"
#include "array.h"

static void globals_exist_test(void** state)
{
    assert_int_equal(LIST_LENGTH, 0);
}

static void node_allocation_test(void** state)
{
    struct Node* test_node_ref = alloc_node("a", NULL, NULL);
    assert_non_null(test_node_ref);
    assert_string_equal(test_node_ref->file_name, "a");
}

static void node_appending_test(void** state)
{
    struct Node* test_node_ref = alloc_node("a", NULL, NULL);
    append_node(test_node_ref, "b");

    assert_string_equal(test_node_ref->next->prev->file_name, "a");
    assert_string_equal(test_node_ref->next->file_name, "b");
}

static void dll_get_length_test(void** state)
{
    struct Node* test_node_ref = alloc_node("a", NULL, NULL);
    append_node(test_node_ref, "b");
    append_node(test_node_ref->next, "c");

    unsigned int l = get_length(test_node_ref);
    assert_int_equal(l, 3);

}

static void array_alloc_test(void** state)
{
    char** test_a = alloc_arr(3);

    assert_non_null(test_a);
    assert_non_null(test_a[0]);

    char* a_256 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    strcpy(test_a[0], a_256);
    assert_string_equal(test_a[0], a_256);
}

static void arrayification_test(void** state)
{
    struct Node* test_node_ref = alloc_node("a", NULL, NULL);
    append_node(test_node_ref, "b");

    char** t = arrayify(test_node_ref);

    assert_string_equal(t[0], test_node_ref->file_name);
    assert_string_equal(t[1], test_node_ref->next->file_name);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(globals_exist_test),
        cmocka_unit_test(node_allocation_test),
        cmocka_unit_test(node_appending_test),
        cmocka_unit_test(dll_get_length_test),
        cmocka_unit_test(array_alloc_test),
        cmocka_unit_test(arrayification_test),

    };
 
    return cmocka_run_group_tests(tests, NULL, NULL);
}
