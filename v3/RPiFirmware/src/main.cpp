#include <iostream>
#include <signal.h>
#include <string.h>
#include <chrono>
#include <thread>

#include <spdlog/spdlog.h>

#include "Common.h"
#include "communication/ServerTCP.h"
#include "drivers/hal.h"
#include "peripherals/RightButton.h"
#include "peripherals/LeftButton.h"
#include "peripherals/Motor.h"

bool _ProgramContinue = true;

void OnExit()
{
	g_Motor.Stop();
	console->info("Executing STOP routine...");
}

void SigHandler(int signum)
{
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

int main()
{
	console->info("---STARTED---");
	Initialize();
	
	while( true )
	{
		if(!_ProgramContinue)
			break;
		
		std::this_thread::sleep_for( std::chrono::milliseconds(1) );
	}
	return 0;
}