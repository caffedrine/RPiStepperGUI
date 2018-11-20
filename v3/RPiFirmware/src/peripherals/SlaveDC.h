//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SLAVEDC_H
#define RPIFIRMWARE_SLAVEDC_H

#include "Config.h"
#include "drivers/DcMotor.h"

class SlaveDC : public DcMotor
{
public:
	SlaveDC (uint8_t PwmPin, uint8_t DirPin) : DcMotor(PwmPin, DirPin)
	{
	
	}
private:
};

SlaveDC g_SlaveDC(MOTOR_SLAVE_PWM_GPIO, MOTOR_SLAVE_DIR_GPIO);

#endif //RPIFIRMWARE_SLAVEDC_H
