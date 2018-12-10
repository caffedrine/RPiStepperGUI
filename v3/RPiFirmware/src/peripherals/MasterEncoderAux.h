//
// Created by curiosul on 11.12.18.
//

#ifndef RPIFIRMWARE_MASTERENCODERAUX_H
#define RPIFIRMWARE_MASTERENCODERAUX_H


#include "Config.h"
#include "Globals.h"
#include "drivers/Encoder.h"

class MasterEncoderAux : public Encoder
{
public:
	explicit MasterEncoderAux(uint8_t gpio) : Encoder(gpio, ENCODER_DEBOUNCING_TIME_MS)
	{
		Encoder::SetPullState(PullState::DOWN);
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
