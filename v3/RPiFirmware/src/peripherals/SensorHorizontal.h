//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSORHORIZONTAL_H
#define RPIFIRMWARE_SENSORHORIZONTAL_H

#include "Config.h"
#include "drivers/Gpio.h"
#include "communication/ServerTCP.h"

class SensorHorizontal : public Gpio
{
public:
	SensorHorizontal(uint8_t _gpio) : Gpio(_gpio)
	{
	}
	
private:
	void onStateChanged(LogicalLevel newState) override
	{
		console->info("Horizontal init button: {0}", (bool)newState);
	}
};

SensorHorizontal g_SensorHorizontal(SENSOR_INIT_HORIZONTAL_GPIO);

#endif //RPIFIRMWARE_SENSORHORIZONTAL_H
