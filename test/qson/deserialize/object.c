#include <test/qson/deserialize/deserialize.internal.h>

bool test_qson_deserialize_object_start() {
	test_run_log("qson_deserialize_object_start");
	char buffer[] = "{ \"test\": true }";
	char buffer2[] = "{ }";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_state(ctx) == QSON_DESERIALIZING_STATE_OBJECT;
	success &= qson_deserialize_ctx_create(&ctx, buffer2, array_len(buffer2)) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_state(ctx) == QSON_DESERIALIZING_STATE_NONE;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_object_entry() {
	test_run_log("qson_deserialize_object_entry");
	char buffer[] = "{ \"key\\u2764\" : \"string value\"  }";
	qson_deserialize_ctx_t ctx;
	char key[10];
	int key_size = 10;
	qson_type_t value_type = QSON_TYPE_AUTO;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_object_entry(ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 7;
	success &= strcmp(key, "key\u2764") == 0;
	success &= value_type == QSON_TYPE_STRING;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_object_entry_value_string() {
	test_run_log("qson_deserialize_object_entry_value_string");
	char buffer[] = "{ \"key\" : \"string value\"  }";
	qson_deserialize_ctx_t ctx;
	char key[4];
	int key_size = 4;
	qson_type_t value_type = QSON_TYPE_AUTO;
	char value[13];
	int value_size = 13;
	bool has_next;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_object_entry(ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "key") == 0;
	success &= value_type == QSON_TYPE_STRING;
	success &= qson_deserialize_object_entry_value_string(ctx, value, &value_size, &has_next) == QSON_RESULT_OK;
	success &= value_size == 13;
	success &= strcmp(value, "string value") == 0;
	success &= !has_next;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_object_entry_value_bool() {
	test_run_log("qson_deserialize_object_entry_value_bool");
	char buffer[] = "{ \"key\" : true , \"kir\": false }";
	qson_deserialize_ctx_t ctx;
	char key[4];
	int key_size = 4;
	qson_type_t value_type = QSON_TYPE_AUTO;
	bool value;
	bool has_next;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(ctx) == QSON_RESULT_OK;

	// First entry
	success &= qson_deserialize_object_entry(ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "key") == 0;
	success &= value_type == QSON_TYPE_BOOL;
	success &= qson_deserialize_object_entry_value_bool(ctx, &value, &has_next) == QSON_RESULT_OK;
	success &= value;
	success &= has_next;

	// Second entry
	success &= qson_deserialize_object_entry(ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "kir") == 0;
	success &= value_type == QSON_TYPE_BOOL;
	success &= qson_deserialize_object_entry_value_bool(ctx, &value, &has_next) == QSON_RESULT_OK;
	success &= !value;
	success &= !has_next;

	test_result_log(success);
	return success;
}

bool test_qson_deserialize_object_entry_value_null() {
	test_run_log("qson_deserialize_object_entry_value_null");
	char buffer[] = "{ \"key\" : null  }";
	qson_deserialize_ctx_t ctx;
	char key[4];
	int key_size = 4;
	qson_type_t value_type = QSON_TYPE_AUTO;
	bool has_next;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_object_entry(ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "key") == 0;
	success &= value_type == QSON_TYPE_NULL;
	success &= qson_deserialize_object_entry_value_null(ctx, &has_next) == QSON_RESULT_OK;
	success &= !has_next;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_object_entry_value_number() {
	test_run_log("qson_deserialize_object_entry_value_number");
	char buffer[] = "{ \"key\" : 1E+1  }";
	qson_deserialize_ctx_t ctx;
	char key[4];
	int key_size = 4;
	qson_type_t value_type = QSON_TYPE_AUTO;
	double value;
	bool has_next;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_object_entry(ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "key") == 0;
	success &= value_type == QSON_TYPE_NUMBER;
	success &= qson_deserialize_object_entry_value_number(ctx, &value, &has_next) == QSON_RESULT_OK;
	success &= value == 10;
	success &= !has_next;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_object_entry_value_sub_ctx() {
	test_run_log("qson_deserialize_object_entry_value_sub_ctx");
	char buffer[] = "{ \"key\" : { \"sik\" : \"}\\\"}\", \"key\": { \"sik\": true } }  }";
	qson_deserialize_ctx_t ctx;
	char key[4];
	int key_size = 4;
	qson_type_t value_type = QSON_TYPE_OBJECT;
	bool has_next;
	qson_deserialize_ctx_t sub_ctx;
	char value[4];
	int value_size = 4;
	qson_deserialize_ctx_t sub_ctx2;
	bool bvalue;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_object_entry(ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "key") == 0;
	success &= qson_deserialize_object_entry_value_sub_ctx(ctx, &sub_ctx) == QSON_RESULT_OK;

	success &= qson_deserialize_object_start(sub_ctx) == QSON_RESULT_OK;
	value_type = QSON_TYPE_STRING;
	success &= qson_deserialize_object_entry(sub_ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "sik") == 0;
	success &= qson_deserialize_object_entry_value_string(sub_ctx, value, &value_size, &has_next) == QSON_RESULT_OK;
	success &= strcmp(value, "}\"}") == 0;
	success &= value_size == 4;
	success &= has_next;

	value_type = QSON_TYPE_OBJECT;
	success &= qson_deserialize_object_entry(sub_ctx, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "key") == 0;
	success &= qson_deserialize_object_entry_value_sub_ctx(sub_ctx, &sub_ctx2) == QSON_RESULT_OK;
	success &= qson_deserialize_object_start(sub_ctx2) == QSON_RESULT_OK;
	
	value_type = QSON_TYPE_BOOL;
	success &= qson_deserialize_object_entry(sub_ctx2, key, &key_size, &value_type) == QSON_RESULT_OK;
	success &= key_size == 4;
	success &= strcmp(key, "sik") == 0;
	success &= qson_deserialize_object_entry_value_bool(sub_ctx2, &bvalue, &has_next) == QSON_RESULT_OK;
	success &= bvalue;
	success &= !has_next;
	success &= qson_deserialize_object_entry_value_sub_ctx_end(sub_ctx, sub_ctx2, &has_next) == QSON_RESULT_OK;
	success &= !has_next;
	success &= qson_deserialize_object_entry_value_sub_ctx_end(ctx, sub_ctx, &has_next) == QSON_RESULT_OK;
	success &= !has_next;

	test_result_log(success);
	return success;
}

bool test_qson_deserialize_object() {
	bool success = 1;
	success &= test_qson_deserialize_object_start();
	success &= test_qson_deserialize_object_entry();
	success &= test_qson_deserialize_object_entry_value_string();
	success &= test_qson_deserialize_object_entry_value_bool();
	success &= test_qson_deserialize_object_entry_value_null();
	success &= test_qson_deserialize_object_entry_value_number();
	success &= test_qson_deserialize_object_entry_value_sub_ctx();
	return success;
}

