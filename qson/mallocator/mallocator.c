#include <qson/qson.internal.h>

qson_result_t qson_mallocator_create(qson_mallocator_t *mallocator, void* (*malloc)(size_t size), void* (*realloc)(void *buf, size_t size), void (*free)(void *buf)) {
	qson_mallocator_t m = malloc(sizeof(&m));
	m->malloc = malloc;
	m->realloc = realloc;
	m->free = free;
	*mallocator = m;
	return QSON_RESULT_OK;
}

qson_result_t qson_mallocator_destroy(qson_mallocator_t m) {
	m->free(m);
	return QSON_RESULT_OK;
}

