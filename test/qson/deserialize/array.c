#include <test/qson/deserialize/deserialize.internal.h>

bool test_qson_start_array() {
	test_run_log("qson_start_array");
	char buffer[] = "[ null ]";
	char buffer2[] = "[ ]";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_start_array(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_state(ctx) == QSON_DESERIALIZING_STATE_ARRAY;
	success &= qson_deserialize_ctx_create(&ctx, buffer2, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_start_array(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_state(ctx) == QSON_DESERIALIZING_STATE_NONE;
	test_result_log(success);
	return success;
}

bool test_qson_get_array_entry() {
	test_run_log("qson_get_array_entry");
	char buffer[] = "[ \"value\" ]";
	qson_deserialize_ctx_t ctx;
	char key[6];
	int key_size = 6;
	qson_type_t value_type = QSON_TYPE_STRING;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_start_array(ctx) == QSON_RESULT_OK;
	success &= qson_get_array_entry(ctx, &value_type) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

bool test_qson_get_array_entry_value_string() {
	test_run_log("qson_get_array_entry_value_string");
	char buffer[] = "[ \"string value\"  ]";
	qson_deserialize_ctx_t ctx;
	qson_type_t value_type = QSON_TYPE_STRING;
	char value[13];
	int value_size = 13;
	bool has_next;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_start_array(ctx) == QSON_RESULT_OK;
	success &= qson_get_array_entry(ctx, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_string(ctx, value, &value_size, &has_next) == QSON_RESULT_OK;
	success &= value_size == 13;
	success &= strcmp(value, "string value") == 0;
	success &= !has_next;
	test_result_log(success);
	return success;
}

bool test_qson_get_array_entry_value_bool() {
	test_run_log("qson_get_array_entry_value_bool");
	char buffer[] = "[ true , false ]";
	qson_deserialize_ctx_t ctx;
	qson_type_t value_type = QSON_TYPE_BOOL;
	bool value;
	bool has_next;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_start_array(ctx) == QSON_RESULT_OK;

	// First entry
	success &= qson_get_array_entry(ctx, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_bool(ctx, &value, &has_next) == QSON_RESULT_OK;
	success &= value;
	success &= has_next;

	// Second entry
	success &= qson_get_array_entry(ctx, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_bool(ctx, &value, &has_next) == QSON_RESULT_OK;
	success &= !value;
	success &= !has_next;

	test_result_log(success);
	return success;
}

bool test_qson_get_array_entry_value_null() {
	test_run_log("qson_get_array_entry_value_null");
	char buffer[] = "[ null ]";
	qson_deserialize_ctx_t ctx;
	qson_type_t value_type = QSON_TYPE_NULL;
	bool has_next;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_start_array(ctx) == QSON_RESULT_OK;
	success &= qson_get_array_entry(ctx, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_null(ctx, &has_next) == QSON_RESULT_OK;
	success &= !has_next;
	test_result_log(success);
	return success;
}

bool test_qson_get_array_entry_value_number() {
	test_run_log("qson_get_array_entry_value_number");
	char buffer[] = "[  1E+2  ]";
	qson_deserialize_ctx_t ctx;
	qson_type_t value_type = QSON_TYPE_NUMBER;
	double value;
	bool has_next;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_start_array(ctx) == QSON_RESULT_OK;
	success &= qson_get_array_entry(ctx, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_number(ctx, &value, &has_next) == QSON_RESULT_OK;
	success &= value == 100;
	success &= !has_next;
	test_result_log(success);
	return success;
}

bool test_qson_get_array_entry_value_sub_ctx() {
	test_run_log("qson_get_array_entry_value_sub_ctx");
	char buffer[] = "[ [ \"]\\\"]\", [ true ] ] ]";
	qson_deserialize_ctx_t ctx;
	qson_type_t value_type = QSON_TYPE_ARRAY;
	bool has_next;
	qson_deserialize_ctx_t sub_ctx;
	char value[4];
	int value_size = 4;
	qson_deserialize_ctx_t sub_ctx2;
	bool bvalue;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_start_array(ctx) == QSON_RESULT_OK;
	success &= qson_get_array_entry(ctx, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_sub_ctx(ctx, &sub_ctx) == QSON_RESULT_OK;

	success &= qson_start_array(sub_ctx) == QSON_RESULT_OK;
	value_type = QSON_TYPE_STRING;
	success &= qson_get_array_entry(sub_ctx, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_string(sub_ctx, value, &value_size, &has_next) == QSON_RESULT_OK;
	success &= strcmp(value, "]\"]") == 0;
	success &= value_size == 4;
	success &= has_next;

	value_type = QSON_TYPE_ARRAY;
	success &= qson_get_array_entry(sub_ctx, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_sub_ctx(sub_ctx, &sub_ctx2) == QSON_RESULT_OK;
	success &= qson_start_array(sub_ctx2) == QSON_RESULT_OK;
	value_type = QSON_TYPE_BOOL;
	success &= qson_get_array_entry(sub_ctx2, &value_type) == QSON_RESULT_OK;
	success &= qson_get_array_entry_value_bool(sub_ctx2, &bvalue, &has_next) == QSON_RESULT_OK;
	success &= bvalue;
	success &= !has_next;
	success &= qson_get_array_entry_value_sub_ctx_end(sub_ctx, sub_ctx2, &has_next) == QSON_RESULT_OK;
	success &= !has_next;
	success &= qson_get_array_entry_value_sub_ctx_end(ctx, sub_ctx, &has_next) == QSON_RESULT_OK;
	success &= !has_next;

	test_result_log(success);
	return success;
}

bool test_qson_deserialize_array() {
	bool success = 1;
	success &= test_qson_start_array();
	success &= test_qson_get_array_entry();
	success &= test_qson_get_array_entry_value_string();
	success &= test_qson_get_array_entry_value_bool();
	success &= test_qson_get_array_entry_value_null();
	success &= test_qson_get_array_entry_value_number();
	success &= test_qson_get_array_entry_value_sub_ctx();
	return success;	
}
