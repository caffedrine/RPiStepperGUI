//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSORVERTICAL_H
#define RPIFIRMWARE_SENSORVERTICAL_H


#include "Config.h"
#include "drivers/Gpio.h"
#include "communication/ServerTCP.h"

class SensorVertical : public Gpio
{
public:
	SensorVertical(uint8_t _gpio) : Gpio(_gpio)
	{
	}

private:
	void onStateChanged(LogicalLevel newState) override
	{
		console->info("Vertical init button: {0}", (bool)newState);
	}
};

SensorVertical g_SensorVertical(SENSOR_INIT_VERTICAL);


#endif //RPIFIRMWARE_SENSORVERTICAL_H
