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

#define GDL90_TRUE 1
#define GDL90_FALSE 0

gdl90_t gdl90_create(const gdl90_byte_t* buffer, gdl90_size_t size);
void gdl90_destroy(gdl90_t gdl);

gdl90_id_t gdl90_id(const gdl90_t gdl);

gdl90_float_t gdl90_lattitude(const gdl90_t gdl);
gdl90_float_t gdl90_longitude(const gdl90_t gdl);
gdl90_int_t gdl90_altitude(const gdl90_t gdl);

gdl90_int_t gdl90_ahrs_yaw(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_pitch(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_roll(const gdl90_t gdl);
gdl90_int_t gdl90_ahrs_heading(const gdl90_t gdl);

#endif
