#ifndef GDL90_H
#define GDL90_H 1

#include <stdint.h>
#include <stdlib.h>

struct _gdl90_t;

typedef struct _gdl90_t* gdl90_t;
typedef uint32_t gdl90_bool_t;
typedef uint32_t gdl90_id_t;
typedef size_t gdl90_size_t;
typedef uint8_t gdl90_byte_t;
typedef int32_t gdl90_int_t;
typedef float gdl90_float_t;

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

/* Accepts a perfectly-packed buffer of GDL90 data and returns the appropriate data type. If the
 * message parsing process fails in any way, NULL is returned. */
gdl90_t gdl90_create(const gdl90_byte_t* buffer, gdl90_size_t size, gdl90_id_t ids);

/* An iterative version of gdl90_create designed to by called in a loop to parse arbitrary buffers
 * of potential GDL90 data. The @offset variable must be initialized to 0 on the first call, as its
 * contents are used to keep track of the position within the buffer. */
gdl90_t gdl90_create_buffer(
	const gdl90_byte_t* buffer,
	gdl90_size_t size,
	gdl90_size_t* offset,
	gdl90_id_t ids
);

/* Frees the memory used by the opaque gdl90_t. */
void gdl90_destroy(gdl90_t gdl);

/* Returns one of the GDL90_* defines above (lines 17-22). If some kind of internal error has
 * occurred, zero is returned. */
gdl90_id_t gdl90_id(const gdl90_t gdl);

/* Returns the numeric index of the first GDL90_FLAGBYTE detected in the specified buffer, starting
 * at the indicated @offset. Returns GDL90_SIZE_INVALID if none are detected. */
gdl90_size_t gdl90_flagbyte(const gdl90_byte_t* buffer, gdl90_size_t size, gdl90_size_t offset);

gdl90_float_t gdl90_latitude(const gdl90_t gdl);
gdl90_float_t gdl90_longitude(const gdl90_t gdl);
gdl90_int_t gdl90_altitude(const gdl90_t gdl);

gdl90_int_t gdl90_ahrs_yaw(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_pitch(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_roll(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_heading(const gdl90_t gdl);

#endif
