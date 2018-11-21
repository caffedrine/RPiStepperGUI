//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSORHORIZONTLNASTER_H
#define RPIFIRMWARE_SENSORHORIZONTLNASTER_H


#include "Config.h"
#include "Globals.h"
#include "drivers/Gpio.h"
//#include "communication/ServerTCP.h"

class SensorHorizontal : public Gpio
{
public:
	SensorHorizontal(uint8_t _gpio) : Gpio(_gpio)
	{
	}

private:
	void onStateChanged(LogicalLevel newState) override
	{
		console->info("Vertical init button: {0}", (bool)newState);
		static Packet packet = { .param = PacketParams::SENSOR_INIT_VERTICAL_MASTER };
		packet.value = (uint8_t)newState;
		//g_TcpServer.SendPacket( &packet );
	}
};

SensorHorizontal g_SensorHorizontal(SENSOR_INIT_HORIZONTAL_GPIO);


#endif //RPIFIRMWARE_SENSORHORIZONTLNASTER_H
