#include "../src/dalloc.h"
#include <check.h>

START_TEST(test_memory_allocation) {

  void* test;

  sz iterations = 50, blocks = 25;

  while (iterations != 0) {

    while(blocks != 0){

      test = dalloc(120);

      ck_assert_ptr_eq(test,NULL);

      dfree(test);

      ck_assert_ptr_eq(test,NULL);

      blocks--;
    }

    iterations--;

  }

} END_TEST


Suite *memory_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Memory");
  tc_core = tcase_create("Core API");

  tcase_add_test(tc_core, test_memory_allocation);
  suite_add_tcase(s, tc_core);

  return s;
}


