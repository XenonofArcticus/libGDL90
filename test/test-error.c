#include <stdio.h>

#include "GDL90.h"

typedef struct _test_error_t {
	gdl90_size_t size;
	gdl90_byte_t* data;
	gdl90_err_t err;
} test_error_t;

static test_error_t TEST_ERROR[] = {
	/* GDL90_HEARTBEAT with no ending GDL90_FLAGBYTE value. */
	{
		11,
		(gdl90_byte_t*)("\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x00"),
		GDL90_ERR_FLAGBYTE_END
	},

	/* GDL90_HEARTBEAT with an improper CRC. */
	{
		11,
		(gdl90_byte_t*)("\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x00\x7E"),
		GDL90_ERR_CRC
	},

	/* GDL90_STRATUX_AHRS with an invalid ID (the 2nd byte). */
	{
		28,
		(gdl90_byte_t*)(
			"\x7E\x4D\x45\x01\x01\xFF\xE8\x00\x5E\x00\x9E\xFF\xEB\x7F"
			"\xFF\x00\x0A\x7F\xFF\x14\x24\x00\x02\x7F\xFF\xBE\x8D\x7E"
		),
		GDL90_ERR_ID_INVALID
	},

	/* A NULL buffer. */
	{
		0,
		(gdl90_byte_t*)(""),
		GDL90_ERR_FLAGBYTE_START
	},

	/* A GDL90_OWNSHIP message that is 3 bytes too SMALL. */
	{
		30,
		(gdl90_byte_t*)(
			"\x7E\x0A\x00\x00\x00\x00\x18\x7D\x5D\xF5\xBD\x1F\xB4\x09\x49"
			"\x88\x27\x40\x00\x82\x32\x33\x34\x35\x20\x20\x00\x8C\xEB\x7E"
		),
		GDL90_ERR_CRC
	},

	/* GDL90_HEARTBEAT that is 4 byte too LARGE. */
	{
		15,
		(gdl90_byte_t*)("\x7E\x00\x81\x41\xDB\x00\x00\x00\x00\xD0\x08\x02\xB3\x00\x7E"),
		GDL90_ERR_CRC
	}
};

#define TEST_ERROR_SIZE 6

static void test_error(gdl90_size_t index) {
	const test_error_t* te = &TEST_ERROR[index];

	gdl90_t gdl = gdl90_create(te->data, te->size, GDL90_ALL);

	printf("test_error %d\n", index);
	printf(" >> gdl90_error = %d\n", gdl90_error(gdl));
	printf(" >> gdl90_error_str = %s\n", gdl90_error_str(gdl90_error(gdl)));
	printf(" >> Error should be: %s\n", gdl90_error_str(te->err));

	gdl90_destroy(gdl);
}

int main(int argc, char** argv) {
	gdl90_size_t i;

	for(i = 0; i < TEST_ERROR_SIZE; i++) test_error(i);

	return 0;
}
