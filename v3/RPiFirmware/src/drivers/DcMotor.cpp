//
// Created by curiosul on 11/20/18.
//

#include "DcMotor.h"

DcMotor::DcMotor(uint8_t Pulse)
{
	this->GpioPulse = Pulse;
	Init();
}

DcMotor::DcMotor(uint8_t Pulse, uint8_t Direction)
{
	this->GpioPulse = Pulse;
	this->GpioDirection = Direction;
	this->Init();
}

DcMotor::DcMotor(uint8_t Pulse, uint8_t Direction, uint8_t Enable)
{
	this->GpioPulse = Pulse;
	this->GpioDirection = Direction;
	this->GpioEnable = Enable;
	this->Init();
}

void DcMotor::Init()
{
	/* Init GPIO pins */
	Vfb_GpioInitialise();
	Vfb_SetPinMode( this->GpioPulse, PinMode::OUTPUT);
	
	if(this->GpioEnable > 0)
	{
		Vfb_SetPinMode(this->GpioEnable, PinMode::OUTPUT);
		this->Disable();
	}
	
	if(this->GpioDirection > 0)
		Vfb_SetPinMode(this->GpioDirection, PinMode::OUTPUT);
}

bool DcMotor::IsRunning()
{
	return !(CurrentState == MotorDcState ::STOPPED);
}

long DcMotor::PwmConfig(unsigned freq, uint8_t duty)
{
	this->PwmDuty = duty;
	this->PwmFreqHz = freq;
	
	/* Set Freq */
	long result = Vfb_InitPwm(this->GpioPulse, this->PwmFreqHz);
	/* Stop motor until Run() */
	Vfb_PwmOut(this->GpioPulse, 0 );
	this->CurrentState = MotorDcState::STOPPED;
	
	/* return actual freq */
	return  result;
}

void DcMotor::SetSpeed(uint8_t speed)
{
	this->PwmDuty = speed;

	if(this->CurrentState == MotorDcState::RUNNING)
		Vfb_PwmOut(this->GpioPulse, this->PwmDuty);
	else
		Vfb_PwmOut(this->GpioPulse, 0);
}

void DcMotor::Stop()
{
	if(!this->IsRunning())
		return;
	
	this->CurrentState = MotorDcState::STOPPED;
	Vfb_PwmOut(this->GpioPulse, 0);
	
	this->Disable();
}

void DcMotor::SetDirection(MotorDcDirection new_direction)
{
	if(this->GpioDirection > 0)
	{
		if( new_direction == MotorDcDirection::FORWARD )
			Vfb_WriteGpio(this->GpioDirection, LogicalLevel::HIGH);
		else
			Vfb_WriteGpio(this->GpioDirection, LogicalLevel::LOW);
	}
}

void DcMotor::Run()
{
	this->CurrentState = MotorDcState::RUNNING;
	
	/* Provide PWM to the output */
	Vfb_PwmOut(this->GpioPulse, this->PwmDuty);
	
	this->Enable();
}

void DcMotor::Enable()
{
	if(this->GpioEnable > 0)
		Vfb_WriteGpio(this->GpioEnable, LogicalLevel::HIGH);
}

void DcMotor::Disable()
{
	if(this->GpioEnable > 0)
		Vfb_WriteGpio(this->GpioEnable, LogicalLevel::HIGH);
}


