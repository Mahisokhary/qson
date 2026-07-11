#include <test/qson/serialize/serialize.internal.h>

extern bool test_qson_serialize_object();
extern bool test_qson_serialize_array();

bool test_qson_serialize() {
	bool success = 1;
	success &= test_qson_serialize_object();
	success &= test_qson_serialize_array();
	return success;
}

