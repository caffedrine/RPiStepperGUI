//
// Created by curiosul on 11.12.18.
//

#ifndef RPIFIRMWARE_SLAVEENCODERAUX_H
#define RPIFIRMWARE_SLAVEENCODERAUX_H


#include "Config.h"
#include "Globals.h"
#include "drivers/EncoderInterrupt.h"

class SlaveEncoderAux : public EncoderInterrupt
{
public:
	explicit SlaveEncoderAux(uint8_t gpio_pin) : EncoderInterrupt(gpio_pin, ENCODER_DEBOUNCING_TIME_US)
	{
		EncoderInterrupt::SetPullState(PullState::DOWN);
	}
	
	void onStep() override
	{
		OnSlaveEncoderAuxStep();
		//console->info("Slave encoder: {}", Encoder::Steps);
	}
protected:
	virtual void OnSlaveEncoderAuxStep(){};
private:
};

#endif //RPIFIRMWARE_SLAVEENCODERAUX_H
