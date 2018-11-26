//
// Created by curiosul on 11/20/18.
//

#ifndef RPIFIRMWARE_LEDTRAFFIC_H
#define RPIFIRMWARE_LEDTRAFFIC_H

#include "Config.h"
#include <drivers/Led.h>

class LedTraffic : public Led
{
public:
	LedTraffic(uint8_t _gpio_pin) : Led(_gpio_pin)
	{
	}
	
private:
};

LedTraffic g_LedTraffic(LED_TRAFFIC);

#endif //RPIFIRMWARE_LEDTRAFFIC_H
