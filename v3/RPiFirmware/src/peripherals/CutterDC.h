//
// Created by curiosul on 11/15/18.
//

#ifndef RPIFIRMWARE_CUTTERDC_H
#define RPIFIRMWARE_CUTTERDC_H

#include "Config.h"
#include "drivers/DcMotor.h"

class CutterDC : public DcMotor
{
public:
	CutterDC(uint8_t PulsePin, uint8_t DirPin) : DcMotor(PulsePin, DirPin)
	{
		DcMotor::PwmConfig(DC_MOTORS_FREQ_HZ, DC_MOTOR_DEFAULT_SPEED);
	}
private:
};

CutterDC g_CutterDC(MOTOR_CUTTER_PWM_GPIO, MOTOR_CUTTER_DIR_GPIO);

#endif //RPIFIRMWARE_CUTTERDC_H
