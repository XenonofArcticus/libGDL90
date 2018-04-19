#include <stdio.h>

#include "GDL90.h"

typedef struct _test_ownship_t {
	gdl90_size_t size;
	const gdl90_byte_t* data;
	gdl90_int_t latitude;
	gdl90_int_t longitude;
	gdl90_int_t altitude;
} test_ownship_t;

static test_ownship_t TEST_OWNSHIP[2] = {
	{
		32,
		(gdl90_byte_t*)(
			"\x7E\x0A" /* FLAGBYTE and ID */
				"\x00\x00\x00\x00"
					"\x15\xA7\xE5" /* Latitude */
					"\xBA\x47\x99" /* Longitude */
					"\x08\xC9" /* Altitude */
				"\x88\xFF\xE0\x00\x80\x01\x4E\x31\x32\x33\x34\x35\x20\x20\x00"
			"\x7B\xE5\x7E" /* CRC and FLAGBYTE */
		),
		30,
		-98,
		2500
	},
	{
		33,
		(gdl90_byte_t*)(
			"\x7E\x0A"
				"\x00\x00\x00\x00"
					"\x18\x7D\x5D\xF5"
					"\xBD\x1F\xB4"
					"\x09\x49"
				"\x88\x27\x40\x00\x82\x01\x4E\x31\x32\x33\x34\x35\x20\x20\x00"
			"\x8C\xEB\x7E"
		),
		34,
		-94,
		2702
	}
};

static void test_ownship(const test_ownship_t* to) {
	gdl90_t gdl = gdl90_create(to->data, to->size, GDL90_ALL);

	if(gdl90_id(gdl) == GDL90_OWNSHIP) {
		printf("gdl90_latitude = %2.2f (%d)\n", gdl90_latitude(gdl), to->latitude);
		printf("gdl90_longitude = %2.2f (%d)\n", gdl90_longitude(gdl), to->longitude);
		printf("gdl90_altitude = %d (%d)\n", gdl90_altitude(gdl), to->altitude);
	}

	gdl90_destroy(gdl);
}

int main(int argc, char** argv) {
	test_ownship(&TEST_OWNSHIP[0]);
	test_ownship(&TEST_OWNSHIP[1]);

	return 0;
}
