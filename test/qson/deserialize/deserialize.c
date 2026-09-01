#include <test/qson/deserialize/deserialize.internal.h>

bool test_qson_deserialize_skip_white_spaces() {
	test_run_log("qson_deserialize_skip_white_spaces");
	char buffer[] = "\t\n\r test";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_skip_white_spaces(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_index(ctx) == 4;
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_skip_white_spacesـunexpected_eof() {
	test_run_log("qson_deserialize_skip_white_spaces_unexpected_eof");
	char buffer[] = "\t\n\r ";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_skip_white_spaces(ctx) == QSON_RESULT_UNEXPECTED_EOF;
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

static qson_result_t _test_qson_ctx_size_check(qson_deserialize_ctx_t ctx) {
	qson_ctx_size_check(ctx, 1);
	return QSON_RESULT_OK;
}

bool test_qson_ctx_size_check() {
	test_run_log("qson_ctx_size_check");
	char buffer[] = "123";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, 4) == QSON_RESULT_OK;
	ctx->index = 3;
	success &= _test_qson_ctx_size_check(ctx) == QSON_RESULT_UNEXPECTED_EOF;
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;

}

bool test_qson_deserialize_skip_white_spacesـline_comment() {
	test_run_log("qson_deserialize_skip_white_spaces_line_comment");
	char buffer[] = "// just a comment\nt";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_skip_white_spaces(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_index(ctx) == 18;
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_skip_white_spacesـarea_comment() {
	test_run_log("qson_deserialize_skip_white_spaces_area_comment");
	char buffer[] = "/* just a comment */t";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_skip_white_spaces(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_index(ctx) == 20;
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_string_skip() {
	test_run_log("qson_deserialize_string_skip");
	char buffer[] = "\"ignored string :( \\\" \"w";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_string_skip(ctx) == QSON_RESULT_OK;
	success &= buffer[qson_deserialize_ctx_index(ctx)] == 'w';
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_bool_skip() {
	test_run_log("qson_deserialize_bool_skip");
	char buffer[] = "truew";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_bool_skip(ctx) == QSON_RESULT_OK;
	success &= buffer[qson_deserialize_ctx_index(ctx)] == 'w';
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_number_skip() {
	test_run_log("qson_deserialize_number_skip");
	char buffer[] = "-1.23E+1w";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_number_skip(ctx) == QSON_RESULT_OK;
	success &= buffer[qson_deserialize_ctx_index(ctx)] == 'w';
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_auto_skip() {
	test_run_log("qson_deserialize_auto_skip");
	char buffer[] = "truew";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_auto_skip(ctx) == QSON_RESULT_OK;
	success &= buffer[qson_deserialize_ctx_index(ctx)] == 'w';
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	test_result_log(success);
	return success;
}

bool test_qson_deserialize_string_auto() {
	test_run_log("qson_deserialize_string_auto");
	qson_deserialize_ctx_t ctx;
	char buffer[] = "\"wtf is this shit?\"w";
	char valid_res[] = "wtf is this shit?";
	size_t valid_res_size = array_len(valid_res);
	char *res;
	size_t res_size;

	bool success = 1;
	success &= qson_deserialize_ctx_create(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_deserialize_string_auto(ctx, &res, &res_size) == QSON_RESULT_OK;
	success &= res_size == valid_res_size;
	success &= strcmp(res, valid_res) == 0;
	success &= qson_deserialize_ctx_destroy(ctx) == QSON_RESULT_OK;
	free(res);
	test_result_log(success);
	return success;
}

extern bool test_qson_deserialize_array();
extern bool test_qson_deserialize_object();

bool test_qson_deserialize() {
	bool success = 1;
	success &= test_qson_deserialize_skip_white_spaces();
	success &= test_qson_deserialize_skip_white_spacesـunexpected_eof();
	success &= test_qson_ctx_size_check();
	success &= test_qson_deserialize_skip_white_spacesـline_comment();
	success &= test_qson_deserialize_skip_white_spacesـarea_comment();
	success &= test_qson_deserialize_string_skip();
	success &= test_qson_deserialize_bool_skip();
	success &= test_qson_deserialize_number_skip();
	success &= test_qson_deserialize_auto_skip();
	success &= test_qson_deserialize_string_auto();
	success &= test_qson_deserialize_array();
	success &= test_qson_deserialize_object();
	return success;
}

