//
// Created by curiosul on 11/15/18.
//

#ifndef RPIFIRMWARE_LED_H
#define RPIFIRMWARE_LED_H

#include "Config.h"
#include <drivers/Led.h>

class LedConnection : public Led
{
public:
	LedConnection(uint8_t _gpio_pin) : Led(_gpio_pin)
	{
	
	}
private:
};

LedConnection g_LedConnection(LED_CONNECTION);

#endif //RPIFIRMWARE_LED_H
