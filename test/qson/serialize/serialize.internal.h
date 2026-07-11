#ifndef _test_serialize_internal_h_
#define _test_serialize_internal_h_

#include <string.h>
#include <qson/serialize/serialize.internal.h>
#include <test/test.h>

#ifdef __cplusplus
extern "C" {
#endif

#define test_run_log(m) printf("[*] Running test %s: ", m)
#define test_result_log(s) printf("%s\n", s ? "passed" : "failed")

#ifdef __cplusplus
}
#endif
#endif

