//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSORLASER_H
#define RPIFIRMWARE_SENSORLASER_H


#include "Config.h"
#include "drivers/Gpio.h"
#include "communication/ServerTCP.h"

class SensorLaser : public Gpio
{
public:
	SensorLaser(uint8_t _gpio) : Gpio(_gpio)
	{
	}

private:
	void onStateChanged(LogicalLevel newState) override
	{
		console->info("Laser button: {0}", (bool)newState);
	}
};

SensorLaser g_SensorLaser(SENSOR_LASER_CUT);


#endif //RPIFIRMWARE_SENSORLASER_H
