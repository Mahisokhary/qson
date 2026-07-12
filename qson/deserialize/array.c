#include "deserialize.internal.h"

inline static qson_result_t set_has_next(struct qson_deserialize_ctx *c, bool *has_next) {
	char current_val = c->buffer[c->index];
	switch (current_val) {
	case QSON_VALUE_SEPARATOR:
		c->state = QSON_DESERIALIZING_STATE_ARRAY;
		*has_next = true;
		break;
	case QSON_END_ARRAY:
		c->state = QSON_DESERIALIZING_STATE_NONE;
		*has_next = false;
		break;
	default:
		return QSON_RESULT_INVALID_CHAR;
	}
	c->index++;
	return QSON_RESULT_OK;
}

qson_result_t qson_start_array(qson_deserialize_ctx_t ctx) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->state != QSON_DESERIALIZING_STATE_NONE) return QSON_RESULT_INVALID_STATE;

	qson_run(_qson_skip_white_spaces(c));

	if (c->buffer[c->index] != QSON_BEGIN_ARRAY) return QSON_RESULT_INVALID_CHAR;
	qson_ctx_skip(c, 1);

	qson_run(_qson_skip_white_spaces(c));
	if (c->buffer[c->index] == QSON_END_ARRAY) {
		c->state = QSON_DESERIALIZING_STATE_NONE;
		if (qson_ctx_has_size(c, 1)) c->index++;
	} else {
		c->state = QSON_DESERIALIZING_STATE_ARRAY;
	}

	return QSON_RESULT_OK;
}

qson_result_t qson_get_array_entry(qson_deserialize_ctx_t ctx, qson_type *type) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY) return QSON_RESULT_INVALID_STATE;
	qson_run(_qson_skip_white_spaces(c));
	qson_run(_qson_detect_type(c, type));
	c->state = QSON_DESERIALIZING_STATE_ARRAY_VALUE;
	return QSON_RESULT_OK;
}

qson_result_t qson_get_array_entry_value_string(qson_deserialize_ctx_t ctx, char *value, int *value_length, bool *has_next) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_read_string(c, value, value_length));
	qson_run(_qson_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_get_array_entry_value_bool(qson_deserialize_ctx_t ctx, bool *value, bool *has_next) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_read_bool(c, value));
	qson_run(_qson_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_get_array_entry_value_null(qson_deserialize_ctx_t ctx, bool *has_next) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_skip_null(c));
	qson_run(_qson_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_get_array_entry_value_number(qson_deserialize_ctx_t ctx, double *value, bool *has_next) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_read_number(c, value));
	qson_run(_qson_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_get_array_entry_value_sub_ctx(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t sub_ctx) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_create_sub_deserialize_ctx(ctx, sub_ctx));
	return QSON_RESULT_OK;
}

qson_result_t qson_get_array_entry_value_sub_ctx_end(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t sub_ctx, bool *has_next) {
	struct qson_deserialize_ctx *c = ctx;
	if (c->state != QSON_DESERIALIZING_STATE_SUBCTX) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_end_sub_deserialize_ctx(ctx, sub_ctx));
	qson_run(_qson_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

