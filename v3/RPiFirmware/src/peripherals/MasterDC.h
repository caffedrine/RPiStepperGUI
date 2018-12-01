//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_MASTERDC_H
#define RPIFIRMWARE_MASTERDC_H

#include "Config.h"
#include "drivers/DcMotor.h"

class MasterDC : public DcMotor
{
public:
	MasterDC(uint8_t PwmPin, uint8_t DirPin) : DcMotor(PwmPin, DirPin)
	{
		DcMotor::PwmConfig(DC_MOTORS_FREQ_HZ, DC_MOTOR_DEFAULT_SPEED);
	}
private:
};

MasterDC g_MasterDC(MOTOR_MASTER_PWM_GPIO, MOTOR_MASTER_DIR_GPIO);

#endif //RPIFIRMWARE_MASTERDC_H
