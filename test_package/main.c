#include <moondance/test.h>

MD_TEST_CASE(my_test) { md_test_pass(); }

int main(int argc, char **argv) {
  md_test_suite suite = md_test_suite_create();

  md_test_add(&suite, my_test);

  return md_test_run(argc, argv, &suite);
}

