#include <stdio.h>

#include "GDL90.h"

/* CRC value should be 0x8BB3. */
static gdl90_byte_t TEST_HEARTBEAT[] = {
	0x7E,
	0x00,
		0x81,
		0x41,
		0xDB, 0xD0,
		0x08, 0x02,
	0xB3, 0x8B,
	0x7E
};

int main(int argc, char** argv) {
	gdl90_t gdl = gdl90_create(TEST_HEARTBEAT, sizeof(TEST_HEARTBEAT));

	printf("gdl90_id = %d\n", gdl90_id(gdl));

	gdl90_destroy(gdl);

	return 0;
}
