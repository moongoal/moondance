/**
 * @internal
 *
 * Test framework.
 */
#ifndef MD_H
#define MD_H

#include <moondance/util.h>

#define MD_MAX_CASES (256u)
#define MD_MAX_REPORTERS (16u)

#define MDNODISCARD [[nodiscard]]

/**
 * Define a test case function prototype.
 *
 * @param case_name The name of the test case.
 */
#define MD_CASE(case_name) static void test_case_##case_name(void *)

/**
 * Define a test case function prototype with context.
 *
 * @param case_name The name of the test case.
 * @param ctx_name The name of the context object pointer.
 */
#define MD_CASE_WC2(case_name, ctx_name) static void test_case_##case_name(void *const ctx_name)

/**
 * Define a test case function prototype with context.
 *
 * @param case_name The name of the test case.
 */
#define MD_CASE_WC(case_name) MD_CASE_WC2(case_name, ctx)

#define md_pass() \
  do { \
    md_current_test_result = MD_RESULT_PASSED; \
    return; \
  } while(false)
#define md_fail() \
  do { \
    md_current_test_result = MD_RESULT_FAILED; \
    return; \
  } while(false)
#define md_assert(cond) \
  if(!(cond)) { \
    md_fail(); \
    return; \
  }
#define md_add(suite_ptr, case_fun) \
  md_add_case(suite_ptr, MD_QUOTE(test_case_##case_fun), test_case_##case_fun)

typedef struct md_suite md_suite;
typedef struct md_reporter md_reporter;

/**
 * The result of a test.
 */
typedef enum {
  MD_RESULT_NOT_RUN,
  MD_RESULT_PASSED,
  MD_RESULT_FAILED,
  MD_RESULT_SKIPPED
} md_result;

/**
 * @internal
 *
 * Result of the currently running test.
 */
extern md_result md_current_test_result;

/**
 * The function prototype for a test case.
 */
typedef void (*md_function)(void *const ctx);

/**
 * Individual test setup.
 *
 * @param ctx The user-provided test context object.
 */
typedef void (*md_local_setup)(void *const ctx);

/**
 * Individual test cleanup.
 *
 * @param ctx The user-provided test context object.
 */
typedef void (*md_local_cleanup)(void *const ctx);

/**
 * Global individual test setup.
 */
typedef void (*md_each_setup)(void *const ctx);

/**
 * Global individual test cleanup.
 */
typedef void (*md_each_cleanup)(void *const ctx);

/**
 * Global test suite setup.
 *
 * @param suite The test suite.
 */
typedef void (*md_suite_setup)(md_suite *const suite);

/**
 * Global test suite cleanup.
 *
 * @param suite The test suite.
 */
typedef void (*md_suite_cleanup)(md_suite *const suite);

/**
 * @internal
 *
 * A test case.
 */
typedef struct {
  /**
   * The name of the test.
   */
  char const *name;

  /**
   * The test function.
   */
  md_function test;

  /**
   * Test setup function.
   */
  md_local_setup setup;

  /**
   * Test cleanup function.
   */
  md_local_cleanup cleanup;

  /**
   * Pointer to the user-provided context object for this case.
   *
   * By default this value is initialised to the suite user context.
   */
  void *user_ctx;

  /**
   * A value indicating whether the test must be skipped.
   */
  bool skip;
} md_case;

/**
 * Called before a test is run.
 *
 * @param reporter The reporter object.
 * @param name The name of the starting test.
 */
typedef void (*md_result_reporter_on_case_startup)(
    md_reporter *const reporter,
    char const *const name
);

/**
 * Called after a test run completes.
 *
 * @param reporter The reporter object.
 * @param name The name of the completed test.
 * @param result The test result.
 */
typedef void (*md_result_reporter_on_case_complete)(
    md_reporter *const reporter,
    char const *const name,
    md_result const result
);

/**
 * Called before the test suite is run.
 *
 * @param reporter The reporter object.
 * @param suite The test suite about to run.
 */
typedef void (*md_result_reporter_on_suite_startup)(
    md_reporter *const reporter,
    md_suite const *const suite
);

/**
 * Called after the test suite run completes.
 *
 * @param reporter The reporter object.
 * @param suite The completed test suite.
 */
typedef void (*md_result_reporter_on_suite_complete)(
    md_reporter *const reporter,
    md_suite const *const suite
);

/**
 * Reporter interface. A test reporter is an object that receives
 * the result of each test invocation as soon as a result is known.
 * and is responsible for publishing this result according to its
 * own semantics.
 */
struct md_reporter {
  md_result_reporter_on_suite_startup on_suite_startup;
  md_result_reporter_on_suite_complete on_suite_complete;
  md_result_reporter_on_case_startup on_case_startup;
  md_result_reporter_on_case_complete on_case_complete;

  /**
   * Test reporter data.
   */
  void *data;
};

/**
 * A reporter that prints results to stderr.
 */
typedef struct {
  int passed_test_count;
  int failed_test_count;
  int skipped_test_count;
} md_console_result_reporter;

/**
 * Create a new console result reporter.
 *
 * @return The new test reporter.
 */
md_reporter md_console_result_reporter_create();

/**
 * A collection of related tests and reporters that can be
 * executed to publish a set of results.
 *
 * By default, a test suite has a console result reporter attached
 * upon creation.
 */
struct md_suite {
  /**
   * The test case instances.
   */
  md_case cases[MD_MAX_CASES];

  /**
   * The test reporter instances.
   */
  md_reporter reporters[MD_MAX_REPORTERS];

  /**
   * Setup function to run before each test.
   */
  md_each_setup each_setup;

  /**
   * Cleanup function to run after each test.
   */
  md_each_cleanup each_cleanup;

  /**
   * Setup function to run before all tests.
   */
  md_suite_setup suite_setup;

  /**
   * Cleanup function to run after all tests.
   */
  md_suite_cleanup suite_cleanup;

  /**
   * The number of registered test case instances.
   */
  unsigned case_count;

  /**
   * The number of registered test reporter instances.
   */
  unsigned reporter_count;

  /**
   * Pointer to the default user-provided context object.
   *
   * This is the default user context assigned to each test. Each
   * test case's user context can then be overridden.
   */
  void *user_ctx;
};

/**
 * Create a new test suite.
 *
 * @return Output test suite object.
 */
MDNODISCARD md_suite md_suite_create();

/**
 * Run a test suite.
 *
 * @param argc The item count in argv.
 * @param argv The program argument array of strings.
 * @param suite Test suite to run.
 *
 * @return The number of failed tests.
 */
int md_run(int argc, char **argv, md_suite *const suite);

/**
 * Add a new test reporter.
 *
 * @param suite The suite to add the reporter to.
 * @param reporter The reporter to add.
 */
void md_add_reporter(md_suite *const suite, md_reporter *const reporter);

/**
 * Add a new test case.
 *
 * @param suite The suite to add the reporter to.
 * @param name The test name.
 * @param test The test function to add.
 *
 * @return A pointer to the test case.
 */
md_case *md_add_case(md_suite *const suite, char const *const name, md_function const test);

/**
 * Add a new test case.
 *
 * @param suite The suite to add the reporter to.
 * @param tcase The test case template to add (will be copied).
 *
 * @return A pointer to the suite test case created by copying the template.
 */
md_case *md_add_case2(md_suite *const suite, md_case *const tcase);

#endif // MD_H

