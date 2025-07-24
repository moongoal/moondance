#include <moondance/test.h>

MD_TEST_CASE(passive_pass) {}
MD_TEST_CASE(active_pass) { md_test_pass(); }
MD_TEST_CASE(fail) { md_test_fail(); }
MD_TEST_CASE(assert_pass) { md_test_assert(true); }
MD_TEST_CASE(assert_fail) { md_test_assert(false); }
MD_TEST_CASE(skip) { md_test_fail(); /* must never be run */ }

int main(int argc, char **argv) {
  md_test_suite suite = md_test_suite_create();

  md_test_add_case(&suite, "Active pass", active_pass);
  md_test_add_case(&suite, "Passive pass", passive_pass);
  md_test_add_case(&suite, "Fail", fail);
  md_test_add_case(&suite, "Assert pass", assert_pass);
  md_test_add_case(&suite, "Assert fail", assert_fail);
  md_test_add_case(&suite, "Skip", skip)->skip = true;

  return 1 - (
        md_test_run(argc, argv, &suite) == 2 /* number of tests supposed to fail */
    );
}

