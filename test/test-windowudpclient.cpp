// StratuxClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>

#include "GDL90\GDL90.h"

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET socketC;

	struct sockaddr_in serverInfo;
	int len = sizeof(serverInfo);
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons((u_short)4000);
	serverInfo.sin_addr.s_addr = INADDR_ANY;

	socketC = socket(AF_INET, SOCK_DGRAM, 0);
	if (bind(socketC, (LPSOCKADDR)&serverInfo, len)<0) {
		printf("Bind failed\n");
		exit(1);
	}

	unsigned char buffer[1024];

	while (1)
	{
		ZeroMemory(buffer, sizeof(buffer));

		int receivecount = recvfrom(socketC, (char*)buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, &len);

		if (receivecount != SOCKET_ERROR)
		{
			printf("Received %d bytes from server\n", receivecount);

			gdl90_t gpsdata = NULL;
			gdl90_size_t bufferoffset = 0;
			while (gpsdata = gdl90_create_buffer((unsigned char*)&buffer,
				receivecount,
				&bufferoffset,
				GDL90_OWNSHIP | GDL90_STRATUX_AHRS))
			{
				//if(gdl90_id(gpsdata) == GDL90_STRATUX_AHRS)
				{
					gdl90_float_t lati = gdl90_latitude(gpsdata);
					gdl90_float_t longi = gdl90_longitude(gpsdata);
					gdl90_int_t alti = gdl90_altitude(gpsdata);

					gdl90_int_t yaw = gdl90_ahrs_yaw(gpsdata);
					gdl90_int_t pitch = gdl90_ahrs_pitch(gpsdata);
					gdl90_int_t roll = gdl90_ahrs_roll(gpsdata);
					gdl90_int_t heading = gdl90_ahrs_heading(gpsdata);

					printf("\nGPS Data %f, %f, %d\n\n", lati, longi, alti);

				}
				gdl90_destroy(gpsdata);
			}

		}
	}
	closesocket(socketC);

	return 0;
}

