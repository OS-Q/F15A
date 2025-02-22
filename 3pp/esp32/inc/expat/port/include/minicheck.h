/* Miniature re-implementation of the "check" library.
 *
 * This is intended to support just enough of check to run the Expat
 * tests.  This interface is based entirely on the portion of the
 * check library being used.
 *
 * This is *source* compatible, but not necessary *link* compatible.
 */

#ifdef __cplusplus
extern "C" {
#endif

#define CK_NOFORK 0
#define CK_FORK   1

#define CK_SILENT  0
#define CK_NORMAL  1
#define CK_VERBOSE 2

/* Workaround for Microsoft's compiler and Tru64 Unix systems where the
   C compiler has a working __func__, but the C++ compiler only has a 
   working __FUNCTION__.  This could be fixed in configure.in, but it's
   not worth it right now. */
#if defined (_MSC_VER) || (defined(__osf__) && defined(__cplusplus))
#define __func__ __FUNCTION__
#endif

/* ISO C90 does not support '__func__' predefined identifier */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ < 199901)
# define __func__ "(unknown)"
#endif

#define START_TEST(testname) static void testname(void) { \
    _check_set_test_info(__func__, __FILE__, __LINE__);   \
    {
#define END_TEST 

#define fail(msg)  _fail_unless(0, __FILE__, __LINE__, msg)

typedef void (*tcase_setup_function)(void);
typedef void (*tcase_teardown_function)(void);
typedef void (*tcase_test_function)(void);

typedef struct SRunner SRunner;
typedef struct Suite Suite;
typedef struct TCase TCase;

struct SRunner {
    Suite *suite;
    int nchecks;
    int nfailures;
};

struct Suite {
    const char *name;
    TCase *tests;
};

struct TCase {
    const char *name;
    tcase_setup_function setup;
    tcase_teardown_function teardown;
    tcase_test_function *tests;
    int ntests;
    int allocated;
    TCase *next_tcase;
};


/* Internal helper. */
void _check_set_test_info(char const *function,
                          char const *filename, int lineno);


/*
 * Prototypes for the actual implementation.
 */

void _fail_unless(int condition, const char *file, int line, const char *msg);
Suite *esp_suite_create(const char *name);
TCase *esp_tcase_create(const char *name);
void esp_suite_add_tcase(Suite *suite, TCase *tc);
void esp_tcase_add_checked_fixture(TCase *,
                               tcase_setup_function,
                               tcase_teardown_function);
void esp_tcase_add_test(TCase *tc, tcase_test_function test);
SRunner *esp_srunner_create(Suite *suite);
void esp_srunner_run_all(SRunner *runner, int verbosity);
int esp_srunner_ntests_failed(SRunner *runner);
void esp_srunner_free(SRunner *runner);

#ifdef __cplusplus
}
#endif
