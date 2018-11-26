//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSORLASER_H
#define RPIFIRMWARE_SENSORLASER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/Gpio.h"

class SensorLaser : public Gpio
{
public:
	explicit SensorLaser(uint8_t _gpio) : Gpio(_gpio)
	{
		Gpio::SetPullState(PullState::DOWN);
	}
	
	bool IsCourtainPresent()
	{
		return !(bool)(Gpio::CurrentState);
	}
};

SensorLaser g_SensorLaser(SENSOR_LASER_CUT_GPIO);

#endif //RPIFIRMWARE_SENSORLASER_H
