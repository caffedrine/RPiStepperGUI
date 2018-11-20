#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

enum class PacketParams
{
	/* Peripherals */
    SENZOR_LEFT,
    SENZOR_RIGHT,
    CUTTER,
    MOTOR_CUTTER,
    SENSOR_CUTTER,
    SENSOR_INIT_VERTICAL,
    SENSOR_INIT_CUTTER,
	ELECTROVALVES,

	/* Commands */
	RESET,
	STOP,
	CUT,
	MOVETO,
	UP,
	DOWN,
	RIGHT,
	LEFT,

	/* Protocol commands */
    ACK,
    ACK_OK
};



#pragma pack(push, 1)
typedef struct Packet
{
	PacketParams param;
	uint16_t value;
} packet_t;
#pragma pack(pop)

#endif // PACKET_H
