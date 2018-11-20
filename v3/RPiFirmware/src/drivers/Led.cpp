//
// Created by curiosul on 11/20/18.
//

#include "Led.h"

Led::Led(uint8_t _gpio_pin) : Gpio(_gpio_pin)
{
	Gpio::SetMode(PinMode::OUTPUT);
	this->Off();
}

void Led::SetReversedPolarity(bool reversed)
{
	Gpio::SetReversedPolarity(reversed);
}

void Led::On()
{
	Gpio::Write(LogicalLevel::HIGH);
}

void Led::Off()
{
	Gpio::Write(LogicalLevel::LOW);
}