#ifndef GDL90_H
#define GDL90_H 1

#include <stdint.h>
#include <stdlib.h>

#ifdef  __cplusplus
extern "C" {
#endif

struct _gdl90_t;

typedef struct _gdl90_t* gdl90_t;
typedef uint16_t gdl90_crc_t;
typedef uint32_t gdl90_bool_t;
typedef uint32_t gdl90_id_t;
typedef size_t gdl90_size_t;
typedef uint8_t gdl90_byte_t;
typedef const gdl90_byte_t* gdl90_buffer_t;
typedef int32_t gdl90_int_t;
typedef float gdl90_float_t;
typedef char gdl90_str_t;

/* These are values (flags) used through the library for identifying message types and for
 * contructing bitflags for use in the various create routines. */
#define GDL90_HEARTBEAT 0x0001
#define GDL90_OWNSHIP 0x0002
#define GDL90_GEOMTRIC 0x0004
#define GDL90_TRAFFIC 0x0008
#define GDL90_STRATUX_AHRS 0x0010
#define GDL90_ALL 0xFFFF

/* The value used internally by the Stratux AHRS messages when some field is invalid. */
#define GDL90_STRATUX_AHRS_INVALID 0x7FFF

#define GDL90_TRUE 1
#define GDL90_FALSE 0
#define GDL90_SIZE_INVALID SIZE_MAX /* 18446744073709551615 */

typedef enum _gdl90_create_t {
	GDL90_CREATE_MALLOC,
	GDL90_CREATE_STATIC
} gdl90_create_t;

void gdl90_setup(gdl90_create_t create);
void gdl90_shutdown();

typedef enum _gdl90_err_t {
	GDL90_ERR_NONE = 0,
	GDL90_ERR_CRC,
	GDL90_ERR_FLAGBYTE_START,
	GDL90_ERR_FLAGBYTE_END,
	GDL90_ERR_ID_INVALID
} gdl90_err_t;

/* Returns the result of a 16bit CRC check against the passed-in @buffer. */
gdl90_crc_t gdl90_crc(gdl90_buffer_t buffer, gdl90_size_t size);

/* Accepts a perfectly-packed buffer of GDL90 data and returns a valid gdl90_t context as long as
 * the GDL90_* value is included in the specified @ids variable. If the message parsing process
 * fails in any way, NULL is returned. */
gdl90_t gdl90_create(gdl90_buffer_t buffer, gdl90_size_t size, gdl90_id_t ids);

/* An iterative version of gdl90_create designed to be called in a loop to parse arbitrary buffers
 * of potential GDL90 data. The @offset variable must be initialized to 0 on the first call, as its
 * contents are used to keep track of the position within the buffer. */
gdl90_t gdl90_create_buffer(
	gdl90_buffer_t buffer,
	gdl90_size_t size,
	gdl90_size_t* offset,
	gdl90_id_t ids
);

/* Frees the memory used by the opaque gdl90_t. */
void gdl90_destroy(gdl90_t gdl);

/* Returns one of the GDL90_* flags above. If some kind of internal error has occurred the gdl90_t
 * context will usually set the id to GDL90_FALSE. */
gdl90_id_t gdl90_id(const gdl90_t gdl);

/* Returns the string name of the GDL90_* id/constant. */
const gdl90_str_t* gdl90_id_str(gdl90_id_t id);

/* Returns the size of the message payload for the corresponding GDL90_* id, minus the two
 * GDL90_FLAGBYTE characters that wrap the buffer. */
gdl90_size_t gdl90_id_size(gdl90_id_t id);

gdl90_id_t gdl90_id_flag(gdl90_byte_t b);
gdl90_id_t gdl90_flag_id(gdl90_byte_t b);

gdl90_err_t gdl90_error(const gdl90_t gdl);
gdl90_bool_t gdl90_valid(const gdl90_t gdl);

/* Returns the numeric index of the first GDL90_FLAGBYTE detected in the specified buffer, starting
 * at the indicated @offset. If the @ids argument includes any of the GDL90_* message bitflags, a
 * valid index will be returned if the subsequent byte AFTER the detected FLAGBYTE matches. If
 * GDL90_FALSE is passed as the @ids argument, this function returns immediately after finding the
 * first FLAGBYTE. Returns GDL90_SIZE_INVALID on failure. */
gdl90_size_t gdl90_flagbyte(
	gdl90_buffer_t buffer,
	gdl90_size_t size,
	gdl90_size_t offset,
	gdl90_id_t ids
);

gdl90_float_t gdl90_latitude(const gdl90_t gdl);
gdl90_float_t gdl90_longitude(const gdl90_t gdl);
gdl90_int_t gdl90_altitude(const gdl90_t gdl);

gdl90_int_t gdl90_ahrs_yaw(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_pitch(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_roll(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_heading(const gdl90_t gdl);

#ifdef  __cplusplus
}
#endif

#endif
