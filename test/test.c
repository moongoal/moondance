#include <moondance/test.h>

MD_CASE(passive_pass) {}
MD_CASE(active_pass) { md_pass(); }
MD_CASE(fail) { md_fail(); }
MD_CASE(assert_pass) { md_assert(MD_TRUE); }
MD_CASE(assert_fail) { md_assert(MD_FALSE); }
MD_CASE(skip) { md_fail(); /* must never be run */ }

int main(int argc, char **argv) {
  md_suite suite = md_suite_create();

  md_add_case(&suite, "Active pass", test_case_active_pass);
  md_add_case(&suite, "Passive pass", test_case_passive_pass);
  md_add_case(&suite, "Fail", test_case_fail);
  md_add_case(&suite, "Assert pass", test_case_assert_pass);
  md_add_case(&suite, "Assert fail", test_case_assert_fail);
  md_add_case(&suite, "Skip", test_case_skip)->skip = MD_TRUE;

  return 1 - (
        md_run(argc, argv, &suite) == 2 /* number of tests supposed to fail */
    );
}

