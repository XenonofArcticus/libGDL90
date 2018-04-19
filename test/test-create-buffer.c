#include <stdio.h>

#include "GDL90.h"

/* This is a chunk of data containing four valid GDL90_HEARTBEAT messages, some NULL/bogus bytes,
 * and a final GDL90_FLAGBYTE with NO ending FLAGBYTE to try and confuse things. */
static const gdl90_byte_t* TEST_CREATE_BUFFER = (const gdl90_byte_t*)(
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /*   0 -  10 */
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E" /*  11 -  21 (VALID) */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /*  22 -  32 */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /*  33 -  43 */
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E" /*  44 -  54 (VALID) */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /*  55 -  65 */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /*  66 -  76 */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /*  77 -  87 */
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E" /*  88 -  98 (VALID) */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /*  99 - 109 */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /* 110 - 120 */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /* 121 - 131 */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /* 132 - 142 */
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E" /* 143 - 154 (VALID) */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /* 155 - 165 */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /* 166 - 176 */
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00" /* 177 - 187 */
	"\x7E\x99\x00\x00\x00\x00\x00\x00\x00\x00\x00" /* 189 - 199 (INVALID, no ending FLAGBYTE) */
	"\x7E\x00\x81\x41\xDB\xD0\x08\x02\xB3\x8B\x7E" /* 200 - 210 (VALID) */
);

#define TEST_CREATE_BUFFER_SIZE (19 * 11)

static void test_create_buffer_flagbyte(gdl90_size_t pos, gdl90_id_t ids) {
	printf("GDL90_FLAGBYTE starting from pos %lu @ %lu\n", pos, gdl90_flagbyte(
		TEST_CREATE_BUFFER,
		TEST_CREATE_BUFFER_SIZE,
		pos,
		ids
	));
}

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
		printf("Message %lu was gdl90_id(%u); next offset: %lu\n", i, gdl90_id(gdl), offset);

		gdl90_destroy(gdl);

		i++;
	}

	test_create_buffer_flagbyte(11, GDL90_HEARTBEAT);
	test_create_buffer_flagbyte(12, GDL90_ALL);
	test_create_buffer_flagbyte(0, GDL90_STRATUX_AHRS);
	test_create_buffer_flagbyte(189, GDL90_FALSE);

	return 0;
}
