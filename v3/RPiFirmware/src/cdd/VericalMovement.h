//
// Created by curiosul on 11/23/18.
//

#ifndef RPIFIRMWARE_VERTICALMOVEMENT_H
#define RPIFIRMWARE_VERTICALMOVEMENT_H

#include "Globals.h"
#include "drivers/Pid.h"
#include "peripherals/MasterEncoder.h"
#include "peripherals/SlaveEncoder.h"
#include "peripherals/MasterDC.h"
#include "peripherals/SlaveDC.h"
#include "peripherals/SensorVerticalMaster.h"
#include "peripherals/SensorVerticalSlave.h"

class VerticalMovement
{
public:
	
	uint16_t Position = 0, TargetPosition = 0;
	uint16_t PositionMM = 0, TargetPositionMM = 0;
	bool IsWorking = false;
	
	void MoveToMM(uint16_t mm)
	{
		this->TargetPositionMM = mm;
	}
	
	void MoveTo(uint16_t encoder_units)
	{
		this->TargetPosition = encoder_units;
	}
	
	
	void MoveUp()
	{
		g_MasterDC.SetDirection(MotorDcDirection::FORWARD);
		g_SlaveDC.SetDirection(MotorDcDirection::FORWARD);
		g_MasterDC.Run();
		g_SlaveDC.Run();
	}
	
	void MoveDown()
	{
		g_MasterDC.SetDirection(MotorDcDirection::BACKWARD);
		g_SlaveDC.SetDirection(MotorDcDirection::BACKWARD);
		g_MasterDC.Run();
		g_SlaveDC.Run();
	}
	
	void Stop()
	{
		/*  Stop */
		g_MasterDC.Stop();
		g_SlaveDC.Stop();
	}
	
	void Reset()
	{
		/* Move motors to init buttons then stop */
		
		Stop();
		Position = 0;
		TargetPosition = 0;
		PositionMM = 0;
		TargetPositionMM = 0;
		IsWorking = false;
		g_MasterEncoder.Reset();
		g_SlaveEncoder.Reset();
	}
	
	uint16_t Enc2Mm(uint32_t enc)
	{
	
	}
	
	uint32_t Mm2Enc(uint16_t mm)
	{
	
	}
	
	void Tick()
	{
	
	}
	
private:
	uint32_t PositionMaster = 0;
	uint32_t PositionSlave = 0;
	
};

VerticalMovement g_Vertical;

#endif //RPIFIRMWARE_VERTICALMOVEMENT_H
