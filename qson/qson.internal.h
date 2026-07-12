/*
 * Its not part of qson API its for internal use only
 */

#ifndef _qson__qson_h_
#define _qson__qson_h_

#include <qson/qson.h>

#ifdef __cplusplus
extern "C" {
#endif

#define array_len(a) (sizeof(a) / sizeof(a[0]))
#define qson_run(call) \
	do { \
		qson_result_t res = call; \
		if (res != QSON_RESULT_OK) return res; \
	} while (0)

#ifdef __cplusplus
}
#endif
#endif
