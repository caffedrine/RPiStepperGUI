//
// Created by curiosul on 06.11.18.
//

#ifndef RPIFIRMWARE_CUTTERRELAY_H
#define RPIFIRMWARE_CUTTERRELAY_H

#include "Config.h"
#include "drivers/Relay.h"

class Cutter : private Relay
{
public:
	Cutter(uint8_t RelayPin) : Relay(RelayPin)
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

Cutter g_Cutter(RELAY_CUTTER_ENB_GPIO);

#endif //RPIFIRMWARE_CUTTERRELAY_H
