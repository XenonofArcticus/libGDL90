#include <stdio.h>

#include "GDL90.h"

gdl90_byte_t* test_pcap_file(const gdl90_str_t* path, gdl90_size_t* size) {
	gdl90_byte_t* data = NULL;
	FILE* pcap = NULL;

	if((pcap = fopen(path, "rb"))) {
		fseek(pcap, 0, SEEK_END);

		*size = ftell(pcap);

		fseek(pcap, 0, SEEK_SET);

		data = (gdl90_byte_t*)(malloc(*size));

		fread(data, *size, 1, pcap);
		fclose(pcap);
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
		printf("Pass the path to a .pcap file!\n");

		return 1;
	}

	if(!(data = test_pcap_file(argv[1], &size))) {
		printf("Couldn't read data from file: %s\n", argv[1]);

		return 2;
	}

	while((gdl = gdl90_create_buffer(data, size, &offset, GDL90_STRATUX_AHRS))) {
		printf(
			"Message %zu was gdl90_id(%s); next offset: %zu\n",
			i,
			gdl90_id_str(gdl90_id(gdl)),
			offset
		);

		gdl90_destroy(gdl);

		i++;
	}

	return 0;
}
