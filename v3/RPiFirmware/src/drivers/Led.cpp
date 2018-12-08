//
// Created by curiosul on 11/20/18.
//

#include "Led.h"

Led::Led(uint8_t _gpio_pin) : GpioInterrupt(_gpio_pin)
{
	GpioInterrupt::SetMode(PinMode::OUTPUT);
	this->Off();
}

void Led::SetReversedPolarity(bool reversed)
{
	GpioInterrupt::SetReversedPolarity(reversed);
}

void Led::On()
{
	GpioInterrupt::Write(LogicalLevel::HIGH);
}

void Led::Off()
{
	GpioInterrupt::Write(LogicalLevel::LOW);
}