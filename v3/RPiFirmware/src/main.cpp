#include <iostream>
#include <signal.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

#include "utils/time_utils.h"
#include "Common.h"
#include "drivers/hal.h"
#include "peripherals/CutterDC.h"
#include "peripherals/CutterRelay.h"
#include "peripherals/ElectroValves.h"
#include "peripherals/Led.h"
#include "peripherals/MasterDC.h"
#include "peripherals/SlaveDC.h"
#include "peripherals/SensorHorizontal.h"
#include "peripherals/SensorVertical.h"
#include "peripherals/SensorLaser.h"
#include "peripherals/MasterEncoder.h"
#include "peripherals/SlaveEncoder.h"

#include "communication/ServerTCP.h"

bool _ProgramContinue = true;

void OnExit()
{
	//g_Motor.Stop();
	console->info("Executing STOP routine...");
}

void SigHandler(int signum)
{
	std::cout << std::endl;
	console->critical("Interrupt signal ({0} - {1}) received", strsignal(signum), signum);
	OnExit();
	_ProgramContinue = false;
	exit(signum);
}

void Initialize()
{
	/* Handle interruption signals */
	signal(SIGINT, SigHandler);
	signal(SIGQUIT, SigHandler);
	signal(SIGTSTP, SigHandler);
}

void onCommandReceived()
{

}

int main()
{
	console->info("Started main()...");
	Initialize();
	
	
	while( true )
	{
		if( !_ProgramContinue )
			break;
	
		/* Check for client's responsiveness */
		if(g_TcpServer.client != NULL && g_TcpServer.client->IsConnected() > 0 && TimeUtils::millis() - g_TcpRecvLastMillis > TCP_ACK_INTERVAL_MS)
		{
			console->warn("Client ACK timeout reached", TCP_ACK_INTERVAL_MS);
			console->warn("Dropped {0}:{1}", g_TcpServer.client->Ip, g_TcpServer.client->Port);
			/* Try send null packet to inform about disconnect */
			g_TcpServer.client->Disconnect();
		}

		g_TcpServer.Send("Alex!", 5);
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	return 0;
}