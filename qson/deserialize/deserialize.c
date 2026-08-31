#include "deserialize.internal.h"

qson_result_t qson_deserialize_ctx_create(qson_deserialize_ctx_t *ctx, char *buffer, int size) {

	qson_mallocator_t m = qson_mallocator_default();
	qson_deserialize_ctx_t c = m->malloc(sizeof(struct qson_deserialize_ctx));
	c->buffer = buffer;
	c->size = size;
	c->index = 0;
	c->state = QSON_DESERIALIZING_STATE_NONE;
	c->flags = 0;
	c->mallocator = m;
	*ctx = c;
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_ctx_destroy(qson_deserialize_ctx_t ctx) {
	qfree(ctx, ctx);
	return QSON_RESULT_OK;
}

bool qson_is_white_space(char chr) {
	for (int i = 0; i < array_len(QSON_WHITESPACES) - 1; i++) {
		if (chr == QSON_WHITESPACES[i]) return true;
	}
	return false;
}

qson_result_t qson_deserialize_skip_white_spaces(qson_deserialize_ctx_t ctx) {
	return _qson_deserialize_skip_white_spaces(ctx);
}

qson_result_t qson_deserialize_bool(qson_deserialize_ctx_t c, bool *value) {
	char *pos = &c->buffer[c->index];
	if (qson_ctx_size_has(c, sizeof(QSON_BOOL_TRUE) - 1) && memcmp(pos, QSON_BOOL_TRUE, sizeof(QSON_BOOL_TRUE) - 1) == 0) {
		qson_ctx_skip(c, sizeof(QSON_BOOL_TRUE) - 1);
		*value = true;
	} else if (qson_ctx_size_has(c, sizeof(QSON_BOOL_FALSE) - 1) && memcmp(pos, QSON_BOOL_FALSE, sizeof(QSON_BOOL_FALSE) - 1) == 0) {
		qson_ctx_skip(c, sizeof(QSON_BOOL_FALSE) - 1);
		*value = false;
	} else {
		return QSON_RESULT_INVALID_CHAR;
	}
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_bool_skip(qson_deserialize_ctx_t c) {
	bool ignored;
	return qson_deserialize_bool(c, &ignored);
}

qson_result_t qson_deserialize_null(qson_deserialize_ctx_t c) {
	qson_ctx_size_check(c, 4);
	for (int i = 0; i < array_len(QSON_NULL) - 1; i++) {
		if (c->buffer[c->index] != QSON_NULL[i]) {
			return QSON_RESULT_INVALID_CHAR;
		}
		c->index++;
	}
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_number(qson_deserialize_ctx_t c, double *value) {
	switch (c->buffer[c->index]) {
	case '0':
		c->index++;
		return QSON_RESULT_OK;
	case '+':
		return QSON_RESULT_INVALID_CHAR;
	}

	char *endptr;
	*value = strtod(c->buffer + c->index, &endptr);
	c->index = endptr - c->buffer;
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_number_skip(qson_deserialize_ctx_t c) {
	double ignored;
	return qson_deserialize_number(c, &ignored);
}

qson_result_t qson_deserialize_string_skip(qson_deserialize_ctx_t c) {
	if (c->buffer[c->index] != QSON_QUOTATION_MARK) return QSON_RESULT_INVALID_CHAR;
	qson_ctx_skip(c, 1);
	while (c->buffer[c->index] != QSON_QUOTATION_MARK && c->index < c->size) {
		int move = c->buffer[c->index] == '\\' ? 2 : 1;
		qson_ctx_skip(c, move);
	}
	qson_ctx_skip(c, 1);
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_ctx_create_subctx(qson_deserialize_ctx_t c, qson_deserialize_ctx_t *sub_ctx) {
	qson_deserialize_ctx_t sc = qmalloc(c, sizeof(struct qson_deserialize_ctx));
	sc->buffer = c->buffer + c->index;
	sc->index = 0;
	sc->size = c->size - c->index;
	sc->flags = QSON_DESERIALIZE_CTX_FLAG_IS_SUBCTX;
	sc->state = QSON_DESERIALIZING_STATE_NONE;
	sc->mallocator = c->mallocator;
	c->state = QSON_DESERIALIZING_STATE_SUBCTX;
	*sub_ctx = sc;
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_ctx_end_subctx(qson_deserialize_ctx_t c, qson_deserialize_ctx_t sc) {
	if (c->state != QSON_DESERIALIZING_STATE_SUBCTX) return QSON_RESULT_INVALID_STATE;
	if (sc->state != QSON_DESERIALIZING_STATE_NONE) return QSON_RESULT_INVALID_STATE;
	if (!(sc->flags & QSON_DESERIALIZE_CTX_FLAG_IS_SUBCTX)) return QSON_RESULT_INVALID_CONTEXT;
	c->index += sc->index;
	qfree(sc, sc);
	return QSON_RESULT_OK;
}

int qson_deserialize_ctx_index(qson_deserialize_ctx_t c) {
	return c->index;
}

qson_deserialize_state_t qson_deserialize_ctx_state(qson_deserialize_ctx_t c) {
	return c->state;
}

qson_result_t qson_deserialize_auto_skip(qson_deserialize_ctx_t c) {
	qson_type_t type = QSON_TYPE_AUTO;
	qson_run(_qson_detect_type(c, &type));
	switch (type) {
	case QSON_TYPE_STRING:
		return qson_deserialize_string_skip(c);
	case QSON_TYPE_NUMBER:
		return qson_deserialize_number_skip(c);
	case QSON_TYPE_NULL:
		return qson_deserialize_null(c);
	case QSON_TYPE_BOOL:
		return qson_deserialize_bool_skip(c);
	case QSON_TYPE_ARRAY:
		return qson_deserialize_array_skip(c);
	case QSON_TYPE_OBJECT:
		return qson_deserialize_object_skip(c);
	}
	return QSON_RESULT_INVALID_TYPE;
}

