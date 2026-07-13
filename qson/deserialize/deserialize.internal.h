/*
 * Its not part of qson API its for internal use only
 */

#ifndef _qson_deserialize_internal_h_
#define _qson_deserialize_internal_h_

#include <stdlib.h>
#include <stdio.h>
#include <qson/qson.internal.h>

#ifdef __cplusplus
extern "C" {
#endif

struct qson_deserialize_ctx {
	char *buffer;	// Buffer which contains the json
	int size;	// Size of the buffer
	int index;	// Current index in buffer
	qson_deserialize_state_t state;	// Current state of deserialization
	char flags;	// flags for current ctx
};

#define qson_ctx_has_size(ctx, required_size) (ctx->size - ctx->index - 1) < required_size || ctx->buffer[ctx->index + required_size] == '\0'
#define qson_ctx_size_check(ctx, required_size) \
	do { \
		if (qson_ctx_has_size(ctx, required_size)) \
			return QSON_RESULT_UNEXPECTED_EOF; \
	} while (0)
#define qson_ctx_skip(ctx, amount) qson_ctx_size_check(ctx, amount); ctx->index += amount;

inline qson_result_t _qson_deserialize_skip_white_spaces(struct qson_deserialize_ctx *ctx) {
	char chr = ctx->buffer[ctx->index];
	int comment_state = 0;	// 0: not in comment,
				// 1: unknown comment (type must be detected by next char),
				// 2: line comment "//",
				// 3: area comment "/*"
				// 3: possible area comment end "*/"
	while (qson_is_white_space(chr) || comment_state != 0 || chr == QSON_COMMENT_PREFIX) {
		switch (comment_state) {
		case 0:
			if (chr == QSON_COMMENT_PREFIX) comment_state = 1;
			break;
		case 1:
			switch (chr) {
			case QSON_COMMENT_LINE: comment_state = 2; break;
			case QSON_COMMENT_AREA: comment_state = 3; break;
			default: return QSON_RESULT_INVALID_CHAR;
			}
			break;
		case 2:
			if (chr == '\n') comment_state = 0;
			break;
		case 3:
			if (chr == QSON_COMMENT_AREA) comment_state = 4;
			break;
		case 4:
			switch (chr) {
			case QSON_COMMENT_PREFIX: comment_state = 0; break;
			default: comment_state = 3; break;
			}
			break;
		}
		qson_ctx_size_check(ctx, 1);
		chr = ctx->buffer[++ctx->index];
	}
	return QSON_RESULT_OK;
}

/*
 * Detect current data type
 * If current given type is AUTO set detected type
 * otherwize check if given type is equal to detected type and if not return QSON_RESULT_INVALID_TYPE
 */
inline qson_result_t _qson_detect_type(struct qson_deserialize_ctx *ctx, qson_type_t *type) {
	qson_type_t dtype;
	char chr = ctx->buffer[ctx->index];
	switch (chr) {
	case QSON_QUOTATION_MARK: dtype = QSON_TYPE_STRING; break;
	case QSON_BEGIN_OBJECT: dtype = QSON_TYPE_OBJECT; break;
	case QSON_BEGIN_ARRAY: dtype = QSON_TYPE_ARRAY; break;
	case 't':
	case 'f': dtype = QSON_TYPE_BOOL; break;
	case 'n': dtype = QSON_TYPE_NULL; break;
	default:
		if (('0' <= chr && chr <= '9' ) || chr == '-')
			dtype = QSON_TYPE_NUMBER;
		else
			return QSON_RESULT_INVALID_CHAR;
	}

	if (*type == QSON_TYPE_AUTO || *type == dtype) {
		*type = dtype;
		return QSON_RESULT_OK;
	}
	return QSON_RESULT_INVALID_TYPE;
}

#ifdef __cplusplus
}
#endif
#endif
