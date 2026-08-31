#include "serialize.internal.h"

static inline qson_result_t _write_object_entry(qson_serialize_ctx_t c, char *key) {
	qson_run(qson_serialize_string(c, key));
	qson_ctx_write(c, QSON_NAME_SEPARATOR);
	return QSON_RESULT_OK;
}

static inline qson_result_t _handle_has_next(qson_serialize_ctx_t c, bool has_next) {
	if (has_next) {
		qson_ctx_write(c, QSON_VALUE_SEPARATOR);
	} else {
		qson_ctx_write(c, QSON_END_OBJECT);
		c->state = QSON_SERIALIZE_STATE_NONE;
	}
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_object(qson_serialize_ctx_t c) {
	if (c->state != QSON_SERIALIZE_STATE_NONE) return QSON_RESULT_INVALID_STATE;
	qson_ctx_write(c, QSON_BEGIN_OBJECT);
	c->state = QSON_SERIALIZE_STATE_OBJECT;
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_object_entry_string(qson_serialize_ctx_t c, char *key, char *value, bool has_next) {
	if (c->state != QSON_SERIALIZE_STATE_OBJECT) return QSON_RESULT_INVALID_STATE;
	qson_run(_write_object_entry(c, key));
	qson_run(qson_serialize_string(c, value));
	qson_run(_handle_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_object_entry_null(qson_serialize_ctx_t c, char *key, bool has_next) {
	if (c->state != QSON_SERIALIZE_STATE_OBJECT) return QSON_RESULT_INVALID_STATE;
	qson_run(_write_object_entry(c, key));
	qson_ctx_write_buffer(c, QSON_NULL);
	qson_run(_handle_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_object_entry_bool(qson_serialize_ctx_t c, char *key, bool value, bool has_next) {
	if (c->state != QSON_SERIALIZE_STATE_OBJECT) return QSON_RESULT_INVALID_STATE;
	qson_run(_write_object_entry(c, key));
	char *buffer = value ? QSON_BOOL_TRUE : QSON_BOOL_FALSE;
	qson_ctx_write_buffer(c, buffer);
	qson_run(_handle_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_object_entry_number(qson_serialize_ctx_t c, char *key, double value, bool has_next) {
	if (c->state != QSON_SERIALIZE_STATE_OBJECT) return QSON_RESULT_INVALID_STATE;
	qson_run(_write_object_entry(c, key));
	qson_run(qson_serialize_number(c, value));
	qson_run(_handle_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_object_entry_subctx(qson_serialize_ctx_t c, char *key, qson_serialize_ctx_t *sub_ctx) {
	if (c->state != QSON_SERIALIZE_STATE_OBJECT) return QSON_RESULT_INVALID_STATE;
	qson_run(_write_object_entry(c, key));
	qson_run(qson_serialize_ctx_create_subctx(c, sub_ctx));
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_object_entry_subctx_end(qson_serialize_ctx_t c, qson_serialize_ctx_t sc, bool has_next) {
	if (c->state != QSON_SERIALIZE_STATE_SUBCTX) return QSON_RESULT_INVALID_STATE;
	if (sc->state != QSON_SERIALIZE_STATE_NONE) return QSON_RESULT_INVALID_STATE;
	c->index = sc->index;
	qfree(sc, sc);
	qson_run(_handle_has_next(c, has_next));
	return QSON_RESULT_OK;
}

qson_result_t qson_serialize_object_end(qson_serialize_ctx_t c) {
	if (c->state != QSON_SERIALIZE_STATE_OBJECT) return QSON_RESULT_INVALID_STATE;
	qson_run(_handle_has_next(c, false));
	return QSON_RESULT_OK;
}

