//
// Created by curiosul on 08.12.18.
//

#ifndef RPIFIRMWARE_GPIOPOOLING_H
#define RPIFIRMWARE_GPIOPOOLING_H

#include "hal.h"
#include "GpioBase.h"
#include "utils/time_utils.h"

typedef void (*level_changed_cb_t)(LogicalLevel);

class GpioPooling : public GpioBase
{
public:
	int Pin;
	bool ReversedPolarity = false;
	int DebounceTimeMs = 0;
	LogicalLevel CurrentState, PreviousState;
	
	explicit GpioPooling(int GpioPin);
	GpioPooling(int GpioPin, int DebounceTimeMillis);
	~GpioPooling();
	
	
	void Tick();
	void SetStateChangedCallback( level_changed_cb_t );

private:
	/* State changed callback function*/
	level_changed_cb_t  LevelChangedCbFunc = NULL;
	void Init();

protected:
	virtual void onGpioStateChanged(LogicalLevel newState);
};


#endif //RPIFIRMWARE_GPIOPOOLING_H
