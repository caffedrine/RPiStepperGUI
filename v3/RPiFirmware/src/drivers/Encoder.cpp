//
// Created by curiosul on 11/15/18.
//

#include "Encoder.h"

Encoder::Encoder(uint8_t _gpio_pin) : Gpio(_gpio_pin)
{
	this->gpio_pin = _gpio_pin;
}

void Encoder::Reset()
{
	this->Steps = 0;
}

void Encoder::onStateChanged(LogicalLevel newState)
{
	Gpio::onStateChanged(newState);
	this->Steps++;
	this->State= (EncoderState)newState;
}

void Encoder::ReversePolarity()
{
	Gpio::ReversedPolarity = !Gpio::ReversedPolarity;
}

void Encoder::SetDebouncer(int micros)
{
	Gpio::DebounceTimeUs = micros;
}

void Encoder::SetPullstate(PullState newPullstate)
{
	Gpio::SetPullState(newPullstate);
}



