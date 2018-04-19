#include <stdio.h>

#include "GDL90.h"

/* This is a chunk of data containing four valid GDL90_HEARTBEAT messages, some NULL/bogus bytes,
 * and a final GDL90_FLAGBYTE with NO ending FLAGBYTE to try and confuse things. */
static const gdl90_byte_t* TEST_CREATE_BUFFER = (const gdl90_byte_t*)(
	"\x00\x00\x00\x00"
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E"
	"\x00\x00\x00\x00"
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E"
	"\x00\x00\x00\x00"
	"\x00\x00\x00\x00"
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E"
	"\x00\x00\x00\x00"
	"\x00\x00\x00\x00"
	"\x00\x00\x00\x00"
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E"
	"\x00\x00\x00\x00"
	"\x7E\x00\x00\x00\x00\x00\x00\x00\x00\x00"
);

#define TEST_CREATE_BUFFER_SIZE 86

/* static void test_create_buffer_flagbyte(gdl90_size_t pos) {
	printf("GDL90_FLAGBYTE starting from pos %lu @ %lu\n", pos, gdl90_flagbyte(
		TEST_CREATE_BUFFER,
		TEST_CREATE_BUFFER_SIZE,
		pos
	));
} */

int main(int argc, char** argv) {
	gdl90_t gdl = NULL;
	gdl90_size_t offset = 0;
	gdl90_size_t i = 0;

	while((gdl = gdl90_create_buffer(
		TEST_CREATE_BUFFER,
		TEST_CREATE_BUFFER_SIZE,
		&offset,
		GDL90_ALL
	))) {
		printf("Message %lu was gdl90_id(%u) @ offset %lu\n", i, gdl90_id(gdl), offset);

		gdl90_destroy(gdl);

		i++;
	}

	/* test_create_buffer_flagbyte(0);
	test_create_buffer_flagbyte(1);
	test_create_buffer_flagbyte(11);
	test_create_buffer_flagbyte(50); */

	return 0;
}
