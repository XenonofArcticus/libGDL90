#include <stdio.h>

#include "GDL90.h"

typedef struct _test_ownship_t {
	gdl90_size_t size;
	const gdl90_byte_t* data;
	gdl90_int_t latitude;
	gdl90_int_t longitude;
	gdl90_int_t altitude;
} test_ownship_t;

#define TEST_OWNSHIP_DATA0_SIZE 32
#define TEST_OWNSHIP_DATA0 \
	"\x7E\x0A" /* FLAGBYTE and ID */ \
		"\x00\x00\x00\x00" \
			"\x15\xA7\xE5" /* Latitude */ \
			"\xBA\x47\x99" /* Longitude */ \
			"\x08\xC9" /* Altitude */ \
		"\x88\xFF\xE0\x00\x80\x01\x4E\x31\x32\x33\x34\x35\x20\x20\x00" \
	"\x7B\xE5\x7E" /* CRC and FLAGBYTE */

#define TEST_OWNSHIP_DATA1_SIZE 33
#define TEST_OWNSHIP_DATA1 \
	"\x7E\x0A" \
		"\x00\x00\x00\x00" \
			"\x18\x7D\x5D\xF5" \
			"\xBD\x1F\xB4" \
			"\x09\x49" \
		"\x88\x27\x40\x00\x82\x01\x4E\x31\x32\x33\x34\x35\x20\x20\x00" \
	"\x8C\xEB\x7E"

#define TEST_OWNSHIP_DATA2_SIZE 32
#define TEST_OWNSHIP_DATA2 \
	"\x7E\x0A\x01\xF0\x00\x00\x1C\x25\xE6\xB5\x0F\xF2\x16\x09\x8A\x00\x08\x00\x42\x01\x53\x74\x72" \
	"\x61\x74\x75\x78\x00\x00\xDB\xF6\x7E"

static test_ownship_t TEST_OWNSHIP[] = {
	{ TEST_OWNSHIP_DATA0_SIZE, (gdl90_buffer_t)(TEST_OWNSHIP_DATA0), 30, -98, 2500 },
	{ TEST_OWNSHIP_DATA1_SIZE, (gdl90_buffer_t)(TEST_OWNSHIP_DATA1), 34, -94, 2702 },
	{ TEST_OWNSHIP_DATA2_SIZE, (gdl90_buffer_t)(TEST_OWNSHIP_DATA2), 39, -105, 7800 }
};

#define TEST_OWNSHIP_BUFFER_SIZE \
	TEST_OWNSHIP_DATA0_SIZE + \
	TEST_OWNSHIP_DATA1_SIZE + \
	TEST_OWNSHIP_DATA2_SIZE + 20

gdl90_buffer_t TEST_OWNSHIP_BUFFER = (gdl90_buffer_t)(
	"\x7E\x00\x00\x00\x7E"
	TEST_OWNSHIP_DATA0
	"\x7E\x00\x00\x00\x7E"
	TEST_OWNSHIP_DATA1
	"\x7E\x00\x00\x00\x00"
	TEST_OWNSHIP_DATA2
	"\x00\x00\x00\x00\x7E"
);

static void test_ownship(const test_ownship_t* to) {
	gdl90_t gdl = gdl90_create(to->data, to->size, GDL90_ALL);

	if(gdl90_id(gdl) == GDL90_OWNSHIP) {
		printf("gdl90_latitude = %2.2f (%d)\n", gdl90_latitude(gdl), to->latitude);
		printf("gdl90_longitude = %2.2f (%d)\n", gdl90_longitude(gdl), to->longitude);
		printf("gdl90_altitude = %d (%d)\n", gdl90_altitude(gdl), to->altitude);
	}

	gdl90_destroy(gdl);
}

static void test_ownship_buffer() {
	gdl90_size_t offset = 0;
	gdl90_t      gdl    = NULL;
	
	while((gdl = gdl90_create_buffer(
		TEST_OWNSHIP_BUFFER,
		TEST_OWNSHIP_BUFFER_SIZE,
		&offset,
		GDL90_ALL
	))) {
		if(gdl90_id(gdl) == GDL90_OWNSHIP) {
			printf(" >> gdl90_latitude = %2.2f\n", gdl90_latitude(gdl));
			printf(" >> gdl90_longitude = %2.2f\n", gdl90_longitude(gdl));
			printf(" >> gdl90_altitude = %d\n", gdl90_altitude(gdl));
		}
	
		gdl90_destroy(gdl);
	}
}

int main(int argc, char** argv) {
	test_ownship(&TEST_OWNSHIP[0]);
	test_ownship(&TEST_OWNSHIP[1]);
	test_ownship(&TEST_OWNSHIP[2]);

	test_ownship_buffer();

	return 0;
}
