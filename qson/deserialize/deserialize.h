#ifndef _qson_deserialize_h_
#define _qson_deserialize_h_

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Current state of deserialization
 */
typedef enum {
	QSON_DESERIALIZING_STATE_NONE = 0,		// no specific deserialization is running
	QSON_DESERIALIZING_STATE_OBJECT = 1,		// deserializing an object
	QSON_DESERIALIZING_STATE_OBJECT_VALUE = 2,	// deserializing value of an object
	QSON_DESERIALIZING_STATE_ARRAY = 3,		// deserializing an array
	QSON_DESERIALIZING_STATE_ARRAY_VALUE = 4,	// deserializing memeber of an array
	QSON_DESERIALIZING_STATE_SUBCTX = 5,		// a sub ctx is active for this ctx
} qson_deserialize_state;

/*
 * Context for deserilizing a json
 */
typedef void* qson_deserialize_ctx_t;

#define QSON_DESERIALIZE_CTX_FLAG_IS_SUBCTX	1	// Indicates current ctx is subctx of another ctx

/*
 * Create a deserialize context for given byte buffer
 */
qson_result_t qson_create_deserialize_ctx(qson_deserialize_ctx_t *ctx, char *buffer, int size);

//
// Check if given char is a white space in a JSON
//
bool qson_is_white_space(char chr);

//
// Skip charachters until reach a non white space char
// Returns UNEXPECTED_EOF if no non-whitespace char is found before buffer end
//
qson_result_t qson_skip_white_spaces(qson_deserialize_ctx_t ctx);

/*
 * Read a string in current context
 * Ignores state
 */
qson_result_t qson_read_string(qson_deserialize_ctx_t ctx, char *buffer, int *size);

/*
 * Read a boolean value in current context
 * Ignores state
 */
qson_result_t qson_read_bool(qson_deserialize_ctx_t ctx, bool *value);

/*
 * Skip null value
 * Ignores state
 */
qson_result_t qson_skip_null(qson_deserialize_ctx_t ctx);


/*
 * Read a number value in current context and return it as a signed double
 * Ignores state
 */
qson_result_t qson_read_number(qson_deserialize_ctx_t ctx, double *value);

/*
 * Create a deserialize context that deserialize an object or and array nested in current context
 * Sets state to SUBCTX
 * Ignores state
 */
qson_result_t qson_create_sub_deserialize_ctx(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t *sub_ctx);

/*
 * Add subctx index to ctx (changing state is caller responsibility)
 * Requires state SUBCTX and state NONE in subctx
 */
qson_result_t qson_end_sub_deserialize_ctx(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t sub_ctx);

/*
 * Returns current index of ctx
 */
int qson_deserialize_ctx_index(qson_deserialize_ctx_t ctx);

/*
 * Returns current state of ctx
 */
qson_deserialize_state qson_deserialize_ctx_state(qson_deserialize_ctx_t ctx);

#ifdef __cplusplus
}
#endif

#include "object.h"
#include "array.h"

#endif

