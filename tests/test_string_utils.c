#include "unity.h"
#include "../include/utils/string_utils.h"

void setUp(void) {
    // Test-Setup
}

void tearDown(void) {
    // Test-Cleanup
}

void test_string_create_should_handle_null_input(void) {
    SafeString *str = string_create(NULL);
    TEST_ASSERT_NOT_NULL(str);
    TEST_ASSERT_EQUAL_size_t(0, str->length);
    TEST_ASSERT_NOT_NULL(str->data);
    string_destroy(&str);
    TEST_ASSERT_NULL(str);
}

void test_string_append_should_handle_overflow(void) {
    SafeString *str = string_create("Test");
    TEST_ASSERT_NOT_NULL(str);
    
    // Großen String zum Anhängen erstellen
    char *large_string = calloc(1000000, 1);
    memset(large_string, 'a', 999999);
    
    ErrorCode error = string_append(str, large_string);
    TEST_ASSERT_EQUAL(ERROR_MEMORY_ALLOCATION, error);
    
    free(large_string);
    string_destroy(&str);
} 