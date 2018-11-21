//
// Created by curiosul on 11/20/18.
//

#ifndef _RELAY_H
#define _RELAY_H

#include "Gpio.h"

class Relay : private Gpio
{
public:
	Relay(uint8_t _gpio_pin);
	
	void SetReversedPolarity(bool reversed);
	void SetPullState(PullState new_state);
	void CloseCircuit();
	void OpenCircuit();
private:
};

#endif //_RELAY_H
