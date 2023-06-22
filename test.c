#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "dll.h"
#include "array.h"

static void dll_get_null_length_test(void** state)
{
    unsigned int l = get_length(NULL);
    assert_int_equal(l, 0);
}

static void dll_get_length_test(void** state)
{
    struct Node* test_node_ref = alloc_node("a", NULL, NULL);
    append_node(test_node_ref, "b");
    append_node(test_node_ref->next, "c");

    unsigned int l = get_length(test_node_ref);
    assert_int_equal(l, 3);

}

static void dll_get_last_test(void** state)
{
    struct Node* t = alloc_node("a", NULL, NULL);
    append_node(t, "b");
    append_node(t->next, "c");

    struct Node* r = get_last(t);

    assert_string_equal(t->next->next->file_name, r->file_name);
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
}

static void dll_appending_test(void** state)
{
    struct Node* first_dll_start_ref = alloc_node("a", NULL, NULL);
    append_node(first_dll_start_ref, "b");

    struct Node* second_dll_start_ref = alloc_node("c", NULL, NULL);
    append_node(second_dll_start_ref, "d");
    append_node(second_dll_start_ref->next, "e");

    append_list(first_dll_start_ref, second_dll_start_ref);

    assert_string_equal(first_dll_start_ref->file_name, "a");
    assert_string_equal(first_dll_start_ref->next->next->next->next->file_name, "e");
}

static void arr_cl_alloc_test(void** state)
{
    arr_cl test = alloc_arr_cl(3);

    assert_non_null(test.array);
    assert_non_null(test.array[0]);

    char* a_256 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    strcpy(test.array[0], a_256);

    assert_string_equal(test.array[0], a_256);
    assert_int_equal(test.length, 3);
}

static void arrayification_test(void** state)
{
    struct Node* test_node_ref = alloc_node("a", NULL, NULL);
    append_node(test_node_ref, "b");

    arr_cl t = arrayify(test_node_ref);

    assert_string_equal(t.array[0], test_node_ref->file_name);
    assert_string_equal(t.array[1], test_node_ref->next->file_name);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(dll_get_null_length_test),
        cmocka_unit_test(dll_get_length_test),
        cmocka_unit_test(dll_get_last_test),
        cmocka_unit_test(node_allocation_test),
        cmocka_unit_test(node_appending_test),
        cmocka_unit_test(dll_appending_test),
        cmocka_unit_test(arr_cl_alloc_test),
        cmocka_unit_test(arrayification_test),

    };
 
    return cmocka_run_group_tests(tests, NULL, NULL);
}
