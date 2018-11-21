//
// Created by curiosul on 11/21/18.
//

#ifndef RPIFIRMWARE_SENSOR_VERTICAL_SLAVE_H
#define RPIFIRMWARE_SENSOR_VERTICAL_SLAVE_H

#include "Config.h"
#include "Globals.h"
#include "drivers/Gpio.h"
//#include "communication/ServerTCP.h"

class SensorVerticalSlave : public Gpio
{
public:
	SensorVerticalSlave(uint8_t _gpio) : Gpio(_gpio)
	{
	}

private:
	void onStateChanged(LogicalLevel newState) override
	{
		console->info("Horizontal init button slave: {0}", (bool)newState);
		static Packet packet = { .param = PacketParams::SENSOR_INIT_CUTTER };
		packet.value = (uint8_t)newState;
		//g_TcpServer.SendPacket( &packet );
	}
};

SensorVerticalSlave g_SensorVerticalSlave(SENSOR_INIT_VERTICAL_SLAVE_GPIO);

#endif //RPIFIRMWARE_SENSOR_VERTICAL_SLAVE_H
