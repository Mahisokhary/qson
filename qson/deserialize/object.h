#ifndef _qson_deserialize_object_h_
#define _qson_deserialize_object_h_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Start deserializing an object and set context state to OBJECT
 * set context state to OBJECT if has entry and set to NONE if not
 * Requires state NONE
 */
qson_result_t qson_deserialize_object_start(qson_deserialize_ctx_t ctx);

/*
 * Start deserializing a entry of an object and set context state to OBJECT_VALUE
 * Requires state OBJECT
 */
qson_result_t qson_deserialize_object_entry(qson_deserialize_ctx_t ctx, char *key, int *key_length, qson_type_t *type);

/*
 * Read value of entry as a string
 * set context state to OBJECT if has next entry and set to NONE if not
 * Requires state OBJECT_VALUE
 */
qson_result_t qson_deserialize_object_entry_value_string(qson_deserialize_ctx_t ctx, char *value, int *value_length, bool *has_next);

/*
 * Read value of entry as boolean
 * set context state to OBJECT if has next entry and set to NONE if not
 * Requires state OBJECT_VALUE
 */
qson_result_t qson_deserialize_object_entry_value_bool(qson_deserialize_ctx_t ctx, bool *value, bool *has_next);

/*
 * Just skip the null value
 * set context state to OBJECT if has next entry and set to NONE if not
 * Requires state OBJECT_VALUE
 */
qson_result_t qson_deserialize_object_entry_value_null(qson_deserialize_ctx_t ctx, bool *has_next);

/*
 * Read value of entry as double
 * set context state to OBJECT if has next entry and set to NONE if not
 * Requires state OBJECT_VALUE
 */
qson_result_t qson_deserialize_object_entry_value_number(qson_deserialize_ctx_t ctx, double *value, bool *has_next);

/*
 * Create a sub context to read array or object
 * Requires state OBJECT_VALUE
 */
qson_result_t qson_deserialize_object_entry_value_sub_ctx(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t sub_ctx);

/*
 * End created subctx
 * set context state to OBJECT if has next entry and set to NONE if not
 * Requires state SUBCTX and state NONE in subctx
 */
qson_result_t qson_deserialize_object_entry_value_sub_ctx_end(qson_deserialize_ctx_t ctx, qson_deserialize_ctx_t sub_ctx, bool *has_next);

#ifdef __cplusplus
}
#endif
#endif
