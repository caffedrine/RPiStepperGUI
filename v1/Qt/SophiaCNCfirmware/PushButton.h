#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "hal.h"
#include <QObject>
#include <QTime>

#define DEBOUNCING_TIME_MS  10

typedef enum PushButtonState
{
    DOWN = 1,
    UP = 0
}button_state_t;

class PushButton : public QObject
{
    Q_OBJECT
public:
    int GpioPin;
    bool ReversedPolarity = false;
    bool Debouncer = false;
    button_state_t CurrentState, PreviousState;
    QTime timer;

    PushButton(int GpioPin);
    PushButton(int GpioPin, bool reversed);
    PushButton(int GpioPin, bool reversed, bool debouncer);
    ~PushButton();
    void Init();

    void tick();
    button_state_t ReadState();
    bool ReadGpio(int reversed = false);
    void ReadStateDebounced();

    void gpio_callback(int pin, int level, uint32_t tick, void *opaque);

private:
    bool DebouncingStarted = false;
    void on_StateChanged(button_state_t NewState);

signals:
    void Pressed();
    void Released();
};

#endif // PUSHBUTTON_H
