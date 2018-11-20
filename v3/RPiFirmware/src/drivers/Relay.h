//
// Created by curiosul on 11/20/18.
//

#ifndef _RELAY_H
#define _RELAY_H

#include "Gpio.h"

class Relay : Gpio
{
	Relay(uint8_t _gpio_pin);
	
	void SetReversedPolarity(bool reversed);
	void CloseCircuit();
	void OpenCircuit();
};

#endif //_RELAY_H
