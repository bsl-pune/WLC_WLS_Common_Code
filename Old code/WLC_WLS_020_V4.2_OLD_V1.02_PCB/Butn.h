#ifndef Butn_h
#define Butn_h

#define HEATER_STATUS_LED 27
#define CURRENT_PARA_LED  28
#define SET_PARA_LED      29
#define ALARM_LED         26
#define BUZZER            21
#define MODE_KEY          35   
#define UP_KEY            37  
#define DOWN_KEY          36
#define RST_KEY           34  
#define HEATER            33 //33saftey 
#define PUMP              25
#define TIMER_LED1        12
#define TIMER_LED2        13
#define safteyRelay       24 //24 safteyRelay //ssr

class Bclass
{
   public :
   Bclass();
   void SETUP();
   void TIME_CAL();  
   void SET_CHANGE_UP();
   void SET_CHANGE_DOWN();
   void Button_Tick_Update();
};
extern Bclass Butn;

#endif
