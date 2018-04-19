#include <stdio.h>

#include "GDL90.h"

gdl90_byte_t* test_scan_file_data(const gdl90_str_t* path, gdl90_size_t* size) {
	gdl90_byte_t* data = NULL;
	FILE* scan_file = NULL;

	if((scan_file = fopen(path, "rb"))) {
		fseek(scan_file, 0, SEEK_END);

		*size = ftell(scan_file);

		fseek(scan_file, 0, SEEK_SET);

		data = (gdl90_byte_t*)(malloc(*size));

		fread(data, *size, 1, scan_file);
		fclose(scan_file);
	}

	return data;
}

int main(int argc, char** argv) {
	gdl90_t gdl = NULL;
	gdl90_size_t offset = 0;
	gdl90_size_t i = 0;
	gdl90_size_t size = GDL90_SIZE_INVALID;
	gdl90_byte_t* data = NULL;

	if(argc <= 1) {
		printf("Pass the path to a file to be scanned!\n");

		return 1;
	}

	if(!(data = test_scan_file_data(argv[1], &size))) {
		printf("Couldn't read data from file: %s\n", argv[1]);

		return 2;
	}

	while((gdl = gdl90_create_buffer(data, size, &offset, GDL90_ALL))) {
		printf("%04lu: %s\n", i, gdl90_id_str(gdl90_id(gdl)));

		if(gdl90_id(gdl) == GDL90_STRATUX_AHRS) {
			printf(" >> yaw:   %d\n", gdl90_ahrs_yaw(gdl));
			printf(" >> pitch: %d\n", gdl90_ahrs_pitch(gdl));
			printf(" >> roll:  %d\n", gdl90_ahrs_roll(gdl));
		}

		gdl90_destroy(gdl);

		i++;
	}

	free(data);

	return 0;
}
