#include "unity.h"
#include "manageInput.h"

void setUp(void) {
    // Test-Setup
}

void tearDown(void) {
    // Test-Cleanup
}

void test_create_prompt_should_handle_null_input(void) {
    PromptResult result = create_prompt(0, NULL);
    TEST_ASSERT_EQUAL(ERROR_INVALID_INPUT, result.error);
    TEST_ASSERT_NULL(result.content);
}

// Weitere Tests... 