#include <stdio.h>

#include "GDL90.h"

typedef struct _test_ahrs_t {
	gdl90_size_t size;
	const gdl90_byte_t* data;
} test_ahrs_t;

static test_ahrs_t TEST_AHRS[3] = {
	{ 28, (gdl90_byte_t*)("\x7E\x4C\x45\x01\x01\xFF\xE8\x00\x5E\x00\x9E\xFF\xEB\x7F\xFF\x00\x0A\x7F\xFF\x14\x24\x00\x02\x7F\xFF\xBE\x8D\x7E") },
	{ 28, (gdl90_byte_t*)("\x7E\x4C\x45\x01\x01\xFF\xE7\x00\x5E\x01\x4B\xFF\xEB\x7F\xFF\x00\x0A\x7F\xFF\x14\x24\xFF\xFD\x7F\xFF\x64\xA7\x7E") },
	{ 28, (gdl90_byte_t*)("\x7E\x4C\x45\x01\x01\xFF\xE9\x00\x5F\x01\x5B\xFF\xEC\x7F\xFF\x00\x0A\x7F\xFF\x14\x25\xFF\xFD\x7F\xFF\xFD\xE4\x7E") }
};

static void test_ahrs(const test_ahrs_t* ta) {
	gdl90_t gdl = gdl90_create(ta->data, ta->size, GDL90_ALL);

	if(gdl90_id(gdl) == GDL90_STRATUX_AHRS) {
		printf("gdl90_ahrs_yaw = %d\n", gdl90_ahrs_yaw(gdl));
		printf("gdl90_ahrs_pitch = %d\n", gdl90_ahrs_pitch(gdl));
		printf("gdl90_ahrs_roll = %d\n", gdl90_ahrs_roll(gdl));
	}

	gdl90_destroy(gdl);
}

int main(int argc, char** argv) {
	test_ahrs(&TEST_AHRS[0]);
	test_ahrs(&TEST_AHRS[1]);
	test_ahrs(&TEST_AHRS[2]);

	return 0;
}
