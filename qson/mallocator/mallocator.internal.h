/*
 * Its not part of qson API its for internal use only
 */

#ifndef _qson_mallocator_mallocator_internal_h_
#define _qson_mallocator_mallocator_internal_h_

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

