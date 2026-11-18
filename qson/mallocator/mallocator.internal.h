/*
 * Its not part of qson API its for internal use only
 */

#ifndef _qson_mallocator_mallocator_internal_h_
#define _qson_mallocator_mallocator_internal_h_

#define qmallocm(v, mal, s) do { \
	v = mal->malloc(s); \
	if (!v) return QSON_RESULT_OUT_OF_MEMORY; \
	} while(0);
#define qreallocm(v, mal, m, s) do { \
	v = mal->realloc(m, s); \
	if (!v) return QSON_RESULT_OUT_OF_MEMORY; \
	} while(0);
#define qfreem(mal, m) mal->free(m)

// For context containing an 'mallocator'
#define qmalloc(v, c, s) qmallocm(v, c->mallocator, s)
#define qrealloc(v, c, m, s) qreallocm(v, c->mallocator, m, s)
#define qfree(c, m) qfreem(c->mallocator, m)

#ifdef __cplusplus
extern "C" {
#endif

/*
 * A memory allocator
 */
struct qson_mallocator {
	void* (*malloc)(size_t size); // Allocate memory
	void* (*realloc)(void *buf, size_t size); // Change size of allocated memory
	void (*free)(void *buf); // Free allocated memory
};

#ifdef __cplusplus
}
#endif
#endif

