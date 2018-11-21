#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define PACKET_SIZE				4
#define START_PACKET_SYMBOL		'#'
#define END_PACKET_SYMBOL		'$'

enum PacketParams
{
	/* Manual movements */
    CUTTER,
	ELECTROVALVES,
	UP,
	DOWN,
	RIGHT,
	LEFT,
	
	/* Received params */
	SENSOR_CUTTER_LASER,
	SENSOR_INIT_VERTICAL_MASTER,
	SENSOR_INIT_VERTICAL_SLAVE,
	SENSOR_INIT_HORIZONTAL_LEFT,
	SENSOR_INIT_HORIZONTAL_RIGHT,
	
	/* Commands */
	RESET,
	EMERGENCY_STOP,
	CUT,
	MOVETO,

	/* Protocol commands */
    ACK,

    /* Number of elements of this enum */
    ELEMENTS_NO
};

#pragma pack(push, 1)
typedef struct Packet
{
	uint8_t param;
	uint8_t value;
} packet_t;
#pragma pack(pop)

int Serialize(const Packet *inputPacket, char *output)
{
	/* Create a new char array to store serialized structure */
	char serialized[PACKET_SIZE];
	/* Start and end symbols */
	serialized[0] = START_PACKET_SYMBOL;
	serialized[PACKET_SIZE-1] = END_PACKET_SYMBOL;
	/* Populate serialized array */
	serialized[1] = (char)(inputPacket->param);
	serialized[2] = (char)(inputPacket->value);
	/* Assign created array to output */
	memcpy(output, serialized, PACKET_SIZE);
	
	/* Return the length */
	return (sizeof(serialized)/ sizeof(char));
}

int Deserialize(const char *recvBytes, int length, Packet *output)
{
	if(length != PACKET_SIZE)
		return -1;
	
	if(recvBytes[0] != START_PACKET_SYMBOL)
		return -1;
	
	if(recvBytes[length-1] != END_PACKET_SYMBOL)
		return -1;
	
	/* Read packet field */
	output->param = (uint8_t)recvBytes[1];
	output->value = (uint8_t)recvBytes[2];
	
	/* Check whether values are valid or not */
	if( ( (output->param) < 0) || ( output->param > PacketParams::ELEMENTS_NO) )
		return -1;
	
	return 0;
}


#endif // PACKET_H
