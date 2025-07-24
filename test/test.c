#include <moondance/test.h>

MD_CASE(passive_pass) {}
MD_CASE(active_pass) { md_pass(); }
MD_CASE(fail) { md_fail(); }
MD_CASE(assert_pass) { md_assert(true); }
MD_CASE(assert_fail) { md_assert(false); }
MD_CASE(skip) { md_fail(); /* must never be run */ }

int main(int argc, char **argv) {
  md_suite suite = md_suite_create();

  md_add_case(&suite, "Active pass", active_pass);
  md_add_case(&suite, "Passive pass", passive_pass);
  md_add_case(&suite, "Fail", fail);
  md_add_case(&suite, "Assert pass", assert_pass);
  md_add_case(&suite, "Assert fail", assert_fail);
  md_add_case(&suite, "Skip", skip)->skip = true;

  return 1 - (
        md_run(argc, argv, &suite) == 2 /* number of tests supposed to fail */
    );
}

