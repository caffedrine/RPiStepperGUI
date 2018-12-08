//
// Created by curiosul on 11/21/18.
//

#ifndef RPIFIRMWARE_SENSOR_VERTICAL_SLAVE_H
#define RPIFIRMWARE_SENSOR_VERTICAL_SLAVE_H

#include "Config.h"
#include "Globals.h"
#include "drivers/PushButton.h"

class SensorVerticalSlave : public PushButton
{
public:
	SensorVerticalSlave(uint8_t _gpio) : PushButton(_gpio, BUTTONS_DBOUNCING_TIME_MS)
	{
		PushButton::SetPullState(PullState::DOWN);
	}
};

SensorVerticalSlave g_SensorVerticalSlave(SENSOR_INIT_VERTICAL_SLAVE_GPIO);

#endif //RPIFIRMWARE_SENSOR_VERTICAL_SLAVE_H
