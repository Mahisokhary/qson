#include <test/test.h>

extern bool test_qson_deserialize();
extern bool test_qson_serialize();

bool test_qson() {
	bool success = 1;
	success &= test_qson_deserialize();
	success &= test_qson_serialize();
	return success;
}
