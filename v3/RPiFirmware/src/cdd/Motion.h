//
// Created by curiosul on 01.12.18.
//

#ifndef RPIFIRMWARE_COMMANDS_H
#define RPIFIRMWARE_COMMANDS_H

#include "Globals.h"
#include "cdd/VericalMovement.h"
#include <peripherals/CutterDC.h>
#include <peripherals/SensorHorizontalLeft.h>
#include <peripherals/Cutter.h>

static void HandleEmergencyStop()
{
	console->warn("Emergency stop request received!");
	g_State.Set(States::EMERGENCY_STOP);
}

static void HandleUp()
{
	g_Vertical.MoveUp();
}

static void HandleDown()
{
	g_Vertical.MoveDown();
}

static void HandleRight()
{
	g_CutterDC.SetDirection(MotorDcDirection::FORWARD);
	g_CutterDC.Run();
}

static void HandleLeft()
{
	g_CutterDC.SetDirection(MotorDcDirection::BACKWARD);
	g_CutterDC.Run();
}

static void HandleReset()
{
	if(g_SensorHorizontalLeft.CurrentState == PushButtonState::UP)
	{
		g_CutterDC.SetDirection(MotorDcDirection::BACKWARD);
		g_CutterDC.Run();
	}
	
	if(g_SensorVerticalSlave.CurrentState == PushButtonState::UP || g_SensorVerticalMaster.CurrentState == PushButtonState::UP)
	{
		g_Vertical.Reset();
	}
	g_State.Set(States::WAIT_RESET);
}

static void HandleMoveTo(uint16_t targetRecv)
{
	if( targetRecv < VERTICAL_MM_OFFSET )
	{
		console->warn("[MOVETO] Error! Value {}mm is below offset {}mm", targetRecv, VERTICAL_MM_OFFSET);
		return;
	}
	
	if((uint16_t)g_Vertical.GetCurrPositionMM() != targetRecv)
	{
		console->info("[MOVETO] Start moving to {} mm", targetRecv);
		g_Vertical.MoveToMM(targetRecv);
		g_State.Set(States::WAIT_MOVETO);
	}
	else
	{
		console->info("[MOVETO] Abort!!! Already at position {}", targetRecv);
	}
}

static void HandleCut()
{
	if(g_SensorHorizontalLeft.CurrentState != PushButtonState::DOWN)
	{
		console->info("Machinery not initialized");
		return;
	}
	
	g_Cutter.On();
	g_State.Set(States::WAIT_CUT);
}

#endif //RPIFIRMWARE_COMMANDS_H
