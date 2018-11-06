//
// Created by curiosul on 10/9/18.
//

#ifndef DRIVERSCONTAINER_SERVERTCP_H
#define DRIVERSCONTAINER_SERVERTCP_H

#include "drivers/TcpServerAsync.h"

class ServerTCP : public TcpServerAsync
{
public:
	ServerTCP() : TcpServerAsync(1337, 5)
	{
	
	}
private:

};

ServerTCP g_TcpServer;

#endif //DRIVERSCONTAINER_SERVERTCP_H
