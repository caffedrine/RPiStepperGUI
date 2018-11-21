//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H
#define RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/PushButton.h"

class SensorVerticalMaster : public PushButton
{
public:
	explicit SensorVerticalMaster(uint8_t _gpio) : PushButton(_gpio)
	{
		PushButton::SetPullState(PullState::DOWN);
	}
};

SensorVerticalMaster g_SensorVerticalMaster(SENSOR_INIT_VERTICAL_MASTER_GPIO);

#endif //RPIFIRMWARE_SENSOR_VERTICAL_MASTER_H
