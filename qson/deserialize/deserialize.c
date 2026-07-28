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

qson_result_t qson_deserialize_ctx_destroy(qson_deserialize_ctx_t ctx) {
	free(ctx);
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

qson_result_t qson_deserialize_bool_skip(qson_deserialize_ctx_t ctx) {
	struct qson_deserialize_ctx *c = ctx;
	char *pos = &c->buffer[c->index];
	if (qson_ctx_size_has(c, sizeof(QSON_BOOL_TRUE) - 1) && memcmp(pos, QSON_BOOL_TRUE, sizeof(QSON_BOOL_TRUE) - 1) == 0) {
		qson_ctx_skip(c, sizeof(QSON_BOOL_TRUE) - 1);
	} else if (qson_ctx_size_has(c, sizeof(QSON_BOOL_FALSE)) && memcmp(pos, QSON_BOOL_FALSE, sizeof(QSON_BOOL_FALSE) - 1) == 0) {
		qson_ctx_skip(c, sizeof(QSON_BOOL_FALSE));
	} else {
		return QSON_RESULT_INVALID_CHAR;
	}
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

qson_result_t qson_deserialize_number_skip(qson_deserialize_ctx_t ctx) {
	double ignored;
	return qson_deserialize_number(ctx, &ignored);
}

qson_result_t qson_deserialize_string_skip(qson_deserialize_ctx_t ctx) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->buffer[c->index] != QSON_QUOTATION_MARK) return QSON_RESULT_INVALID_CHAR;
	qson_ctx_skip(c, 1);
	while (c->buffer[c->index] != QSON_QUOTATION_MARK && c->index < c->size) {
		int move = c->buffer[c->index] == '\\' ? 2 : 1;
		qson_ctx_skip(c, move);
	}
	qson_ctx_skip(c, 1);
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

qson_result_t qson_deserialize_ctx_end_subctx(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t sub_ctx) {
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

qson_result_t qson_deserialize_auto_skip(qson_deserialize_ctx_t ctx) {
	struct qson_deserialize_ctx *c = ctx;
	qson_type_t type = QSON_TYPE_AUTO;
	qson_run(_qson_detect_type(c, &type));
	switch (type) {
	case QSON_TYPE_STRING:
		qson_run(qson_deserialize_string_skip(ctx));
		break;
	case QSON_TYPE_NUMBER:
		qson_run(qson_deserialize_number_skip(ctx));
		break;
	case QSON_TYPE_NULL:
		qson_run(qson_deserialize_null(ctx));
		break;
	case QSON_TYPE_BOOL:
		qson_run(qson_deserialize_bool_skip(ctx));
		break;
	case QSON_TYPE_OBJECT:
	case QSON_TYPE_ARRAY:
	default:
		return QSON_RESULT_INVALID_TYPE;
	}
	return QSON_RESULT_OK;
}

