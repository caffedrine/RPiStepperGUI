//
// Created by curiosul on 11/20/18.
//

#ifndef _LED_H
#define _LED_H

#include "Gpio.h"

class Led : private Gpio
{
public:
	Led(uint8_t _gpio_pin);
	
	void SetReversedPolarity(bool reversed);
	void On();
	void Off();
private:

};


#endif //_LED_H
