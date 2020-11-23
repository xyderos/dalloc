#include "allocTests.h"

#include <stdio.h>

int main() {

  int failed = 0;

  Suite *s = memory_suite();

  SRunner *sr = srunner_create(s);

  srunner_run_all(sr,CK_NORMAL);

  failed = srunner_ntests_failed(sr);

  srunner_free(sr);

  return (failed ==0)? 0 : 1;

}
