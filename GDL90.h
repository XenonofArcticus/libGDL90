#ifndef GDL90_H
#define GDL90_H 1

struct _gdl90_t;

typedef struct _gdl90_t* gdl90_t;
typedef uint32_t gdl90_bool_t;
typedef uint32_t gdl90_type_t;
typedef uint32_t gdl90_int_t;

#define GDL90_HEARTBEART 0x0001
#define GDL90_OWNSHIP 0x0002 /* Ownship Report */
#define GDL90_GEOMTRIC 0x0004 /* Ownship Geometric Altitude */
#define GDL90_TRAFFIC 0x0008 /* Traffic Report */

#define GDL90_TRUE 1
#define GDL90_FALSE 0

gdl90_t gdl90_create(const uint8_t* buffer, size_t size);
void gdl90_destroy(gdl90_t gdl);

gdl90_type_t gdl90_type(gdl90_t gdl);

gdl90_int_t gdl90_altitude(gdl90_t gdl);
gdl90_float_t gdl90_lattitude(gdl90_t gdl);
gdl90_float_t gdl90_longitude(gdl90_t gdl);

typedef struct _gdl90_traffic_t {
	gdl_int_t altitude;
	gdl_float_t lattitude;
	gdl_float_t longitude;
} gdl90_traffic_t;

gdl90_bool_t gdl90_traffic(gdl90_t gdl, gdl90_traffic_t* traffic);

/*
 * gdl90_t gdl = gdl90_create(buff, 80);
 *
 * if(gdl90_type(gdl) == GDL90_TRAFFIC) {
 * 	gdl90_int_t altitude = gld90_altitude(gdl);
 *
 * 	...
 * }
 *
 * gdl90_destroy(gdl);
 */

#endif
