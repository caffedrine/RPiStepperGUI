//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSORLASER_H
#define RPIFIRMWARE_SENSORLASER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/GpioInterrupt.h"

class SensorLaser : public GpioInterrupt
{
public:
	explicit SensorLaser(uint8_t _gpio) : GpioInterrupt(_gpio)
	{
		GpioInterrupt::SetPullState(PullState::DOWN);
	}
	
	bool IsCourtainPresent()
	{
		return !(bool)(GpioInterrupt::CurrentState);
	}
};

SensorLaser g_SensorLaser(SENSOR_LASER_CUT_GPIO);

#endif //RPIFIRMWARE_SENSORLASER_H
