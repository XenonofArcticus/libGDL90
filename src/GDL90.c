#include "GDL90.h"

#include <string.h>
#include <float.h>

static gdl90_crc_t GDL90_CRC[] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
	0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
	0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
	0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
	0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
	0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
	0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
	0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
	0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
	0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
	0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
	0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
	0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
	0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
	0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
	0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
	0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
	0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
	0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
	0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
	0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
	0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
	0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

#if 0
/* This code generates the table above. To prevent any kind of init() function being
 * necessary, we simply statically include the encoded table above. Perhaps with further
 * GDL90 library being added in the future this won't be necessary. */
static gdl90_crc_t GDL90_CRC[256];
static void gdl90_crc_init() {
	gdl90_crc_t i;
	gdl90_crc_t bitctr;
	gdl90_crc_t crc;

	for(i = 0; i < 256; i++) {
		crc = (i << 8);

		for(bitctr = 0; bitctr < 8; bitctr++) {
		    crc = (crc << 1) ^ ((crc & 0x8000) ? 0x1021 : 0);
		}

		GDL90_CRC[i] = crc;
	}
}
#endif

gdl90_crc_t gdl90_crc(gdl90_buffer_t buffer, gdl90_size_t size) {
	gdl90_size_t i;
	gdl90_crc_t crc = 0;

	for(i = 0; i < size; i++) crc = GDL90_CRC[crc >> 8] ^ (crc << 8) ^ buffer[i];

	return crc;
}

#define GDL90_FLAGBYTE 0x7E
#define GDL90_ESCAPEBYTE 0x7D
#define GDL90_ID_HEARTBEAT 0x00
#define GDL90_ID_HEARTBEAT_SIZE 0x09
#define GDL90_ID_UPLINK_DATA 0x07
#define GDL90_ID_UPLINK_DATA_SIZE 0x01B6 /* 438 bytes */
#define GDL90_ID_OWNSHIP 0x0A
#define GDL90_ID_OWNSHIP_SIZE 0x1E /* 30 bytes */
#define GDL90_ID_TRAFFIC 0x14
#define GDL90_ID_TRAFFIC_SIZE 0x1E /* 30 bytes */
#define GDL90_ID_STRATUX_HEARTBEAT0 0xCC
#define GDL90_ID_STRATUX_HEARTBEAT1 0x5358
#define GDL90_ID_STRATUX_AHRS 0x4C
#define GDL90_ID_STRATUX_AHRS_SIZE 0x1A /* 26 bytes */

struct _gdl90_t {
	gdl90_int_t id;
	gdl90_byte_t* data;
	gdl90_err_t err;
};

typedef struct _gdl90_static_t {
	struct _gdl90_t gdl;
	gdl90_bool_t locked;
} gdl90_static_t;

#define GDL90_STATIC_CREATE(id) \
	static gdl90_static_t GDL90_STATIC_##id = { \
		{ GDL90_##id, NULL, GDL90_ERR_NONE }, \
		GDL90_FALSE \
	};

#define GDL90_STATIC_CREATE_DATA(id) \
	if(!GDL90_STATIC_##id.gdl.data) { \
		GDL90_STATIC_##id.gdl.data = (gdl90_byte_t*)(malloc(GDL90_ID_##id##_SIZE + 2)); \
	}

GDL90_STATIC_CREATE(FALSE)
GDL90_STATIC_CREATE(HEARTBEAT)
GDL90_STATIC_CREATE(OWNSHIP)
GDL90_STATIC_CREATE(TRAFFIC)
GDL90_STATIC_CREATE(STRATUX_AHRS)

static void gdl90_init(gdl90_t gdl) {
	gdl->id = GDL90_FALSE;
	gdl->data = NULL;
	gdl->err = GDL90_ERR_NONE;
}

static gdl90_create_t GDL90_CREATE_MODE = GDL90_CREATE_MALLOC;

void gdl90_setup(gdl90_create_t create) {
	GDL90_CREATE_MODE = create;

	GDL90_STATIC_CREATE_DATA(HEARTBEAT)
	GDL90_STATIC_CREATE_DATA(OWNSHIP)
	GDL90_STATIC_CREATE_DATA(TRAFFIC)
	GDL90_STATIC_CREATE_DATA(STRATUX_AHRS)
}

void gdl90_shutdown() {
}

gdl90_id_t gdl90_id_flag(gdl90_byte_t b) {
	if(b == GDL90_ID_HEARTBEAT) return GDL90_HEARTBEAT;

	else if(b == GDL90_ID_OWNSHIP) return GDL90_OWNSHIP;

	else if(b == GDL90_ID_TRAFFIC) return GDL90_TRAFFIC;

	else if(b == GDL90_ID_STRATUX_AHRS) return GDL90_STRATUX_AHRS;

	else return GDL90_FALSE;
}

gdl90_id_t gdl90_flag_id(gdl90_byte_t b) {
	if(b == GDL90_HEARTBEAT) return GDL90_ID_HEARTBEAT;

	else if(b == GDL90_OWNSHIP) return GDL90_ID_OWNSHIP;

	else if(b == GDL90_TRAFFIC) return GDL90_ID_TRAFFIC;

	else if(b == GDL90_STRATUX_AHRS) return GDL90_ID_STRATUX_AHRS;

	else return GDL90_FALSE;
}

#define gdl90_err_return(gdl, error) { \
	gdl->id = GDL90_FALSE; \
	gdl->err = GDL90_ERR_##error; \
	return gdl; \
}

gdl90_t gdl90_create(gdl90_buffer_t buffer, gdl90_size_t size, gdl90_id_t ids) {
	gdl90_t gdl = (gdl90_t)(&GDL90_STATIC_FALSE);
	gdl90_crc_t crc = 0;
	gdl90_size_t i = 0;
	gdl90_size_t c = 0;
	gdl90_size_t id_size = 0;

	if(GDL90_CREATE_MODE == GDL90_CREATE_MALLOC) gdl = (gdl90_t)(malloc(
		sizeof(struct _gdl90_t)
	));

	gdl90_init(gdl);

	/* Make sure that the buffer begins with a valid GDL90_FLAGBYTE, followed by one or more of the
	 * GDL90_* flag values specified in the @ids argument. */
	if(gdl90_flagbyte(buffer, size, 0, ids)) gdl90_err_return(gdl, FLAGBYTE_START);

	/* Check to ensure the buffer ends with a GDL90_FLAGBYTE. */
	if(buffer[size - 1] != GDL90_FLAGBYTE) gdl90_err_return(gdl, FLAGBYTE_END);

	if(!(gdl->id = gdl90_id_flag(buffer[1]))) gdl90_err_return(gdl, ID_INVALID);

	id_size = gdl90_id_size(gdl->id);

	if(GDL90_CREATE_MODE == GDL90_CREATE_MALLOC) gdl->data = (gdl90_byte_t*)(malloc(id_size));

	/* If a FLAGBYTE or ESCAPEBYTE is included in the payload, this process will extract them
	 * properly using the technique described in the spec on page 5. */
	while(i < size) {
		if(c >= id_size) break;

		if(buffer[i + 1] == GDL90_ESCAPEBYTE) {
			gdl->data[c] = buffer[i + 2] ^ 0x20;

			i++;
		}

		else gdl->data[c] = buffer[i + 1];

		i++;
		c++;
	}

	/* The precalculated CRC is the last 2 bytes at the end of the payload. */
	crc = *((gdl90_crc_t*)(&gdl->data[id_size - 2]));

	/* Now calculate our CRC value using everything UP TO the 2 bytes. */
	if(crc != gdl90_crc(gdl->data, id_size - 2)) gdl90_err_return(gdl, CRC);

	return gdl;
}

gdl90_t gdl90_create_buffer(
	gdl90_buffer_t buffer,
	gdl90_size_t size,
	gdl90_size_t* offset,
	gdl90_id_t ids
) {
	gdl90_size_t start;

	if((start = gdl90_flagbyte(buffer, size, *offset, ids)) != GDL90_SIZE_INVALID) {
		gdl90_size_t end;
		
		*offset = start + 1;

		if((end = gdl90_flagbyte(buffer, size, start + 1, GDL90_FALSE)) != GDL90_SIZE_INVALID) {
			gdl90_t gdl = gdl90_create(&buffer[start], (end - start) + 1, ids);

			if(gdl90_valid(gdl)) *offset += end - start;

			return gdl;
		}
	}

	return NULL;
}

void gdl90_destroy(gdl90_t gdl) {
	if(gdl->data) free(gdl->data);

	free(gdl);
}

gdl90_size_t gdl90_flagbyte(
	gdl90_buffer_t buffer,
	gdl90_size_t size,
	gdl90_size_t offset,
	gdl90_id_t ids
) {
	gdl90_size_t i = 0;

	while(GDL90_TRUE) {
		if(offset + i >= size) break;

		if(buffer[offset + i] == GDL90_FLAGBYTE) {
			gdl90_byte_t b;

			if(ids == GDL90_FALSE) return offset + i;

			if(offset + i + 1 >= size) break;

			b = buffer[offset + i + 1];

			if(
				(b == GDL90_ID_HEARTBEAT && (ids & GDL90_HEARTBEAT)) ||
				(b == GDL90_ID_OWNSHIP && (ids & GDL90_OWNSHIP)) ||
				(b == GDL90_ID_TRAFFIC && (ids & GDL90_TRAFFIC)) ||
				(b == GDL90_ID_STRATUX_AHRS && (ids & GDL90_STRATUX_AHRS))
			) {
				return offset + i;
			}
		}

		i++;
	}

	return GDL90_SIZE_INVALID;
}

gdl90_id_t gdl90_id(const gdl90_t gdl) {
	return gdl->id;
}

const gdl90_str_t* gdl90_id_str(gdl90_id_t id) {
	if(id == GDL90_HEARTBEAT) return "GDL90_HEARTBEAT";

	else if(id == GDL90_OWNSHIP) return "GDL90_OWNSHIP";

	else if(id == GDL90_TRAFFIC) return "GDL90_TRAFFIC";

	else if(id == GDL90_STRATUX_AHRS) return "GDL90_STRATUX_AHRS";

	else return "GDL90_FALSE";
}

gdl90_size_t gdl90_id_size(gdl90_id_t id) {
	if(id == GDL90_HEARTBEAT) return GDL90_ID_HEARTBEAT_SIZE;

	else if(id == GDL90_OWNSHIP) return GDL90_ID_OWNSHIP_SIZE;

	else if(id == GDL90_TRAFFIC) return GDL90_ID_TRAFFIC_SIZE;

	else if(id == GDL90_STRATUX_AHRS) return GDL90_ID_STRATUX_AHRS_SIZE;

	else return GDL90_SIZE_INVALID;
}

gdl90_err_t gdl90_error(const gdl90_t gdl) {
	return gdl->err;
}

const gdl90_str_t* gdl90_error_str(gdl90_err_t err) {
	if(err == GDL90_ERR_NONE) return "GDL90_ERR_NONE";

	else if(err == GDL90_ERR_CRC) return "GDL90_ERR_CRC";

	else if(err == GDL90_ERR_FLAGBYTE_START) return "GDL90_ERR_FLAGBYTE_START";

	else if(err == GDL90_ERR_FLAGBYTE_END) return "GDL90_ERR_FLAGBYTE_END";

	else if(err == GDL90_ERR_ID_INVALID) return "GDL90_ERR_ID_INVALID";

	else return "GDL90_ERR_INVALID";
}

gdl90_bool_t gdl90_valid(const gdl90_t gdl) {
	return gdl->err == GDL90_ERR_NONE;
}

static gdl90_int_t gdl90_uint24(gdl90_buffer_t buffer) {
	gdl90_byte_t b0 = buffer[0];
	gdl90_byte_t b1 = buffer[1];
	gdl90_byte_t b2 = buffer[2];

	return (b0 << 16) + (b1 << 8) + b2;
}

static gdl90_int_t gdl90_int24(gdl90_buffer_t buffer) {
	gdl90_int_t val = gdl90_uint24(buffer);

	if(val > 8388607) val -= 16777216;

	return val;
}

static gdl90_int_t gdl90_uint16(gdl90_buffer_t buffer) {
	gdl90_byte_t b0 = buffer[0];
	gdl90_byte_t b1 = buffer[1];

	return (b0 << 8) + b1;
}

static gdl90_int_t gdl90_int16(gdl90_buffer_t buffer) {
	gdl90_int_t val = gdl90_uint16(buffer);

	if(val > 32767) val -= 65536;

	return val;
}

#define GDL90_LAT_LONG_RES (180.0f / 8388608.0f)

gdl90_float_t gdl90_latitude(const gdl90_t gdl) {
	if(gdl->id != GDL90_OWNSHIP && gdl->id != GDL90_TRAFFIC) return FLT_MAX;

	return gdl90_int24(&gdl->data[5]) * GDL90_LAT_LONG_RES;
}

gdl90_float_t gdl90_longitude(const gdl90_t gdl) {
	if(gdl->id != GDL90_OWNSHIP && gdl->id != GDL90_TRAFFIC) return FLT_MAX;

	return gdl90_int24(&gdl->data[8]) * GDL90_LAT_LONG_RES;
}

gdl90_int_t gdl90_altitude(const gdl90_t gdl) {
	gdl90_int_t val = 0;

	if(gdl->id == GDL90_OWNSHIP || gdl->id != GDL90_TRAFFIC) {
		val += ((int32_t)(gdl->data[11])) << 4;
		val += (gdl->data[12] & 0xF0) >> 4;
		val = (val * 25) - 1000;
	}

	return val;
}

/* The value used internally by the Stratux AHRS messages when some field is invalid. */
#define GDL90_STRATUX_AHRS_INVALID 0x7FFF

static gdl90_float_t gdl90_ahrs_int16_float(const gdl90_t gdl, gdl90_size_t offset) {
	if(gdl->id != GDL90_STRATUX_AHRS) return FLT_MAX;

	else {
		gdl90_int_t val = gdl90_int16(&gdl->data[offset]);
		
		if(val == GDL90_STRATUX_AHRS_INVALID) return FLT_MAX;
		
		else return val / 10.0f;
	}
}

gdl90_float_t gdl90_ahrs_yaw(const gdl90_t gdl) {
	return gdl90_ahrs_int16_float(gdl, 12);
}

gdl90_float_t gdl90_ahrs_pitch(const gdl90_t gdl) {
	return gdl90_ahrs_int16_float(gdl, 6);
}

gdl90_float_t gdl90_ahrs_roll(const gdl90_t gdl) {
	return gdl90_ahrs_int16_float(gdl, 4);
}

gdl90_int_t gdl90_ahrs_heading(const gdl90_t gdl) {
	if(gdl->id != GDL90_STRATUX_AHRS) return 0;

	return gdl90_int16(&gdl->data[8]);
}

gdl90_buffer_t gdl90_buffer(const gdl90_t gdl) {
	return gdl->data;
}
