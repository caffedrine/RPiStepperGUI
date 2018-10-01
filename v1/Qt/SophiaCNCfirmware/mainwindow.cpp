#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QPixmap>
#include <QtConcurrent/QtConcurrent>
#include <QTouchEvent>

#include "hal.h"

#define MAX_UP_MM                       3000
#define DCMOTOR_SPEED                   70

#define CUTTER_MOTOR_ON                 Vfb_WriteGpio(CUTTER_MOTOR_PIN, HIGH)
#define CUTTER_MOTOR_OFF                Vfb_WriteGpio(CUTTER_MOTOR_PIN, LOW)

#define ELECTROVALVES_ON                Vfb_WriteGpio(ELECTROVALVES_PIN, HIGH)
#define ELECTROVALVES_OFF               Vfb_WriteGpio(ELECTROVALVES_PIN, LOW)

#define SHIELD_ON                       Vfb_WriteGpio(SHIELD_ENABLE_PIN, HIGH);
#define SHIELD_OFF                      Vfb_WriteGpio(SHIELD_ENABLE_PIN, LOW);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Init GPIO driver */
    Vfb_GpioInitialise();

    /* Init GPIO Pins */
    Vfb_SetPinMode(VERTICAL_MOTOR_PULSE_PIN, OUTPUT);
    Vfb_SetPinMode(VERTICAL_MOTOR_DIRECTION_PIN, OUTPUT);
    Vfb_SetPinMode(HORIZONTAL_MOTOR_ENABLE_PIN, OUTPUT);
    Vfb_SetPinMode(HORIZONTAL_MOTOR_DIRECTION_PIN, OUTPUT);
    Vfb_SetPinMode(CUTTER_MOTOR_PIN, OUTPUT);
    Vfb_SetPinMode(SHIELD_ENABLE_PIN, OUTPUT);
     /* Buttons */
    Vfb_SetPinMode(VERTICAL_MOTOR_INIT_BUTTON_PIN, INPUT);
    Vfb_SetPinMode(HORIZONTAL_MOTOR_INIT_BUTTON_PIN, INPUT);
    Vfb_SetPinMode(CROP_FINISHED_SENSOR_PIN, INPUT);

    /* Write 0 on all pins by default */
    CUTTER_MOTOR_OFF;
    ELECTROVALVES_OFF;
    Vfb_WriteGpio(VERTICAL_MOTOR_DIRECTION_PIN, LOW);
    Vfb_WriteGpio(HORIZONTAL_MOTOR_ENABLE_PIN, LOW);
    Vfb_WriteGpio(HORIZONTAL_MOTOR_DIRECTION_PIN, LOW);
    Vfb_WriteGpio(CUTTER_MOTOR_PIN, LOW);

    /* Disable mouse while inside app */
    QApplication::setOverrideCursor(Qt::BlankCursor);

    QMainWindow::showFullScreen();

    ui->groupBox_Manual->hide();
    ui->groupBox_Automatic->show();
    ui->pushButton_AutomaticManual->setText("MANUAL");

    /* Initialize pi-blaster driver */
    //    QProcess *process = new QProcess(this);
    //    process->start("bash", QStringList() << "-c" << "sudo /usr/sbin/pi-blaster --gpio 4,21");
    //    process->waitForFinished();

    /* Setting up buttons */
    HorizontalMotorInitButton = new PushButton(HORIZONTAL_MOTOR_INIT_BUTTON_PIN, true);
    connect(HorizontalMotorInitButton, SIGNAL( Pressed()), this, SLOT( on_horizontalMotorInitSwitch_Pressed()));
    connect(HorizontalMotorInitButton, SIGNAL( Released()), this, SLOT( on_horizontalMotorInitSwitch_Released()));

    VerticalMotorInitButton = new PushButton(VERTICAL_MOTOR_INIT_BUTTON_PIN, true);
    connect(VerticalMotorInitButton, SIGNAL( Pressed()), this, SLOT( on_verticalMotorInitSwitch_Pressed()));
    connect(VerticalMotorInitButton, SIGNAL( Released()), this, SLOT( on_verticalMotorInitSwitch_Released()));

    CropFinishButton = new PushButton(CROP_FINISHED_SENSOR_PIN);
    connect(CropFinishButton, SIGNAL( Pressed()), this, SLOT( on_cropFinishedSensor_Pressed()));
    connect(CropFinishButton, SIGNAL( Released()), this, SLOT( on_cropFinishedSensor_Released()));

    /* Launch ticking thread */
    TickThread = new TickingThread();
    connect(TickThread, SIGNAL( Tick() ), this, SLOT( TickTask() ) );
    TickThread->start();

    /* Launching states machine thread */
    StatesMachineThread = new MainWorkerThread();
    connect(StatesMachineThread, SIGNAL( Tick() ), this, SLOT( StatesMachine()) );
    StatesMachineThread->start();

    /* Initialize stepper motor */
    stepper = new Stepper(VERTICAL_MOTOR_DIRECTION_PIN, VERTICAL_MOTOR_PULSE_PIN);
    connect(stepper, SIGNAL( PositionChanged(float) ), this, SLOT(on_stepperPositionUpdated(float)) );

    /* Initialize DC motor */
    dc_motor = new DCMotor(HORIZONTAL_MOTOR_ENABLE_PIN, HORIZONTAL_MOTOR_DIRECTION_PIN);    

    StatesDescrpiption[EMERGENCY_STOP_REQUEST] = "Emergency stop requested!";
    StatesDescrpiption[STANDBY] =  "Standby, waiting for start";
    StatesDescrpiption[INIT_MOTORS] =  "Initializing motors requested";
    StatesDescrpiption[WAIT_INIT] =  "Wait motors initializing";
    StatesDescrpiption[MOVE_STEPPER] = "Start button pressed";
    StatesDescrpiption[WAIT_MOVE_STEPPER] = "Wait move stepper";
    StatesDescrpiption[WAIT_SECOND_START] = "Waiting for start cutter";
    StatesDescrpiption[BLOCK_CURTAIN] = "Block electrovalves";
    StatesDescrpiption[WAIT_BLOCK_CURTAIN] = "Wait block electrovalves";
    StatesDescrpiption[TENSION_CURTAIN] = "Tension curtain";
    StatesDescrpiption[WAIT_TENSION_CURTAIN] = "Wait tension curtain";
    StatesDescrpiption[START_CUTTER_MOTOR] = "Start cutter motor";
    StatesDescrpiption[WAIT_START_CUTTER_MOTOR] = "Waiting forcutter to start";
    StatesDescrpiption[CUT_CURTAIN] = "Cut curtain start";
    StatesDescrpiption[WAIT_CUT_CURTAIN] = "Waiting for motor to cut the curtain";
    StatesDescrpiption[STOP_CUTTER_MOTOR] = "Stop cutter";
    StatesDescrpiption[WAIT_STOP_CUTTER_MOTOR] = "Waiting for cutter to stop";
    StatesDescrpiption[MOTORS_RETURN] = "Cutter return";
    StatesDescrpiption[WAIT_MOTOR_RETURN] = "Waiting cutter return";

    /* Setting up initial state */
    SetState(STANDBY);

    /* Enable shield */
    SHIELD_ON;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StatesMachine()
{

    /** GENERAL */
    if(CurrentState == States::EMERGENCY_STOP_REQUEST)
    {
        dc_motor->Stop();
        CUTTER_MOTOR_OFF;
        ELECTROVALVES_OFF;
        stepper->Stop();
        SetState(States::STANDBY);
        return;
    }

    if(CurrentState == States::STANDBY)
    {
        return;
    }

    if(CurrentState == States::INIT_MOTORS)
    {
        /* Initialize vertical motor */
        if( VerticalMotorInitButton->CurrentState == PushButtonState::DOWN )
        {
            stepper->Stop();
        }
        else
        {
            stepper->SetDirection(MotorDirection::BACKWARD);
            stepper->Run();
        }

        /* Initialize horizontal motor */
        if(HorizontalMotorInitButton->CurrentState == PushButtonState::DOWN)
        {
            dc_motor->Stop();
        }
        else
        {
            dc_motor->SetDirection(DCMotorDirection::BACKWARD);
            dc_motor->Run(DCMOTOR_SPEED);
        }
        SetState(States::WAIT_INIT);
    }

    if(CurrentState == States::WAIT_INIT)
    {
        /* Initialize vertical motor */
        if( VerticalMotorInitButton->CurrentState == PushButtonState::DOWN )
        {
            stepper->Stop();
        }

        /* Initialize horizontal motor */
        if(HorizontalMotorInitButton->CurrentState == PushButtonState::DOWN)
        {
            dc_motor->Stop();
        }

        /* If both motors were initialized go to next state */
        if(VerticalMotorInitButton->CurrentState == PushButtonState::DOWN &&
                HorizontalMotorInitButton->CurrentState == PushButtonState::DOWN)
        {
            CurrentPosition = 0;
            SetState(States::STANDBY);
            stepper->ResetPosition();

            SystemInitialized = true;
        }
    }


    /** Sequencial states **/

    if(CurrentState == States::MOVE_STEPPER)
    {
        int TargetPosition = get_digits_value();
        int MillisToDo = 0;

        if( TargetPosition < STEPPER_POSITION_OFFSET || TargetPosition > MAX_UP_MM )
        {
            qDebug() << "LENGTH OUT OF RANGE!";
            SetState(EMERGENCY_STOP_REQUEST);
            return;
        }

        if( TargetPosition > CurrentPosition)
        {
            stepper->SetDirection(FORWARD);
            MillisToDo = TargetPosition - CurrentPosition;

        }
        else if(TargetPosition < CurrentPosition)
        {
            stepper->SetDirection(BACKWARD);
            MillisToDo = CurrentPosition - TargetPosition;
        }
        else
        {
            MillisToDo = 0;
        }

        stepper->RunMillimeters( (float)MillisToDo );
        SetState(States::WAIT_MOVE_STEPPER);
    }

    if(CurrentState == States::WAIT_MOVE_STEPPER)
    {
        if(stepper->StepsToDo == 0)
        {
            ui->pushButton_start->show();
            SetState(States::WAIT_SECOND_START);
            ui->pushButton_start->show();
        }
    }

    if(CurrentState == States::WAIT_SECOND_START)
    {
        /* This state requires user to press start button again */
        return;
    }

    if(CurrentState == States::BLOCK_CURTAIN)
    {
        ELECTROVALVES_ON;
        WaitTimer.restart();
        SetState(States::WAIT_BLOCK_CURTAIN);
    }

    if(CurrentState == States::WAIT_BLOCK_CURTAIN)
    {
        if(WaitTimer.elapsed() >= 1000 )    /* 1s */
        {
            //SetState(States::TENSION_CURTAIN);
            SetState(States::START_CUTTER_MOTOR);
        }
    }

//    if(CurrentState == States::TENSION_CURTAIN)
//    {
//        stepper->SetDirection(FORWARD);
//        stepper->RunMillimeters(10);
//        WaitTimer.restart();
//        SetState(States::WAIT_TENSION_CURTAIN);
//    }

//    if(CurrentState == States::WAIT_TENSION_CURTAIN)
//    {
//        if(WaitTimer.elapsed() >= 1000)
//        {
//            SetState(States::START_CUTTER_MOTOR);
//        }
//    }

    if(CurrentState == States::START_CUTTER_MOTOR)
    {
        CUTTER_MOTOR_ON;
        WaitTimer.restart();
        SetState(States::WAIT_START_CUTTER_MOTOR);
    }

    if(CurrentState == States:: WAIT_START_CUTTER_MOTOR)
    {
        if(WaitTimer.elapsed() >= 1000)
        {
            SetState(States::CUT_CURTAIN);
        }
    }

    if(CurrentState == States::CUT_CURTAIN)
    {
        dc_motor->SetDirection(DCMotorDirection::FORWARD);
        dc_motor->Run(DCMOTOR_SPEED);

        SetState(States::WAIT_CUT_CURTAIN);

        /* Timer used to counti time after start - infrared sensor data is correct only after first second */
        WaitTimer.restart();
    }

    if(CurrentState == States::WAIT_CUT_CURTAIN)
    {
        if(WaitTimer.elapsed() >= 1000)
        {
            if( CropFinishButton->CurrentState == PushButtonState::UP )
                SetState(States::STOP_CUTTER_MOTOR);
        }
    }

    if(CurrentState == States::STOP_CUTTER_MOTOR)
    {
        CUTTER_MOTOR_OFF;
        WaitTimer.restart();
        SetState(WAIT_STOP_CUTTER_MOTOR);
    }

    if(CurrentState == States::WAIT_STOP_CUTTER_MOTOR )
    {
        if(WaitTimer.elapsed() >= 100)
        {
            dc_motor->Stop();
            SetState(States::MOTORS_RETURN);
        }
    }

    if(CurrentState == States::MOTORS_RETURN)
    {
        dc_motor->SetDirection(DCMotorDirection::BACKWARD);
        dc_motor->Run(DCMOTOR_SPEED);
        SetState(WAIT_MOTOR_RETURN);
    }

    if(CurrentState == States::WAIT_MOTOR_RETURN)
    {
        if (HorizontalMotorInitButton->CurrentState == PushButtonState::DOWN)
        {            
            ui->pushButton_start->show();
            ui->pushButton_left->setEnabled(true);
            ui->pushButton_right->setEnabled(true);
            ui->pushButton_up->setEnabled(true);
            ui->pushButton_down->setEnabled(true);
            ui->pushButton_stop->setStyleSheet("background-color: red");

            dc_motor->Stop();
            ELECTROVALVES_OFF;
            this->ActionStarted = false;
            SetState(States::STANDBY);
        }
    }
}

void MainWindow::TickTask()
{
        HorizontalMotorInitButton->tick();
        VerticalMotorInitButton->tick();
        CropFinishButton->tick();
}

void MainWindow::on_stepperPositionUpdated(float new_position)
{
    this->CurrentPosition = new_position;
    ui->positionLabel->setText( "Position: " + QString::number(this->CurrentPosition)  + " mm");
}

void MainWindow::SetState(states_t new_state)
{
    //QString dbgStr = "(" + QString::number(CurrentState) + " -> " + QString::number(state) + "): ";
    QString dbgStr = this->StatesDescrpiption[new_state];

    qDebug() << dbgStr;
    ui->statusLabel->setText(dbgStr);

    PreviousState = CurrentState;
    CurrentState = new_state;
}

int MainWindow::get_digits_value()
{
   int val = 0;
   val += ui->pushButton_value3->text().toInt() * 1000;
   val += ui->pushButton_value2->text().toInt() * 100;
   val += ui->pushButton_value1->text().toInt() * 10;
   val += ui->pushButton_value0->text().toInt();
   return val;
}

void MainWindow::set_digits_value(int new_val)
{
   ui->pushButton_value3->setText( QString::number(new_val/1000) );
   new_val = new_val%1000;
   ui->pushButton_value2->setText(QString::number( new_val/100 ));
   new_val = new_val%100;
   ui->pushButton_value1->setText(QString::number( new_val/10 ));
   new_val = new_val%10;
   ui->pushButton_value0->setText(QString::number( new_val ));
}

void MainWindow::update_digits(int value)
{
    if(ui->pushButton_value0->isChecked())
    {
        ui->pushButton_value0->setChecked(false);
        ui->pushButton_value0->setText(  QString::number(value)  );
    }
    else if(ui->pushButton_value1->isChecked())
    {
        ui->pushButton_value1->setChecked(false);
        ui->pushButton_value1->setText(  QString::number(value)  );
    }
    else if(ui->pushButton_value2->isChecked())
    {
        ui->pushButton_value2->setChecked(false);
        ui->pushButton_value2->setText(  QString::number(value)  );
    }
    else if(ui->pushButton_value3->isChecked())
    {
        ui->pushButton_value3->setChecked(false);
        ui->pushButton_value3->setText(  QString::number(value)  );
    }
}

//     _  _________   ______   ___    _    ____  ____     ____ ___ ____ ___ _____ ____
//    | |/ / ____\ \ / / __ ) / _ \  / \  |  _ \|  _ \   |  _ \_ _/ ___|_ _|_   _/ ___|
//    | ' /|  _|  \ V /|  _ \| | | |/ _ \ | |_) | | | |  | | | | | |  _ | |  | | \___ \
//    | . \| |___  | | | |_) | |_| / ___ \|  _ <| |_| |  | |_| | | |_| || |  | |  ___) |
//    |_|\_\_____| |_| |____/ \___/_/   \_\_| \_\____/   |____/___\____|___| |_| |____/


void MainWindow::on_pushButton_digit1_pressed()
{
    ui->pushButton_digit1->setStyleSheet("background-color: red");
    update_digits(1);
}

void MainWindow::on_pushButton_digit2_pressed()
{
    ui->pushButton_digit2->setStyleSheet("background-color: red");
    update_digits(2);
}

void MainWindow::on_pushButton_digit3_pressed()
{
    ui->pushButton_digit3->setStyleSheet("background-color: red");
    update_digits(3);
}

void MainWindow::on_pushButton_digit4_pressed()
{
    ui->pushButton_digit4->setStyleSheet("background-color: red");
    update_digits(4);
}

void MainWindow::on_pushButton_digit5_pressed()
{
    ui->pushButton_digit5->setStyleSheet("background-color: red");
    update_digits(5);
}

void MainWindow::on_pushButton_digit6_pressed()
{
    ui->pushButton_digit6->setStyleSheet("background-color: red");
    update_digits(6);
}

void MainWindow::on_pushButton_digit7_pressed()
{
    ui->pushButton_digit7->setStyleSheet("background-color: red");
    update_digits(7);
}

void MainWindow::on_pushButton_digit8_pressed()
{
    ui->pushButton_digit8->setStyleSheet("background-color: red");
    update_digits(8);
}

void MainWindow::on_pushButton_digit9_pressed()
{
    ui->pushButton_digit9->setStyleSheet("background-color: red");
    update_digits(9);
}

void MainWindow::on_pushButton_digit0_pressed()
{
    ui->pushButton_digit0->setStyleSheet("background-color: red");
    update_digits(0);
}

void MainWindow::on_pushButton_digit1_released()
{
    ui->pushButton_digit1->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit2_released()
{
    ui->pushButton_digit2->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit3_released()
{
    ui->pushButton_digit3->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit4_released()
{
    ui->pushButton_digit4->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit5_released()
{
    ui->pushButton_digit5->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit6_released()
{
    ui->pushButton_digit6->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit7_released()
{
    ui->pushButton_digit7->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit8_released()
{
    ui->pushButton_digit8->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit9_released()
{
    ui->pushButton_digit9->setStyleSheet("background-color: rgb(114, 159, 207)");
}

void MainWindow::on_pushButton_digit0_released()
{
    ui->pushButton_digit0->setStyleSheet("background-color: rgb(114, 159, 207)");
}

//     _   _ ____       __  ____   _____        ___   _
//    | | | |  _ \     / / |  _ \ / _ \ \      / / \ | |
//    | | | | |_) |   / /  | | | | | | \ \ /\ / /|  \| |
//    | |_| |  __/   / /   | |_| | |_| |\ V  V / | |\  |
//     \___/|_|     /_/    |____/ \___/  \_/\_/  |_| \_|
//
void MainWindow::on_pushButton_up_pressed()
{
    ui->pushButton_up->setStyleSheet("background-color: red");
    stepper->SetDirection(MotorDirection::FORWARD);
    stepper->Run();
}

void MainWindow::on_pushButton_up_released()
{
    ui->pushButton_up->setStyleSheet("background-color: rgb(114, 159, 207)");
    stepper->Stop();
    set_digits_value( get_digits_value() + stepper->GetLastDistance() );
}

void MainWindow::on_pushButton_down_pressed()
{
    ui->pushButton_down->setStyleSheet("background-color: red");
    stepper->SetDirection(MotorDirection::BACKWARD);
    stepper->Run();
}

void MainWindow::on_pushButton_down_released()
{
    ui->pushButton_down->setStyleSheet("background-color: rgb(114, 159, 207)");
    stepper->Stop();
    set_digits_value( get_digits_value() - stepper->GetLastDistance() );
}

//     ____  ___ ____ _   _ _____      __  _     _____ _____ _____
//    |  _ \|_ _/ ___| | | |_   _|    / / | |   | ____|  ___|_   _|
//    | |_) || | |  _| |_| | | |     / /  | |   |  _| | |_    | |
//    |  _ < | | |_| |  _  | | |    / /   | |___| |___|  _|   | |
//    |_| \_\___\____|_| |_| |_|   /_/    |_____|_____|_|     |_|
//

void MainWindow::on_pushButton_left_pressed()
{
    ui->pushButton_left->setStyleSheet("background-color: red");
    dc_motor->SetDirection(DCMotorDirection::BACKWARD);
    dc_motor->Run(DCMOTOR_SPEED);
}

void MainWindow::on_pushButton_left_released()
{
    ui->pushButton_left->setStyleSheet("background-color: rgb(114, 159, 207)");
    dc_motor->Stop();
}

void MainWindow::on_pushButton_right_pressed()
{
    ui->pushButton_right->setStyleSheet("background-color: red");
    dc_motor->SetDirection(DCMotorDirection::FORWARD);
    dc_motor->Run(DCMOTOR_SPEED);
}

void MainWindow::on_pushButton_right_released()
{
    ui->pushButton_right->setStyleSheet("background-color: rgb(114, 159, 207)");
    dc_motor->Stop();
}

//     ____ _____  _    ____ _____      __  ____ _____ ___  ____
//    / ___|_   _|/ \  |  _ \_   _|    / / / ___|_   _/ _ \|  _ \
//    \___ \ | | / _ \ | |_) || |     / /  \___ \ | || | | | |_) |
//     ___) || |/ ___ \|  _ < | |    / /    ___) || || |_| |  __/
//    |____/ |_/_/   \_\_| \_\|_|   /_/    |____/ |_| \___/|_|
//
void MainWindow::on_pushButton_start_pressed()
{
    /* Start shall only work when motors were calibrated */
    if(!SystemInitialized)
    {
        for(int i = 0; i < 5; i++)
        {
            ui->pushButton_reinitialize->setStyleSheet("background-color: blue");
            qApp->processEvents();
            QThread::msleep ( 100 );
            ui->pushButton_reinitialize->setStyleSheet("background-color: red");
            qApp->processEvents();
            QThread::msleep ( 100 );
        }
        return;
    }

    /* If already started */
    if(this->ActionStarted == true  && StartPhase != 2)
        return;

    if(StartPhase == 1)
    {
        ui->pushButton_start->hide();
        ui->pushButton_left->setEnabled(false);
        ui->pushButton_right->setEnabled(false);
        ui->pushButton_up->setEnabled(false);
        ui->pushButton_down->setEnabled(false);
        //ui->pushButton_start->setStyleSheet("background-color: red");

        SetState(States::MOVE_STEPPER);
        this->ActionStarted = true;
        stepper->SetDirection(FORWARD);
        stepper->RunMillimeters(get_digits_value());
        //stepper->RunSteps( get_digits_value() );
        this->StartPhase = 2;
    }
    else if(StartPhase == 2)
    {
        ui->pushButton_start->hide();
        this->SetState(States::BLOCK_CURTAIN);
        StartPhase = 1;
    }
}

void MainWindow::on_pushButton_start_released()
{
    ui->pushButton_start->setStyleSheet("background-color: green");
}

void MainWindow::on_pushButton_stop_pressed()
{
        ui->pushButton_start->show();
        ui->pushButton_left->setEnabled(true);
        ui->pushButton_right->setEnabled(true);
        ui->pushButton_up->setEnabled(true);
        ui->pushButton_down->setEnabled(true);
        ui->pushButton_stop->setStyleSheet("background-color: blue");

        this->StartPhase = 1;
        SetState(States::EMERGENCY_STOP_REQUEST);
        this->ActionStarted = false;
}

void MainWindow::on_pushButton_stop_released()
{
    ui->pushButton_stop->setStyleSheet("background-color: red");
}

void MainWindow::on_pushButton_value3_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_value3->setStyleSheet("background-color: red");

        ui->pushButton_value0->setChecked(false);
        ui->pushButton_value1->setChecked(false);
        ui->pushButton_value2->setChecked(false);
    }
    else
    {
        ui->pushButton_value3->setStyleSheet("background-color: rgb(114, 159, 207)");
    }
}

void MainWindow::on_pushButton_value2_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_value2->setStyleSheet("background-color: red");

        ui->pushButton_value0->setChecked(false);
        ui->pushButton_value1->setChecked(false);
        ui->pushButton_value3->setChecked(false);
    }
    else
    {
        ui->pushButton_value2->setStyleSheet("background-color: rgb(114, 159, 207)");
    }
}

void MainWindow::on_pushButton_value1_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_value1->setStyleSheet("background-color: red");

        ui->pushButton_value0->setChecked(false);
        ui->pushButton_value2->setChecked(false);
        ui->pushButton_value3->setChecked(false);
    }
    else
    {
        ui->pushButton_value1->setStyleSheet("background-color: rgb(114, 159, 207)");
    }
}

void MainWindow::on_pushButton_value0_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_value0->setStyleSheet("background-color: red");

        ui->pushButton_value1->setChecked(false);
        ui->pushButton_value2->setChecked(false);
        ui->pushButton_value3->setChecked(false);
    }
    else
    {
        ui->pushButton_value0->setStyleSheet("background-color: rgb(114, 159, 207)");
    }
}

 void MainWindow::on_verticalMotorInitSwitch_Pressed()
 {
    ui->pushButton_v->setStyleSheet("background-color: red");
 }

 void MainWindow::on_verticalMotorInitSwitch_Released()
 {
    ui->pushButton_v->setStyleSheet("background-color: green");
 }

 void MainWindow::on_horizontalMotorInitSwitch_Pressed()
 {
    ui->pushButton_H->setStyleSheet("background-color: red");
 }

 void MainWindow::on_horizontalMotorInitSwitch_Released()
 {
    ui->pushButton_H->setStyleSheet("background-color: green");
 }

 void MainWindow::on_cropFinishedSensor_Pressed()
 {
    ui->pushButton_L->setStyleSheet("background-color: red");
 }

 void MainWindow::on_cropFinishedSensor_Released()
 {
    ui->pushButton_L->setStyleSheet("background-color: green");
 }


void MainWindow::on_pushButton_reinitialize_pressed()
{
    ELECTROVALVES_OFF;
    CUTTER_MOTOR_OFF;
    this->SetState(States::INIT_MOTORS);
}

void MainWindow::on_pushButton_reinitialize_released()
{

}

void MainWindow::on_pushButton_AutomaticManual_pressed()
{
    if(ui->groupBox_Automatic->isVisible() == true)
    {
        ui->groupBox_Automatic->hide();
        ui->groupBox_Manual->show();
        ui->pushButton_AutomaticManual->setText("AUTO");
    }
    else
    {
        ui->groupBox_Automatic->show();
        ui->groupBox_Manual->hide();
        ui->pushButton_AutomaticManual->setText("MANUAL");
    }
}


void MainWindow::on_pushButton_Cutter_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_Cutter->setStyleSheet("background-color: red");
        ui->pushButton_Cutter->setText("CUTTER ON");
        CUTTER_MOTOR_ON;
    }
    else
    {
        ui->pushButton_Cutter->setStyleSheet("background-color: green");
        ui->pushButton_Cutter->setText("CUTTER OFF");
        CUTTER_MOTOR_OFF;
    }
}

void MainWindow::on_pushButton_Valves_toggled(bool checked)
{
    if(checked)
    {
        ui->pushButton_Valves->setStyleSheet("background-color: red");
        ui->pushButton_Valves->setText("VALVES ON");
        ELECTROVALVES_ON;
    }
    else
    {
        ui->pushButton_Valves->setStyleSheet("background-color: green");
        ui->pushButton_Valves->setText("VALVES OFF");
        ELECTROVALVES_OFF;
    }
}
