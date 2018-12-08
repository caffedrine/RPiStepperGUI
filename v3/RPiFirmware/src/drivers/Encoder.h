//
// Created by curiosul on 11/15/18.
//

#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <stdint.h>

#include "GpioInterrupt.h"

enum class EncoderState
{
	HIGH = 1,
	LOW = 0
};

class Encoder : GpioInterrupt
{
public:
	uint64_t Steps = 0;
	EncoderState  State;
	
	Encoder(uint8_t gpio_pin);
	void Reset();
	void ReversePolarity();
	void SetDebouncer(int micros);
	void SetPullstate(PullState newPullstate);
	
protected:
	uint8_t gpio_pin;
	void onStateChanged(LogicalLevel newState) override;
	
protected:
	virtual void onStep() {};
};


#endif //_ENCODER_H_
