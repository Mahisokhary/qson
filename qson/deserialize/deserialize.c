#include "deserialize.internal.h"

qson_result_t qson_deserialize_ctx_create(qson_deserialize_ctx_t *ctx, char *buffer, int size) {
	struct qson_deserialize_ctx *c = malloc(sizeof(struct qson_deserialize_ctx));
	c->buffer = buffer;
	c->size = size;
	c->index = 0;
	c->state = QSON_DESERIALIZING_STATE_NONE;
	c->flags = 0;
	*ctx = c;
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

qson_result_t qson_deserialize_bool(qson_deserialize_ctx_t ctx, bool *value) {
	struct qson_deserialize_ctx *c = ctx;
	char chr = c->buffer[c->index];

	int i;
	qson_ctx_size_check(c, 4);
	for (i = 0; i < array_len(QSON_BOOL_TRUE) - 1; i++) {
		if (c->buffer[c->index + i] != QSON_BOOL_TRUE[i]) {
			qson_ctx_size_check(c, 5);
			for (i = 0; i < array_len(QSON_BOOL_FALSE) - 1; i++) {
				if (c->buffer[c->index + i] != QSON_BOOL_FALSE[i]) {
					c->index += i;
					return QSON_RESULT_INVALID_CHAR;
				}
			}
			*value = false;
			goto qson_deserialize_bool_exit;
		}
	}
	*value = true;
qson_deserialize_bool_exit:
	c->index += i;
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_null(qson_deserialize_ctx_t ctx) {
	struct qson_deserialize_ctx *c = ctx;
	qson_ctx_size_check(c, 4);
	for (int i = 0; i < array_len(QSON_NULL) - 1; i++) {
		if (c->buffer[c->index] != QSON_NULL[i]) {
			return QSON_RESULT_INVALID_CHAR;
		}
		c->index++;
	}
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_number(qson_deserialize_ctx_t ctx, double *value) {
	struct qson_deserialize_ctx *c = ctx;
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

static inline qson_result_t skip_string(qson_deserialize_ctx_t ctx) {
	struct qson_deserialize_ctx *c = ctx;
	qson_ctx_skip(c, 1);
	while (c->buffer[c->index] != QSON_QUOTATION_MARK && c->index < c->size) {
		int move = c->buffer[c->index] == '\\' ? 2 : 1;
		qson_ctx_size_check(c, move);
		c->index += move;
	}
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_ctx_create_subctx(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t *sub_ctx) {
	struct qson_deserialize_ctx *c = ctx;
	struct qson_deserialize_ctx *sc = malloc(sizeof(struct qson_deserialize_ctx));
	sc->buffer = c->buffer + c->index;
	sc->index = 0;
	sc->size = c->size - c->index;
	sc->flags = QSON_DESERIALIZE_CTX_FLAG_IS_SUBCTX;
	sc->state = QSON_DESERIALIZING_STATE_NONE;
	c->state = QSON_DESERIALIZING_STATE_SUBCTX;
	*sub_ctx = sc;
	return QSON_RESULT_OK;
}

qson_result_t qson_end_sub_deserialize_ctx(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t sub_ctx) {
	struct qson_deserialize_ctx *c = ctx;
	struct qson_deserialize_ctx *sc = sub_ctx;
	if (c->state != QSON_DESERIALIZING_STATE_SUBCTX) return QSON_RESULT_INVALID_STATE;
	if (sc->state != QSON_DESERIALIZING_STATE_NONE) return QSON_RESULT_INVALID_STATE;
	if (!(sc->flags & QSON_DESERIALIZE_CTX_FLAG_IS_SUBCTX)) return QSON_RESULT_INVALID_CONTEXT;
	c->index += sc->index;
	free(sub_ctx);
	return QSON_RESULT_OK;
}

int qson_deserialize_ctx_index(qson_deserialize_ctx_t ctx) {
	return ((struct qson_deserialize_ctx *) ctx)->index;
}

qson_deserialize_state_t qson_deserialize_ctx_state(qson_deserialize_ctx_t ctx) {
	return ((struct qson_deserialize_ctx *) ctx)->state;
}

