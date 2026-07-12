#include <test/qson/deserialize/deserialize.internal.h>

bool test_qson_skip_white_spaces() {
	test_run_log("qson_skip_white_spaces");
	char buffer[] = "\t\n\r test";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_create_deserialize_ctx(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_skip_white_spaces(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_index(ctx) == 4;
	test_result_log(success);
	return success;
}

bool test_qson_skip_white_spacesـunexpected_eof() {
	test_run_log("qson_skip_white_spaces_unexpected_eof");
	char buffer[] = "\t\n\r ";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_create_deserialize_ctx(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_skip_white_spaces(ctx) == QSON_RESULT_UNEXPECTED_EOF;
	test_result_log(success);
	return success;
}

static qson_result_t _test_qson_ctx_size_check(struct qson_deserialize_ctx *ctx) {
	qson_ctx_size_check(ctx, 1);
	return QSON_RESULT_OK;
}

bool test_qson_ctx_size_check() {
	test_run_log("qson_ctx_size_check");
	char buffer[] = "testing";
	struct qson_deserialize_ctx *ctx;

	bool success = 1;
	success &= qson_create_deserialize_ctx((void**) &ctx, buffer, 4) == QSON_RESULT_OK;
	ctx->index = 3;
	success &= _test_qson_ctx_size_check(ctx) == QSON_RESULT_UNEXPECTED_EOF;
	test_result_log(success);
	return success;

}

bool test_qson_skip_white_spacesـline_comment() {
	test_run_log("qson_skip_white_spaces_line_comment");
	char buffer[] = "// just a comment\nt";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_create_deserialize_ctx(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_skip_white_spaces(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_index(ctx) == 18;
	test_result_log(success);
	return success;
}

bool test_qson_skip_white_spacesـarea_comment() {
	test_run_log("qson_skip_white_spaces_area_comment");
	char buffer[] = "/* just a comment */t";
	qson_deserialize_ctx_t ctx;

	bool success = 1;
	success &= qson_create_deserialize_ctx(&ctx, buffer, array_len(buffer)) == QSON_RESULT_OK;
	success &= qson_skip_white_spaces(ctx) == QSON_RESULT_OK;
	success &= qson_deserialize_ctx_index(ctx) == 20;
	test_result_log(success);
	return success;
}

extern bool test_qson_deserialize_array();
extern bool test_qson_deserialize_object();

bool test_qson_deserialize() {
	bool success = 1;
	success &= test_qson_skip_white_spaces();
	success &= test_qson_skip_white_spacesـunexpected_eof();
	success &= test_qson_ctx_size_check();
	success &= test_qson_skip_white_spacesـline_comment();
	success &= test_qson_skip_white_spacesـarea_comment();
	success &= test_qson_deserialize_array();
	success &= test_qson_deserialize_object();
	return success;
}

