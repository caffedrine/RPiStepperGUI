//
// Created by curiosul on 11/20/18.
//

#include "Relay.h"

Relay::Relay(uint8_t _gpio_pin) : GpioInterrupt(_gpio_pin)
{
	GpioInterrupt::SetMode(PinMode::OUTPUT);
	this->OpenCircuit();
}

void Relay::SetReversedPolarity(bool reversed)
{
	GpioInterrupt::SetReversedPolarity(reversed);
}

void Relay::CloseCircuit()
{
	GpioInterrupt::Write(LogicalLevel::HIGH);
}

void Relay::OpenCircuit()
{
	GpioInterrupt::Write(LogicalLevel::LOW);
}

void Relay::SetPullState(PullState new_state)
{
	GpioInterrupt::SetPullState(new_state);
}
