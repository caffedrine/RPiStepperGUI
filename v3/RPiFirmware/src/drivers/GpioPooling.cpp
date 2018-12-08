//
// Created by curiosul on 08.12.18.
//

#include "GpioPooling.h"

GpioPooling::GpioPooling(int GpioPin) : GpioBase(GpioPin)
{
	this->Pin = GpioPin;
	this->Init();
}

GpioPooling::GpioPooling(int GpioPin, int DebounceTimeMillis) : GpioBase(GpioPin)
{
	this->Pin = GpioPin;
	this->DebounceTimeMs = DebounceTimeMillis;
	this->Init();
}

GpioPooling::~GpioPooling()
{

}

void GpioPooling::Init()
{

}

void GpioPooling::Tick()
{
	static uint64_t LastTicks = 0;
	this->CurrentState = GpioBase::Read();
	
	if(this->CurrentState != this->PreviousState)
	{
		if( TimeUtils::millis() - LastTicks >= this->DebounceTimeMs )
		{
			LastTicks = TimeUtils::millis();
			this->PreviousState = this->CurrentState;
			
			if( this->LevelChangedCbFunc > 0 )
				this->LevelChangedCbFunc(this->CurrentState);
			
			onGpioStateChanged(this->CurrentState);
		}
	}
}

void GpioPooling::SetStateChangedCallback(level_changed_cb_t f)
{
	this->LevelChangedCbFunc = f;
}

void GpioPooling::onGpioStateChanged(LogicalLevel newState)
{

}
