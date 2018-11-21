//
// Created by curiosul on 11/21/18.
//

#ifndef RPIFIRMWARE_TCPPUBLISHER_H
#define RPIFIRMWARE_TCPPUBLISHER_H

#include "Globals.h"

typedef void (*publisher_func)(Packet *);

class TcpPublisher
{
	/* Pointer to a TCP client handler to publish change */
	publisher_func TcpSend = nullptr;
	
	void SetPublisher(publisher_func )
	{
	
	}
};


#endif //RPIFIRMWARE_TCPPUBLISHER_H
