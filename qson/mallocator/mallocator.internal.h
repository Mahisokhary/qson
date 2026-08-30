/*
 * Its not part of qson API its for internal use only
 */

#ifndef _qson_mallocator_mallocator_internal_h_
#define _qson_mallocator_mallocator_internal_h_

// For context containing an 'mallocator'
#define qmalloc(c, s) c->mallocator->malloc(s)
#define qrealloc(c, m, s) c->mallocator->realloc(m, s)
#define qfree(c, m) c->mallocator->free(m)

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

