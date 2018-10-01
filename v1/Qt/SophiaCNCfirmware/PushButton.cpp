#include "PushButton.h"
#include "hal.h"

PushButton::PushButton(int _GpioPin)
{
    this->GpioPin = _GpioPin;
    this->Init();
}

PushButton::PushButton(int _GpioPin, bool _reversed)
{
    this->GpioPin = _GpioPin;
    this->ReversedPolarity = _reversed;
    this->Init();
}

PushButton::PushButton(int _GpioPin, bool _reversed, bool _debounce)
{
    this->GpioPin = _GpioPin;
    this->ReversedPolarity = _reversed;
    this->Debouncer = _debounce;
    this->Init();
}

PushButton::~PushButton()
{

}

void PushButton::Init()
{
    /* Set pin as input */
    Vfb_SetPinMode(this->GpioPin, INPUT);

    /* Enable interruptions on pin state changed */
    //gpioSetAlertFuncEx(this->GpioPin, (this->gpio_callback), NULL);

    this->ReadState();

    if(this->Debouncer)
        this->timer.restart();
}

bool PushButton::ReadGpio(int reversed)
{
    if(reversed == false)
    {
        return (bool)(button_state_t) Vfb_ReadGpio(GpioPin);
    }
    else
    {
        return (bool)(button_state_t)(!Vfb_ReadGpio(GpioPin) );
    }
}

void PushButton::tick()
{
    if(this->Debouncer)
        this->ReadStateDebounced();
    else
        this->ReadState();
}

void PushButton::gpio_callback(int pin, int level, uint32_t tick, void *opaque)
{
    if(pin != this->GpioPin)
        return;

    if(!Debouncer)
        this->ReadState();
    else
        this->ReadStateDebounced();
}

void PushButton::ReadStateDebounced()
{
    CurrentState = (button_state_t)this->ReadGpio(ReversedPolarity);
    if( CurrentState != PreviousState )
    {
        if(!DebouncingStarted)
        {
            timer.restart();
            DebouncingStarted = true;
        }
        else if(timer.elapsed() >= DEBOUNCING_TIME_MS)
        {
            on_StateChanged(CurrentState);
            DebouncingStarted = false;
        }
    }
    else
    {
        DebouncingStarted = false;
    }
}

button_state_t PushButton::ReadState()
{
    PreviousState = CurrentState;
    CurrentState = (button_state_t)this->ReadGpio(ReversedPolarity);

    if(CurrentState != PreviousState)
    {
        on_StateChanged(CurrentState);
    }
    return CurrentState;
}

void PushButton::on_StateChanged(button_state_t NewState)
{
    if(NewState == PushButtonState::DOWN)
    {
        emit Pressed();
    }
    else
    {
        emit Released();
    }
}
