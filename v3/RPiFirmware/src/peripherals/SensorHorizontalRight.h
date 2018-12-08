//
// Created by curiosul on 21.11.18.
//

#ifndef RPIFIRMWARE_SENSORHORIZONTALRIGHT_H
#define RPIFIRMWARE_SENSORHORIZONTALRIGHT_H

#include "Config.h"
#include "Globals.h"
#include "drivers/PushButton.h"

class SensorHorizontalRight : public PushButton
{
public:
	explicit SensorHorizontalRight(uint8_t  gpio) : PushButton(gpio, BUTTONS_DBOUNCING_TIME_MS)
	{
		PushButton::SetPullState(PullState::DOWN);
	}
};

SensorHorizontalRight g_SensorHorizontalRight(SENSOR_INIT_HORIZONTAL_RIGHT_GPIO);

#endif //RPIFIRMWARE_SENSORHORIZONTALRIGHT_H
