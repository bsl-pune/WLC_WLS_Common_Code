#ifndef Procss_h
#define Procss_h

class Pclass
{
   public :
   Pclass();
   void Setup();
   void ALL_LED_OFF();
   void SET_LONG_PRESS_FUNCTION();
   void SET_SHORT_PRESS_FUNCTION();
   void toggle_ticker_update();
};

enum 
{
  Stir_ON_OFF, 
  Unit_C_F,      
  Safe_Value,    
  Time_Mode,   
  Auto_Resume_Enable,   
  Product_Selection_Parameter,  
  Calibration_Selection,
  Cal1_Decimal,
  Cal1_Point,    
  Cal2LowTemp_Set_Display,
  Cal2HighTemp_Set_Display ,
  Cal2LowDecimalTemp_Enter_Heat, 
  Cal2LowPointTemp_Enter_Maintain,   
  Cal2HighDecimalTemp_Enter_Heat,
  Cal2HighPointTemp_Enter_Maintain,
  Cal2_Complete_Display,
  Slow_Fast_Heating,  
  Log,
  FactoryAndSure_Reset,
  ResetDone,
  Safe_In_Fah ,   
  Cal1_In_Fah    
}LongPressSelection;

extern Pclass Procss;
#endif
