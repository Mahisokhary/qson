/*
 * Define memory allocators!
 */

#ifndef _qson_mallocator_mallocator_h_
#define _qson_mallocator_mallocator_h_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Pointer to actual allocator
 */
typedef struct qson_mallocator *qson_mallocator_t;

/*
 * Creates a memory allocator
 */
qson_result_t qson_mallocator_create(qson_mallocator_t *mallocator, void* (*malloc)(size_t size), void* (*realloc)(void *buf, size_t size), void (*free)(void *buf));

/*
 * Destroys memory allocator
 */
qson_result_t qson_mallocator_destroy(qson_mallocator_t m);

/*
 * Returns qson default memory allocator
 */
qson_mallocator_t qson_mallocator_default();

#ifdef __cplusplus
}
#endif
#endif

