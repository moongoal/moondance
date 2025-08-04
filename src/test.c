#include <moondance/test.h>
#include <moondance/util.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_MSG_BUF_LEN 1024

md_result md_current_test_result;

static void on_test_startup(md_suite *const suite, md_case const *const tcase) {
  for (unsigned i = 0; i < suite->reporter_count; ++i) {
    suite->reporters[i].on_case_startup(&suite->reporters[i], tcase->name);
  }

  if (suite->each_setup) {
    suite->each_setup(tcase->user_ctx);
  }
}

static void on_test_complete(md_suite *const suite, md_case const *const tcase,
                             md_result const result) {
  if (result != MD_RESULT_SKIPPED && suite->each_cleanup) {
    suite->each_cleanup(tcase->user_ctx);
  }

  for (unsigned i = 0; i < suite->reporter_count; ++i) {
    suite->reporters[i].on_case_complete(&suite->reporters[i], tcase->name,
                                         result);
  }
}

static void on_suite_startup(md_suite *const suite) {
  for (unsigned i = 0; i < suite->reporter_count; ++i) {
    suite->reporters[i].on_suite_startup(&suite->reporters[i], suite);
  }

  if (suite->suite_setup) {
    suite->suite_setup(suite);
  }
}

static void on_suite_complete(md_suite *const suite) {
  if (suite->suite_cleanup) {
    suite->suite_cleanup(suite);
  }

  for (unsigned i = 0; i < suite->reporter_count; ++i) {
    suite->reporters[i].on_suite_complete(&suite->reporters[i], suite);
  }
}

int md_run(int, char **, md_suite *const suite) {
  int failed_count = 0;

  on_suite_startup(suite);

  for (unsigned i = 0; i < suite->case_count; ++i) {
    md_case const *const tcase = &suite->cases[i];

    on_test_startup(suite, tcase);
    md_current_test_result = MD_RESULT_NOT_RUN;

    if (!tcase->skip) {
      if (tcase->setup) {
        tcase->setup(tcase->user_ctx);
      }

      tcase->test(tcase->user_ctx);

      if (tcase->cleanup) {
        tcase->cleanup(tcase->user_ctx);
      }

      if (md_current_test_result == MD_RESULT_NOT_RUN) {
        // Test reached the end without failing - auto pass
        md_current_test_result = MD_RESULT_PASSED;
      }
    } else {
      md_current_test_result = MD_RESULT_SKIPPED;
    }

    failed_count += (md_current_test_result == MD_RESULT_FAILED);

    on_test_complete(suite, tcase, md_current_test_result);
  }

  on_suite_complete(suite);

  return failed_count;
}

static void
console_result_reporter_on_suite_startup(md_reporter *const reporter,
                                         md_suite const *const) {
  md_console_result_reporter *const data =
      malloc(sizeof(md_console_result_reporter));

  data->passed_test_count = 0;
  data->failed_test_count = 0;
  data->skipped_test_count = 0;

  reporter->data = data;
}

static void
console_result_reporter_on_suite_complete(md_reporter *const reporter,
                                          md_suite const *const) {
  md_console_result_reporter *const data = reporter->data;

  int const test_count = (data->passed_test_count + data->failed_test_count +
                          data->skipped_test_count);

  float const pass_ratio =
      (test_count > 0 ? (float)data->passed_test_count / test_count : 1.0f);

  fprintf(stderr, "Ratio: %.2f\n", pass_ratio);
  fprintf(stderr, "Passed:  %u\n", data->passed_test_count);
  fprintf(stderr, "Failed:  %u\n", data->failed_test_count);
  fprintf(stderr, "Skipped: %u\n", data->skipped_test_count);
  fprintf(stderr, "Total:   %u\n", test_count);

  free(reporter->data);
  reporter->data = NULL;
}

static void console_result_reporter_on_case_startup(md_reporter *const,
                                                    char const *const name) {
  fputs(name, stderr);
}

static void console_result_reporter_on_case_complete(
    md_reporter *const reporter, char const *const, md_result const result) {
  char result_text[MAX_MSG_BUF_LEN];
  md_console_result_reporter *const data = reporter->data;

  switch (result) {
  case MD_RESULT_NOT_RUN: // Should never happen
    strcpy_s(result_text, sizeof(char) * MAX_MSG_BUF_LEN, "-NOT RUN-");
    break;

  case MD_RESULT_PASSED:
    strcpy_s(result_text, sizeof(char) * MAX_MSG_BUF_LEN, "PASS");
    data->passed_test_count++;
    break;

  case MD_RESULT_FAILED:
    strcpy_s(result_text, sizeof(char) * MAX_MSG_BUF_LEN, "***FAIL***");
    data->failed_test_count++;
    break;

  case MD_RESULT_SKIPPED:
    strcpy_s(result_text, sizeof(char) * MAX_MSG_BUF_LEN, "-SKIP-");
    data->skipped_test_count++;
    break;
  }

  fprintf(stderr, " [%s]\n", result_text);
}

md_suite md_suite_create() {
  return (md_suite){.cases = {0},
                    .reporters = {md_console_result_reporter_create(), {0}},
                    .each_setup = NULL,
                    .each_cleanup = NULL,
                    .suite_setup = NULL,
                    .suite_cleanup = NULL,
                    .case_count = 0,
                    .reporter_count = 1};
}

md_reporter md_console_result_reporter_create() {
  return (md_reporter){
      .on_suite_startup = console_result_reporter_on_suite_startup,
      .on_suite_complete = console_result_reporter_on_suite_complete,
      .on_case_startup = console_result_reporter_on_case_startup,
      .on_case_complete = console_result_reporter_on_case_complete,
      .data = NULL};
}

md_case *md_add_case(md_suite *const suite, char const *const name,
                     md_function const test) {
  return md_add_case2(suite,
                      &(md_case){
                          name,            // name
                          test,            // test
                          NULL,            // setup
                          NULL,            // cleanup
                          suite->user_ctx, // user_ctx
                          false            // skip
                      });
}

md_case *md_add_case2(md_suite *const suite, md_case *const tcase) {
  unsigned const index = suite->case_count++;

  if (index > MD_MAX_CASES) {
    fputs("Too many test cases.", stderr);
    abort();
  }

  md_case *const suite_tcase = &suite->cases[index];
  *suite_tcase = *tcase;

  return suite_tcase;
}

