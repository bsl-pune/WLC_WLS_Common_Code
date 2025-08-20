#include "Arduino.h"
#include "Ext_var.h"
#define PERIOD_EXAMPLE_VALUE  250

Bclass::Bclass()
{}

//********************************************************LONGPRESS***************************************
int  SET_LONG_VALUE[25]={0};
//int  MAX_SET_VALUE_LONG[20]={1, 1, 99, 1, 99, 9, 50, 95, 50, 9, 99, 9, 1, 1, 999, 9999, 999, 1 };
//********************************************************SINGLEPRESS***************************************
int INC=0,TIME_INC=0;
int SET_VALUE[6]={0, 0, 0, 0, 0, 0};
int MAX_SET_VALUE[6]={0, 99, 9, 99, 59, 1};
int SPEED_SW[5]={1, 2, 4, 5, 10};
                           //            a  s  
int Menu_Limit_Lower_Side[]={1, 1, 99, 1, 1, 1, 1, 99, 9, 50, 95, 50, 9, 99, 9,  0,  99, 99, 99, 99, 1}; //<0
//                           0  1   2  3  4  5   6  7   8  9  10  11  12  13  14  15  16  17
int Menu_Limit_Upper_Side[]={0, 0, 0 , 0, 0, 0, 0, 0,   0,  0, 0, 0 , 0,  0, 0,  0,   0,  0, 0 }; //>max limit
int  MAX_SET_VALUE_LONG[21]={1, 1, 99, 1, 1, 1, 1, 99, 9, 50,  95, 50, 9, 99, 9,  1, 1,  99, 99, 99, 1 };

bool COUNTDOWN_START=0;                                                //12=blank in prev
//int  currentMillis=0,  currentMillis1=0;
long previousMillis = 0;  
////**********************************************************************************************************************************

void Bclass:: SETUP()
{  
   cli();
   TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
   TCA0.SINGLE.CTRLB   = TCA_SINGLE_WGMODE_NORMAL_gc;
   TCA0.SINGLE.EVCTRL &= ~(TCA_SINGLE_CNTEI_bm);
   TCA0.SINGLE.PER     = PERIOD_EXAMPLE_VALUE;
   TCA0.SINGLE.CTRLA   = TCA_SINGLE_CLKSEL_DIV1024_gc | TCA_SINGLE_ENABLE_bm;
   sei();
}

void  Bclass:: TIME_CAL()
{
  if(COUNTDOWN_START)
  {
     TIME_INC++;   
     if(TIME_INC == 3760)   
     {
       actualRunMin++;
       SET_MIN--;   TIME_INC=0; 
       if(SET_MIN==0)  COUNTDOWN_START=0; 
     }
 }
}

ISR(TCA0_OVF_vect)
{
    Tick.TICKER_UPDATE();
    i++;
    Butn.TIME_CAL();
    if(Mode_Button_Single_Press_In_Settings && SELECTION_PARAMETER ==  ResetDone)  currentMillis++;
//    currentMillis1++;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;  
}

void Bclass :: SET_CHANGE_UP()
{
  if(LONG_PRESS )
   {
      if(SELECTION_PARAMETER == Safe_Value  && DEGREE_VALUE_SET)     SET_LONG_VALUE[Safe_In_Fah] += SPEED_SW[INC];   if(SET_LONG_VALUE[Safe_In_Fah] > 212) SET_LONG_VALUE[Safe_In_Fah] = 212;  
      else if(SELECTION_PARAMETER == Cal1_Point && DEGREE_VALUE_SET) SET_LONG_VALUE[Cal1_In_Fah] += SPEED_SW[INC];   if(SET_LONG_VALUE[Cal1_In_Fah] > 212) SET_LONG_VALUE[Cal1_In_Fah] = 212;  
      else
      {
         SET_LONG_VALUE[SELECTION_PARAMETER] += SPEED_SW[INC];
         if(SET_LONG_VALUE[SELECTION_PARAMETER] > MAX_SET_VALUE_LONG[SELECTION_PARAMETER])    SET_LONG_VALUE[SELECTION_PARAMETER]= Menu_Limit_Upper_Side[SELECTION_PARAMETER]; 
      }
  }
  else
  {
               if( ENTER_BUT == 1 )   
               {
                 SET_VALUE[ENTER_BUT] += SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] > SAFE_VALUE_SET_DEG) SET_VALUE[ENTER_BUT] = 0; 
               }
                else if( ENTER_BUT == 2 )   
               {
                SET_VALUE[ENTER_BUT] +=SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] > 9) SET_VALUE[ENTER_BUT] = 9; 
                SET_VALUE[22] +=SPEED_SW[INC]; if(SET_VALUE[22] > SAFE_VALUE_SET_FAH) SET_VALUE[22] = 32; 
               }
               else if( ENTER_BUT == 3 )   
               {
                SET_VALUE[ENTER_BUT] +=SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] > MAX_SET_VALUE[ENTER_BUT]) SET_VALUE[ENTER_BUT] = 0; 
               }
               else if( ENTER_BUT == 4 )   
               {
                SET_VALUE[ENTER_BUT] +=SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] > MAX_SET_VALUE[ENTER_BUT]) SET_VALUE[ENTER_BUT] = 0; 
               }  
               if( ENTER_BUT == 5 && PUMP_FLAG && select_product_type)   
               {
                SET_VALUE[ENTER_BUT] +=SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] > MAX_SET_VALUE[ENTER_BUT]) SET_VALUE[ENTER_BUT] = 0; 
               }
  }
}

void Bclass :: SET_CHANGE_DOWN()
{
  if(LONG_PRESS )
  {
      if(SELECTION_PARAMETER == Safe_Value  && DEGREE_VALUE_SET)      SET_LONG_VALUE[Safe_In_Fah] -= SPEED_SW[INC];  if(SET_LONG_VALUE[Safe_In_Fah] < 32) SET_LONG_VALUE[Safe_In_Fah] = 32;   
      else if(SELECTION_PARAMETER == Cal1_Point && DEGREE_VALUE_SET)  SET_LONG_VALUE[Cal1_In_Fah] -= SPEED_SW[INC];  if(SET_LONG_VALUE[Cal1_In_Fah] < 32) SET_LONG_VALUE[Cal1_In_Fah] = 32;   
      else
      {
        SET_LONG_VALUE[SELECTION_PARAMETER] -= SPEED_SW[INC]; 
        if(SET_LONG_VALUE[SELECTION_PARAMETER] < 0)     SET_LONG_VALUE[SELECTION_PARAMETER] = Menu_Limit_Lower_Side[SELECTION_PARAMETER];
      }
  }
  else
  {
    if( ENTER_BUT == 1 )   
    {
       SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = SAFE_VALUE_SET_DEG; 
    }
    else if( ENTER_BUT == 2 )   
    {
      if(!DEGREE_VALUE_SET) SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 9; 
      else                  SET_VALUE[22] -= SPEED_SW[INC]; if(SET_VALUE[22] < 32) SET_VALUE[22] = SAFE_VALUE_SET_FAH; 
    }
    else if( ENTER_BUT == 3 )   
    {
      SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 99; 
    }
    else if( ENTER_BUT == 4 )   
    {
      SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 59; 
    }
    else if( ENTER_BUT == 5 && PUMP_FLAG && select_product_type)   
    {
      SET_VALUE[ENTER_BUT] -= SPEED_SW[INC]; if(SET_VALUE[ENTER_BUT] < 0) SET_VALUE[ENTER_BUT] = 1; 
    }
  }
}

Bclass Butn =Bclass();
