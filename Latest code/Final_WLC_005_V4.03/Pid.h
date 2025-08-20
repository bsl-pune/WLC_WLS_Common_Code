#ifndef Pid_h
#define Pid_h

class Lclass
{
   public :
   Lclass();
   void Setup();
   void PID_Start();
   void PID_Stop();
   void PID_Tick_Update();
   void PID_callingg();
};

extern Lclass Pid;
#endif
