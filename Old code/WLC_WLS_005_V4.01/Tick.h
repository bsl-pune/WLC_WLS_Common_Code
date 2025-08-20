#ifndef Tick_h
#define Tick_h

class Tclass
{
   public :
   Tclass();
   void Setup();
   void BUZZ_TICK_STOP();
   void BUZZ_TICK_START();
   void BUZZ_INTERVAL(int i);
   void BUZZ_INTERVAL1(int i);
   void BUZZ_INTERVAL2(int i);
   void BUZZ_TICK1_Start();
   void LED_On_Call();
   void LED_Off_Call();
   void TICKER_UPDATE();
};

extern Tclass Tick;
#endif
