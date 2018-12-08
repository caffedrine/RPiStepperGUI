//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_SLAVEENCODER_H
#define RPIFIRMWARE_SLAVEENCODER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/Encoder.h"

class SlaveEncoder : public Encoder
{
public:
	explicit SlaveEncoder(uint8_t gpio_pin) : Encoder(gpio_pin, ENCODER_DEBOUNCING_TIME_MS)
	{
		Encoder::SetPullState(PullState::DOWN);
	}
	
	void onStep() override
	{
		OnSlaveEncoderStep();
		//console->info("Slave encoder: {}", Encoder::Steps);
	}
protected:
	virtual void OnSlaveEncoderStep(){};
private:
};

//SlaveEncoder g_SlaveEncoder(ENCODER_SLAVE_GPIO);

#endif //RPIFIRMWARE_SLAVEENCODER_H
