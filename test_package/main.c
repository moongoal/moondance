#include <moondance/test.h>

MD_CASE(my_test) { md_pass(); }

int main(int argc, char **argv) {
  md_suite suite = md_suite_create();

  md_add(&suite, my_test);

  return md_run(argc, argv, &suite);
}

