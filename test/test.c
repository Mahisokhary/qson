#include "test.h"

extern bool test_qson();

int main() {
	bool success = 1;
	success &= test_qson();
	return !success;
}

