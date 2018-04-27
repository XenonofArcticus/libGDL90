// StratuxClient.cpp : Defines the entry point for the console application.
//

#include <winsock2.h>
#include <stdio.h>
#include <float.h>

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

	float _lati, _longi, _alti = 0.0f;
	float _pitch, _yaw, _roll = 0.0f;
	float _heading = 0.0f;

	while (1)
	{
		ZeroMemory(buffer, sizeof(buffer));

		int receivecount = recvfrom(socketC, (char*)buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, &len);

		if (receivecount != SOCKET_ERROR)
		{
			//printf("Received %d bytes from server\n", receivecount);

			gdl90_t gpsdata = NULL;
			gdl90_size_t bufferoffset = 0;
			while (gpsdata = gdl90_create_buffer((unsigned char*)&buffer,
				receivecount,
				&bufferoffset,
				GDL90_OWNSHIP | GDL90_STRATUX_AHRS))
			{
				if(gdl90_id(gpsdata) == GDL90_STRATUX_AHRS)
				{
					printf("Decoded a GDL90_STRATUX_AHRS message\n");

					gdl90_float_t lati = gdl90_latitude(gpsdata);
					if (lati != FLT_MAX) _lati = lati; else printf("   Latitude was FLT_MAX\n");

					gdl90_float_t longi = gdl90_longitude(gpsdata);
					if (longi != FLT_MAX) _longi = longi; else printf("   Longitude was FLT_MAX\n");

					gdl90_int_t alti = gdl90_altitude(gpsdata);
					if (alti != INT_MAX) _alti = alti; else printf("   Altitude was INT_MAX\n");


					gdl90_float_t yaw = gdl90_ahrs_yaw(gpsdata);
					if (yaw != FLT_MAX) _yaw = yaw; else printf("   Yaw was FLT_MAX\n");

					gdl90_float_t pitch = gdl90_ahrs_pitch(gpsdata);
					if (pitch != FLT_MAX) _pitch = pitch; else printf("   Pitch was FLT_MAX\n");

					gdl90_float_t roll = gdl90_ahrs_roll(gpsdata);
					if (roll != FLT_MAX) _roll = roll; else printf("   Roll was FLT_MAX\n");

					gdl90_int_t heading = gdl90_ahrs_heading(gpsdata);
					if (heading != INT_MAX) _heading = heading; else printf("   Heading was INT_MAX\n");


					printf("\nCurrent data  lat: %f long: %f alti: %f\n              pitch: %f yaw: %f roll: %f heading: %f \n", _lati, _longi, _alti, _pitch, _yaw, _roll, _heading);

				}
				gdl90_destroy(gpsdata);
			}

		}
	}
	closesocket(socketC);

	return 0;
}

