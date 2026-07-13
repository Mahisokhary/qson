#include <test/qson/serialize/serialize.internal.h>

bool _test_qson_serialize_object() {
	test_run_log("qson_serialize_object");
	char buffer[3];
	qson_serialize_ctx_t ctx;

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_serialize_object(ctx) == QSON_RESULT_OK;
	success &= qson_serialize_object_end(ctx) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp(buffer, "{}\0") == 0;
	test_result_log(success);
	return success;
}

bool test_qson_serialize_object_entry_string() {
	test_run_log("qson_serialize_object_entry_string");
	qson_serialize_ctx_t ctx;
	char buffer[30];
	char *key = "key";
	char *value = "value";

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_serialize_object(ctx) == QSON_RESULT_OK;
	success &= qson_serialize_object_entry_string(ctx, key, value, true) == QSON_RESULT_OK;
	success &= qson_serialize_object_entry_string(ctx, key, value, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp("{\"key\":\"value\",\"key\":\"value\"}", buffer) == 0;
	test_result_log(success);
	return success;
}

bool test_qson_serialize_object_entry_null() {
	test_run_log("qson_serialize_object_entry_null");
	qson_serialize_ctx_t ctx;
	char buffer[150];
	char *key = "key";

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_serialize_object(ctx) == QSON_RESULT_OK;
	success &= qson_serialize_object_entry_null(ctx, key, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp("{\"key\":null}", buffer) == 0;
	test_result_log(success);
	return success;
}

bool test_qson_serialize_object_entry_bool() {
	test_run_log("qson_serialize_object_entry_bool");
	qson_serialize_ctx_t ctx;
	char buffer[25];
	char *key = "key";

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_serialize_object(ctx) == QSON_RESULT_OK;
	success &= qson_serialize_object_entry_bool(ctx, key, true, true) == QSON_RESULT_OK;
	success &= qson_serialize_object_entry_bool(ctx, key, false, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp("{\"key\":true,\"key\":false}", buffer) == 0;
	test_result_log(success);
	return success;
}

bool test_qson_serialize_object_entry_number() {
	test_run_log("qson_serialize_object_entry_number");
	qson_serialize_ctx_t ctx;
	char buffer[18];
	char *key = "key";

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_serialize_object(ctx) == QSON_RESULT_OK;
	success &= qson_serialize_object_entry_number(ctx, key, -1.23, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp("{\"key\":-1.230000}", buffer) == 0;
	test_result_log(success);
	return success;
}

bool test_qson_serialize_object_entry_subctx() {
	test_run_log("qson_serialize_object_entry_subctx");
	qson_serialize_ctx_t ctx;
	qson_serialize_ctx_t subctx;
	char buffer[21];
	char *key = "key";

	bool success = 1;
	success &= qson_serialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_serialize_object(ctx) == QSON_RESULT_OK;

	success &= qson_serialize_object_entry_subctx(ctx, key, &subctx) == QSON_RESULT_OK;
	success &= qson_serialize_object(subctx) == QSON_RESULT_OK;
	success &= qson_serialize_object_entry_bool(subctx, key, true, false) == QSON_RESULT_OK;
	success &= qson_serialize_ctx_end(subctx) == QSON_RESULT_OK;
	success &= qson_serialize_object_entry_subctx_end(ctx, subctx, false) == QSON_RESULT_OK;

	success &= qson_serialize_ctx_end(ctx) == QSON_RESULT_OK;
	success &= strcmp(buffer, "{\"key\":{\"key\":true}}") == 0;
	test_result_log(success);
	return success;
}

bool test_qson_serialize_object() {
	bool success = 1;
	success &= _test_qson_serialize_object();
	success &= test_qson_serialize_object_entry_string();
	success &= test_qson_serialize_object_entry_null();
	success &= test_qson_serialize_object_entry_bool();
	success &= test_qson_serialize_object_entry_number();
	success &= test_qson_serialize_object_entry_subctx();
	return success;
}

