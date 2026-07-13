#ifndef _qson_serialize_object_h_
#define _qson_serialize_object_h_
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Start writing an object and set context state to OBJECT
 * Requires state NONE
 */
qson_result_t qson_serialize_object(qson_serialize_ctx_t ctx);

/*
 * Add an object entry with value type of string
 * if "has_next" writes chars needed for next entry
 * and if not writes chars needed for object end and sets state to NONE
 * Requires state OBJECT
 */
qson_result_t qson_serialize_object_entry_string(qson_serialize_ctx_t ctx, char *key, char *value, bool has_next);

/*
 * Add an object entry with value type of null
 * if "has_next" writes chars needed for next entry
 * and if not writes chars needed for object end and sets state to NONE
 * Requires state OBJECT
 */
qson_result_t qson_serialize_object_entry_null(qson_serialize_ctx_t ctx, char *key, bool has_next);

/*
 * Add an object entry with value type of boolean
 * if "has_next" writes chars needed for next entry
 * and if not writes chars needed for object end and sets state to NONE
 * Requires state OBJECT
 */
qson_result_t qson_serialize_object_entry_bool(qson_serialize_ctx_t ctx, char *key, bool value, bool has_next);

/*
 * Add an object entry with value type of number
 * if "has_next" writes chars needed for next entry
 * and if not writes chars needed for object end and sets state to NONE
 * Requires state OBJECT
 */
qson_result_t qson_serialize_object_entry_number(qson_serialize_ctx_t ctx, char *key, double value, bool has_next);


/*
 * Create sub ctx to write an array or object
 * Sets state SUBCTX
 * Requires state OBJECT
 */
qson_result_t qson_serialize_object_entry_subctx(qson_serialize_ctx_t ctx, char *key, qson_serialize_ctx_t *sub_ctx);

/*
 * End created sub ctx
 * if "has_next" writes chars needed for next entry
 * and if not writes chars needed for object end and sets state to NONE
 * Requires state SUBCTX on ctx and state NONE on sub ctx
 */
qson_result_t qson_serialize_object_entry_subctx_end(qson_serialize_ctx_t ctx, qson_serialize_ctx_t sub_ctx, bool has_next);

/*
 * End the object
 * Sets state to NONE
 * Requires state OBJECT
 */
qson_result_t qson_serialize_object_end(qson_serialize_ctx_t ctx);

#ifdef __cplusplus
}
#endif
#endif

