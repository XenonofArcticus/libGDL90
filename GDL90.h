#ifndef GDL90_H
#define GDL90_H 1

typedef void* gdl90_t;

gdl90_t gdl90_create(const uint8_t* buffer, size_t size);
void gdl90_destroy(gdl90_t gdl);

typedef struct _gdl90_heartbeat_t {
	uint8_t id;
	uint8_t status1;
	uint8_t status2;
	uint16_t timestamp;
	uint16_t counts;
} gdl90_heartbeat_t;

typedef struct _gdl90_traffic_t {
} gdl90_traffic_t;

#define GDL90_HEARTBEAT(gdl) ((gdl90_heartbeat_t*)(gdl))

#endif
