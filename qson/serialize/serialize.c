#include "serialize.internal.h"

qson_result_t qson_serialize_ctx_create(qson_serialize_ctx_t *ctx, char *buffer, int size) {
	qson_serialize_ctx_t c = malloc(sizeof(struct qson_serialize_ctx));
	c->buffer = buffer;
	c->size = size;
	c->index = 0;
	c->state = QSON_SERIALIZE_STATE_NONE;
	c->flags = 0;
	*ctx = c;
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_ctx_destroy(qson_serialize_ctx_t ctx) {
	free(ctx);
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_ctx_end(qson_serialize_ctx_t c) {
	if (c->state != QSON_SERIALIZE_STATE_NONE) return QSON_RESULT_INVALID_STATE;
	if (c->flags & QSON_SERIALIZE_CTX_FLAG_IS_SUBCTX) return QSON_RESULT_OK;
	c->buffer[c->index] = '\0';
	return QSON_RESULT_OK;
}

static inline qson_result_t _qson_serialize_string_write_escaped(qson_serialize_ctx_t c, char chr) {
	qson_ctx_write(c, QSON_STRING_ESCAPE_CHAR);
	qson_ctx_write(c, chr);
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_string(qson_serialize_ctx_t c, char *value) {
	qson_ctx_write(c, QSON_QUOTATION_MARK);

	int i = 0;
	char chr;
	while ((chr = value[i]) != '\0') {
		switch (chr) {
		case '\b':
		case '\f':
		case '\n':
		case '\r':
		case '\t':
		case QSON_QUOTATION_MARK:
			qson_run(_qson_serialize_string_write_escaped(c, chr));
			break;
		default:
			if (chr < 32 || 126 < chr) return QSON_RESULT_INVALID_CHAR;
			qson_ctx_write(c, chr);
			break;
		}
		i++;
	}
	qson_ctx_write(c, QSON_QUOTATION_MARK);
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_number(qson_serialize_ctx_t c, double value) {
	int available_size = c->size - c->index;
	int used_size = snprintf(c->buffer + c->index, available_size, "%f", value);
	if (used_size > available_size) return QSON_RESULT_BUFFER_TOO_SMALL;
	c->index += used_size;
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_ctx_create_subctx(qson_serialize_ctx_t c, qson_serialize_ctx_t *sub_ctx) {
	qson_serialize_ctx_t sc = malloc(sizeof(struct qson_serialize_ctx));
	c->state = QSON_SERIALIZE_STATE_SUBCTX;
	sc->state = QSON_SERIALIZE_STATE_NONE;
	sc->flags = c->flags | QSON_SERIALIZE_CTX_FLAG_IS_SUBCTX;
	sc->index = c->index;
	sc->buffer = c->buffer;
	sc->size = c->size;
	*sub_ctx = sc;
	return QSON_RESULT_OK;
}

