#include <test/qson/serialize/serialize.internal.h>

bool test_qson_write_array() {
	test_run_log("qson_write_array");
	char buffer[3];
	qson_serialize_ctx_t ctx;

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_write_array(ctx) == QSON_RESULT_OK;
	success &= qson_serialize_array_end(ctx) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp(buffer, "[]\0") == 0;
	test_result_log(success);
	return success;
}

bool test_qson_write_array_entry_string() {
	test_run_log("qson_write_array_entry_string");
	qson_serialize_ctx_t ctx;
	char buffer[30];
	char *value = "value";

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_write_array(ctx) == QSON_RESULT_OK;
	success &= qson_write_array_entry_string(ctx, value, true) == QSON_RESULT_OK;
	success &= qson_write_array_entry_string(ctx, value, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp("[\"value\",\"value\"]", buffer) == 0;
	test_result_log(success);
	return success;
}

bool test_qson_write_array_entry_null() {
	test_run_log("qson_write_array_entry_null");
	qson_serialize_ctx_t ctx;
	char buffer[7];

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_write_array(ctx) == QSON_RESULT_OK;
	success &= qson_write_array_entry_null(ctx, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp("[null]", buffer) == 0;
	test_result_log(success);
	return success;
}

bool test_qson_write_array_entry_bool() {
	test_run_log("qson_write_array_entry_bool");
	qson_serialize_ctx_t ctx;
	char buffer[13];

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_write_array(ctx) == QSON_RESULT_OK;
	success &= qson_write_array_entry_bool(ctx, true, true) == QSON_RESULT_OK;
	success &= qson_write_array_entry_bool(ctx, false, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp("[true,false]", buffer) == 0;
	test_result_log(success);
	return success;
}

bool test_qson_write_array_entry_number() {
	test_run_log("qson_write_array_entry_number");
	qson_serialize_ctx_t ctx;
	char buffer[12];

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_write_array(ctx) == QSON_RESULT_OK;
	success &= qson_write_array_entry_number(ctx, -1.23, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp("[-1.230000]", buffer) == 0;
	test_result_log(success);
	return success;
}

bool test_qson_write_array_entry_subctx() {
	test_run_log("qson_write_array_entry_subctx");
	qson_serialize_ctx_t ctx;
	qson_serialize_ctx_t subctx;
	char buffer[9];

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_write_array(ctx) == QSON_RESULT_OK;

	success &= qson_write_array_entry_subctx(ctx, &subctx) == QSON_RESULT_OK;
	success &= qson_write_array(subctx) == QSON_RESULT_OK;
	success &= qson_write_array_entry_bool(subctx, true, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(subctx) == QSON_RESULT_OK;
	success &= qson_write_array_entry_subctx_end(ctx, subctx, false) == QSON_RESULT_OK;

	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp(buffer, "[[true]]") == 0;
	test_result_log(success);
	return success;
}

bool test_qson_serialize_array() {
	bool success = 1;
	success &= test_qson_write_array();
	success &= test_qson_write_array_entry_string();
	success &= test_qson_write_array_entry_null();
	success &= test_qson_write_array_entry_bool();
	success &= test_qson_write_array_entry_number();
	success &= test_qson_write_array_entry_subctx();
	return success;
}

