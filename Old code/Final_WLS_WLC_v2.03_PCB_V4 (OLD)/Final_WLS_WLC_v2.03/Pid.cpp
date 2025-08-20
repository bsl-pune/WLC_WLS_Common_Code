#include "Ext_var.h"

void PID_RUN();
Ticker PID_EXE(PID_RUN, 400, 0);

//------------------------------------------------------PID---------------------------------------------------------------//
double PID_SETPOINT, PID_INPUT, PID_OUTPUT;
double Kp=150, Ki=0, Kd=0, Ki_saved;
double consKp = 150, consKi = 0.5, consKd = 400;     // for internal probe values p 200
//------------------------------------------------------PID---------------------------------------------------------------//
int WINDOW_SIZE = 1000;
unsigned long WINDOW_START_TIME;
PID myPID(&PID_INPUT, &PID_OUTPUT, &PID_SETPOINT, Kp, Ki, Kd, DIRECT);
//------------------------------------------------------------------------------------------------------------------------//

Lclass::Lclass()
{}

void Lclass:: Setup()
{
  myPID.SetOutputLimits(0, WINDOW_SIZE);
  WINDOW_START_TIME = millis();
  myPID.SetMode(AUTOMATIC);
}

void PID_RUN()
{
   myPID.Compute();  
}

void Lclass ::PID_callingg()
{
  if(!LONG_PRESS)    PID_INPUT = TEMP_DEG;
  else               PID_INPUT = TEMP_DEG_RAW;

  if(PID_SETPOINT < 45)
  {
        Kp= 30; Ki=0.020; Kd=550;  //14-02-2023
    
         myPID.SetTunings(Kp, Ki, Kd);    
  }
  else
  {
    if((PID_SETPOINT - PID_INPUT) > 5)   
    {
       Kp=150, Ki=0, Kd=0,
       myPID.SetTunings(Kp, Ki, Kd);
    }                    
    else
     myPID.SetTunings(consKp, consKi, consKd); 
  }   
  if (millis() - WINDOW_START_TIME > WINDOW_SIZE)
          WINDOW_START_TIME += WINDOW_SIZE;
 
  if (PID_OUTPUT < millis() - WINDOW_START_TIME)      digitalWrite(HEATER, LOW);
  else 
  {
    if(PID_INPUT >= (float(PID_SETPOINT))) /* + 0.1)) */    digitalWrite(HEATER, LOW);
    else                                                    digitalWrite(HEATER, HIGH);
  }
}

void Lclass :: PID_Start()
{
   PID_EXE.start(); 
}

void Lclass :: PID_Stop()
{
   PID_EXE.stop(); 
}

void Lclass :: PID_Tick_Update()
{
  PID_EXE.update();
}

Lclass Pid = Lclass();
