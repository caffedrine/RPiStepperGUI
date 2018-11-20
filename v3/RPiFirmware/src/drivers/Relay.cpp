//
// Created by curiosul on 11/20/18.
//

#include "Relay.h"

Relay::Relay(uint8_t _gpio_pin) : Gpio(_gpio_pin)
{
	Gpio::SetMode(PinMode::OUTPUT);
	this->OpenCircuit();
}

void Relay::SetReversedPolarity(bool reversed)
{
	Gpio::SetReversedPolarity(reversed);
}

void Relay::CloseCircuit()
{
	Gpio::Write(LogicalLevel::HIGH);
}

void Relay::OpenCircuit()
{
	Gpio::Write(LogicalLevel::LOW);
}
