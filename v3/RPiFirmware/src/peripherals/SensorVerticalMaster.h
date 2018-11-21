//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H
#define RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/Gpio.h"
#include "communication/ServerTCP.h"

class SensorVerticalMaster : public Gpio
{
public:
	explicit SensorVerticalMaster(uint8_t _gpio) : Gpio(_gpio)
	{
	}
	
private:
	void onStateChanged(LogicalLevel newState) override
	{
		console->info("Sensor init button horizontal master: {0}", (bool)newState);
		static Packet packet = { .param = PacketParams::SENSOR_INIT_CUTTER };
		packet.value = (uint8_t)newState;
		g_TcpServer.SendPacket( &packet );
	}
};

SensorVerticalMaster g_SensorVerticalMaster(SENSOR_INIT_VERTICAL_MASTER_GPIO);

#endif //RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H
