//
// Created by curiosul on 11.12.18.
//

#ifndef RPIFIRMWARE_MASTERENCODERAUX_H
#define RPIFIRMWARE_MASTERENCODERAUX_H


#include "Config.h"
#include "Globals.h"
#include "drivers/EncoderInterrupt.h"

class MasterEncoderAux : public EncoderInterrupt
{
public:
	explicit MasterEncoderAux(uint8_t gpio) : EncoderInterrupt(gpio, ENCODER_DEBOUNCING_TIME_US)
	{
		EncoderInterrupt::SetPullState(PullState::DOWN);
	}
	
	void onStep() override
	{
		OnMasterEncoderAuxStep();
	}
protected:
	virtual void OnMasterEncoderAuxStep() {};
private:
};


#endif //RPIFIRMWARE_MASTERENCODERAUX_H
