#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

typedef enum Params
{
    SENZOR_LEFT,
    SENZOR_RIGHT,
    MOTOR_LEFT,
    MOTOR_RIGHT,
    CUTTER,
    MOTOR_CUTTER,
    SENSOR_CUTTER,
    SENSOR_INIT_VERTICAL,
    SENSOR_INIT_CUTTER,
    ACK,
    ACK_OK
} param_t;

typedef struct Packet
{
    //param_t param;
    uint8_t value;
} packet_t;

#endif // PACKET_H
