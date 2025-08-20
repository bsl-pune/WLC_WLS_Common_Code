#include "Ext_var.h"

Tclass::Tclass()
{}

void STOP_BUZZER();
Ticker BUZZ(STOP_BUZZER, 100, 1);

void STOP_BUZZER1();
Ticker BUZZ_TICK1(STOP_BUZZER1, 2000, 0);

void STOP_BUZZER2();
Ticker BUZZ_TICK2(STOP_BUZZER2, 1000, 1);

//void LED_on();
//Ticker Blink_LED_ON(LED_on,1000,0);
//
//void LED_off();
//Ticker Blink_LED_OFF(LED_off,500,0);

void LED_on();
Ticker Blink_LED_ON(LED_on,300,0);

void LED_off();
Ticker Blink_LED_OFF(LED_off,600,0);

void Tclass:: Setup()
{ 
  
}

void Tclass :: TICKER_UPDATE()
{
  BUZZ.update();
  Pid.PID_Tick_Update();
  Temp.Rtd_Temp_Update();
  BUZZ_TICK1.update();
  BUZZ_TICK2.update();  
  Blink_LED_ON.update();  
  Blink_LED_OFF.update();  
}

void Tclass :: LED_On_Call()
{
//  Blink_LED_ON.start(); 
  Blink_LED_OFF.start(); 
}

void Tclass :: LED_Off_Call()
{
  Blink_LED_ON.stop();   
  Blink_LED_OFF.stop();
}

void LED_on()
{  
  digitalWrite(ALARM_LED, HIGH); 
//  Blink_LED_OFF.start();
 Blinking_Freq++;  
}

void LED_off()
{
  digitalWrite(ALARM_LED, LOW);  
  Blink_LED_ON.start(); 
//  Blinking_Freq++;  
}

void Tclass :: BUZZ_INTERVAL(int i)
{
  BUZZ.interval(i);
  digitalWrite(BUZZER, HIGH);
  BUZZ.start();
}

void STOP_BUZZER2()
{
  digitalWrite(BUZZER, LOW);  
   if(REPETATION_FLAG) FOR_REP++;
   FOR_REP2++;
}

void Tclass :: BUZZ_TICK_STOP()
{
  BUZZ_TICK1.stop();
}

void Tclass :: BUZZ_TICK_START()
{
  BUZZ_TICK1.start();
}

void STOP_BUZZER1()
{
  BUZZ_TICK2.start();
  digitalWrite(BUZZER, HIGH);  
}

void Tclass :: BUZZ_INTERVAL1(int i)
{
  BUZZ_TICK1.interval(i);
}

void Tclass :: BUZZ_INTERVAL2(int i)
{
  BUZZ_TICK2.interval(i);
}

void Tclass :: BUZZ_TICK1_Start()
{
  BUZZ_TICK1.start(); 
}

void STOP_BUZZER()
{
  BUZZ.stop();
  digitalWrite(BUZZER, LOW);  
}

Tclass Tick = Tclass();
