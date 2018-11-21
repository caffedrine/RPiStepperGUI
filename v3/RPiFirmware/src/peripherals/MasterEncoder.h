//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_MASTERENCODER_H
#define RPIFIRMWARE_MASTERENCODER_H

#include "Config.h"
#include "drivers/Encoder.h"

class MasterEncoder : public Encoder
{
public:
	MasterEncoder(uint8_t gpio) : Encoder(gpio)
	{
		Encoder::SetPullstate(PullState::DOWN);
	}
};

MasterEncoder g_MasterEncoder(ENCODER_MASTER_GPIO);

#endif //RPIFIRMWARE_MASTERENCODER_H
