#include <iostream>
#include <signal.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <spdlog/spdlog.h>

#include "utils/time_utils.h"
#include "Globals.h"
#include "drivers/hal.h"
#include "peripherals/CutterDC.h"
#include "peripherals/Cutter.h"
#include "peripherals/ElectroValves.h"
#include "peripherals/LedConnection.h"
#include "peripherals/MasterDC.h"
#include "peripherals/SlaveDC.h"
#include "communication/ServerTCP.h"
#include "peripherals/SensorVerticalMaster.h"
#include "peripherals/SensorVerticalSlave.h"
#include "peripherals/SensorHorizontal.h"
#include "peripherals/SensorLaser.h"
#include "peripherals/MasterEncoder.h"
#include "peripherals/SlaveEncoder.h"

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
	g_LedConnection.Off();
	g_LedTraffic.Off();
	g_MasterDC.Stop();
	g_SlaveDC.Stop();
	g_CutterDC.Stop();
	g_Cutter.Off();
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


/*
 * TODO: MUTEX MUTEX MUTEX to be added
 */
 

int main()
{
	console->info("Started main()...");
	Initialize();
	
	while( true )
	{
		if( !_ProgramContinue )
			break;
	
//		/* Check for client's responsiveness */
//		if(g_TcpServer.client != NULL && g_TcpServer.client->IsConnected() > 0 && TimeUtils::millis() - g_TcpRecvLastMillis > TCP_ACK_INTERVAL_MS)
//		{
//			console->warn("Client ACK timeout reached", TCP_ACK_INTERVAL_MS);
//			console->warn("Dropped {0}:{1}", g_TcpServer.client->Ip, g_TcpServer.client->Port);
//			/* Drop client connection socket */
//			g_TcpServer.client->Disconnect();
//			/* Go to emergency stop until next client is connected */
//			g_State.Set(States::EMERGENCY_STOP);
//		}
		
		/* Handle EMERGENCY_STOP state */
		if(g_State.Current.Val == States::EMERGENCY_STOP)
		{
			g_Cutter.Off();
			g_CutterDC.Stop();
			g_MasterDC.Stop();
			g_SlaveDC.Stop();
			
			g_State.Set(States::STANDBY);
		}
		
		/* Handle WAIT_RESET state */
		else if(g_State.Current.Val == States::WAIT_RESET)
		{
		
		}
		
		/* Handle WAIT_MOVETO state */
		else if(g_State.Current.Val == States::WAIT_MOVETO)
		{
		
		}
		
		/* Handle WAIT_CUT state */
		else if(g_State.Current.Val == States::WAIT_CUT)
		{
		
		}
		
		/* Handle WAIT_CUTTER_INIT state */
		else if(g_State.Current.Val == States::WAIT_CUTTER_INIT)
		{
		
		}
		
		/* Handle WAIT_LOCK state */
		else if(g_State.Current.Val == States::WAIT_LOCK)
		{
		
		}
		
		/* Handle WAIT_UNLOCK state */
		else if(g_State.Current.Val == States::WAIT_UNLOCK)
		{
		
		}
		
		/* Prevent excessive CPU load */
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}