//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_ELECTROVALVES_H
#define RPIFIRMWARE_ELECTROVALVES_H

#include "Config.h"
#include "drivers/Relay.h"

class ElectroValves : private Relay
{
public:
	ElectroValves(uint8_t gpio_pin) : Relay(gpio_pin)
	{
	
	}
	
	void On()
	{
		Relay::CloseCircuit();
	}
	
	void Off()
	{
		Relay::OpenCircuit();
	}
private:
};

ElectroValves g_ElectroValves(RELAY_ELECTROVALVES_GPIO);

#endif //RPIFIRMWARE_ELECTROVALVES_H
