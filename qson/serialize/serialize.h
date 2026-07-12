#ifndef _qson_serialize_h_
#define _qson_serialize_h_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Current state of serialization
 */
typedef enum {
	QSON_SERIALIZE_STATE_NONE = 0,		// no specific serialization is running
	QSON_SERIALIZE_STATE_OBJECT = 1,	// writing an object
	QSON_SERIALIZE_STATE_ARRAY = 2,		// writing an object
	QSON_SERIALIZE_STATE_SUBCTX = 3,	// a subctx is active
} qson_serialize_state;

/*
 * Context for serilizing a json
 */
typedef void* qson_serialize_ctx_t;

#define QSON_SERIALIZE_CTX_FLAG_IS_SUBCTX	1	// Indicates current ctx is subctx of another ctx

/*
 * Create a serialize context for given byte buffer
 */
qson_result_t qson_create_serialize_ctx(qson_serialize_ctx_t *ctx, char *buffer, int size);

/*
 * Finish buffer with a null terminator
 * Requires state NONE
 */
qson_result_t qson_end_serialize_ctx(qson_serialize_ctx_t ctx);

/*
 * Writes a string value with needed escapes and with qutations
 * Ignores state
 */
qson_result_t qson_write_string(qson_serialize_ctx_t ctx, char *value);

/*
 * Writes a number value
 * Ignores state
 */
qson_result_t qson_write_number(qson_serialize_ctx_t ctx, double value);

/*
 * Create a subctx for current ctx
 * Subctx will contains ctx flags plus IS_CTX flag set
 * Sets ctx state to SUBCTX
 * Ignores state
 */
qson_result_t qson_create_sub_serialize_ctx(qson_serialize_ctx_t ctx, qson_serialize_ctx_t *sub_ctx);

#ifdef __cplusplus
}
#endif

#include "object.h"
#include "array.h"

#endif
