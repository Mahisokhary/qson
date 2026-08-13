#include "deserialize.internal.h"

inline static qson_result_t set_has_next(qson_deserialize_ctx_t c, bool *has_next) {
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

qson_result_t qson_deserialize_array_start(qson_deserialize_ctx_t c) {
	if (c->state != QSON_DESERIALIZING_STATE_NONE) return QSON_RESULT_INVALID_STATE;

	qson_run(_qson_deserialize_skip_white_spaces(c));

	if (c->buffer[c->index] != QSON_BEGIN_ARRAY) return QSON_RESULT_INVALID_CHAR;
	qson_ctx_skip(c, 1);

	qson_run(_qson_deserialize_skip_white_spaces(c));
	if (c->buffer[c->index] == QSON_END_ARRAY) {
		c->state = QSON_DESERIALIZING_STATE_NONE;
		if (qson_ctx_size_has(c, 1)) c->index++;
	} else {
		c->state = QSON_DESERIALIZING_STATE_ARRAY;
	}

	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_entry(qson_deserialize_ctx_t c, qson_type_t *type) {
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY) return QSON_RESULT_INVALID_STATE;
	qson_run(_qson_deserialize_skip_white_spaces(c));
	qson_run(_qson_detect_type(c, type));
	c->state = QSON_DESERIALIZING_STATE_ARRAY_VALUE;
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_entry_value_skip(qson_deserialize_ctx_t c, bool *has_next) {
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(_qson_deserialize_skip_white_spaces(c));
	qson_run(qson_deserialize_auto_skip(c));
	qson_run(_qson_deserialize_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_entry_value_string(qson_deserialize_ctx_t c, char *value, int *value_length, bool *has_next) {
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_deserialize_string(c, value, value_length));
	qson_run(_qson_deserialize_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_entry_value_bool(qson_deserialize_ctx_t c, bool *value, bool *has_next) {
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_deserialize_bool(c, value));
	qson_run(_qson_deserialize_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_entry_value_null(qson_deserialize_ctx_t c, bool *has_next) {
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_deserialize_null(c));
	qson_run(_qson_deserialize_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_entry_value_number(qson_deserialize_ctx_t c, double *value, bool *has_next) {
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_deserialize_number(c, value));
	qson_run(_qson_deserialize_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_entry_value_sub_ctx(qson_deserialize_ctx_t c, qson_deserialize_ctx_t *sc) {
	if (c->state != QSON_DESERIALIZING_STATE_ARRAY_VALUE) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_deserialize_ctx_create_subctx(c, sc));
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_entry_value_sub_ctx_end(qson_deserialize_ctx_t c, qson_deserialize_ctx_t sc, bool *has_next) {
	if (c->state != QSON_DESERIALIZING_STATE_SUBCTX) return QSON_RESULT_INVALID_STATE;
	qson_run(qson_deserialize_ctx_end_subctx(c, sc));
	qson_run(_qson_deserialize_skip_white_spaces(c));
	qson_run(set_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_deserialize_array_skip(qson_deserialize_ctx_t c) {
	qson_deserialize_ctx_t sc;
	qson_deserialize_state_t state = c->state;
	qson_run(qson_deserialize_ctx_create_subctx(c, &sc));
	qson_run(qson_deserialize_array_start(sc));
	if (sc->state == QSON_DESERIALIZING_STATE_ARRAY) {
		bool has_next = true;
		while (has_next) {
			qson_type_t type = QSON_TYPE_AUTO;
			qson_run(qson_deserialize_array_entry(sc, &type));
			qson_run(qson_deserialize_array_entry_value_skip(sc, &has_next));
		}
	}
	qson_run(qson_deserialize_ctx_end_subctx(c, sc));
	c->state = state;
	return QSON_RESULT_OK;
}

