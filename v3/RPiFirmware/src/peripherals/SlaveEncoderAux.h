//
// Created by curiosul on 11.12.18.
//

#ifndef RPIFIRMWARE_SLAVEENCODERAUX_H
#define RPIFIRMWARE_SLAVEENCODERAUX_H


#include "Config.h"
#include "Globals.h"
#include "drivers/Encoder.h"

class SlaveEncoderAux : public Encoder
{
public:
	explicit SlaveEncoderAux(uint8_t gpio_pin) : Encoder(gpio_pin, ENCODER_DEBOUNCING_TIME_MS)
	{
		Encoder::SetPullState(PullState::DOWN);
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
