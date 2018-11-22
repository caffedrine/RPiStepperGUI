//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSORHORIZONTLNASTER_H
#define RPIFIRMWARE_SENSORHORIZONTLNASTER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/PushButton.h"

class SensorHorizontalLeft : public PushButton
{
public:
	explicit SensorHorizontalLeft(uint8_t _gpio) : PushButton(_gpio, 100000)
	{
		PushButton::SetPullState(PullState::DOWN);
	}
};

SensorHorizontalLeft g_SensorHorizontalLeft(SENSOR_INIT_HORIZONTAL_LEFT_GPIO);


#endif //RPIFIRMWARE_SENSORHORIZONTLNASTER_H
