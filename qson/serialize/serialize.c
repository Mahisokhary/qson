#include "serialize.internal.h"

qson_result_t qson_create_serialize_ctx(qson_serialize_ctx_t *ctx, char *buffer, int size) {
	struct qson_serialize_ctx *c = malloc(sizeof(struct qson_serialize_ctx));
	c->buffer = buffer;
	c->size = size;
	c->index = 0;
	c->state = QSON_SERIALIZE_STATE_NONE;
	c->flags = 0;
	*ctx = c;
	return QSON_RESULT_OK;
}

qson_result_t qson_end_serialize_ctx(qson_serialize_ctx_t ctx) {
	struct qson_serialize_ctx *c = ctx;
	if (c->state != QSON_SERIALIZE_STATE_NONE) return QSON_RESULT_INVALID_STATE;
	if (c->flags & QSON_SERIALIZE_CTX_FLAG_IS_SUBCTX) return QSON_RESULT_OK;
	c->buffer[c->index] = '\0';
	return QSON_RESULT_OK;
}

static inline qson_result_t _qson_serialize_string_write_escaped(qson_serialize_ctx_t ctx, char chr) {
	struct qson_serialize_ctx *c = ctx;
	qson_ctx_write(c, QSON_STRING_ESCAPE_CHAR);
	qson_ctx_write(c, chr);
	return QSON_RESULT_OK;
}

qson_result_t qson_write_string(qson_serialize_ctx_t ctx, char *value) {
	struct qson_serialize_ctx *c = ctx;
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

qson_result_t qson_write_number(qson_serialize_ctx_t ctx, double value) {
	struct qson_serialize_ctx *c = ctx;
	int available_size = c->size - c->index;
	int used_size = snprintf(c->buffer + c->index, available_size, "%f", value);
	if (used_size > available_size) return QSON_RESULT_BUFFER_TOO_SMALL;
	c->index += used_size;
	return QSON_RESULT_OK;
}

qson_result_t qson_create_sub_serialize_ctx(qson_serialize_ctx_t ctx, qson_serialize_ctx_t *sub_ctx) {
	struct qson_serialize_ctx *c = ctx;
	struct qson_serialize_ctx *sc = malloc(sizeof(struct qson_serialize_ctx));
	c->state = QSON_SERIALIZE_STATE_SUBCTX;
	sc->state = QSON_SERIALIZE_STATE_NONE;
	sc->flags = c->flags | QSON_SERIALIZE_CTX_FLAG_IS_SUBCTX;
	sc->index = c->index;
	sc->buffer = c->buffer;
	sc->size = c->size;
	*sub_ctx = sc;
	return QSON_RESULT_OK;
}

