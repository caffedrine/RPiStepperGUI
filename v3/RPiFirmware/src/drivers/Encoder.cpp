//
// Created by curiosul on 11/15/18.
//

#include "Encoder.h"

Encoder::Encoder(uint8_t _gpio_pin) : GpioInterrupt(_gpio_pin)
{
	this->gpio_pin = _gpio_pin;
}

void Encoder::Reset()
{
	this->Steps = 0;
}

void Encoder::onStateChanged(LogicalLevel newState)
{
	GpioInterrupt::onStateChanged(newState);
//	if( newState == (LogicalLevel) EncoderState::HIGH )
//	{
		this->Steps++;
		this->State = (EncoderState) newState;
		onStep();
//	}
}

void Encoder::ReversePolarity()
{
	GpioInterrupt::ReversedPolarity = !GpioInterrupt::ReversedPolarity;
}

void Encoder::SetDebouncer(int micros)
{
	GpioInterrupt::DebounceTimeUs = micros;
}

void Encoder::SetPullstate(PullState newPullstate)
{
	GpioInterrupt::SetPullState(newPullstate);
}



