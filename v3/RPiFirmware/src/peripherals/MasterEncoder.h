//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_MASTERENCODER_H
#define RPIFIRMWARE_MASTERENCODER_H

#include "Config.h"
#include "Globals.h"
#include "drivers/Encoder.h"

class MasterEncoder : public Encoder
{
public:
	explicit MasterEncoder(uint8_t gpio) : Encoder(gpio)
	{
		Encoder::SetPullstate(PullState::DOWN);
	}
	
	void onStep() override
	{
		console->info("Master encoder: {0}", Encoder::Steps);
	}
private:
};

MasterEncoder g_MasterEncoder(ENCODER_MASTER_GPIO);

#endif //RPIFIRMWARE_MASTERENCODER_H
