#include "Ext_var.h"

bool SIGN=0, Proceess_Resume_Enable_Flag;

Rclass::Rclass()
{}

void Rclass:: Setup()
{
  STIR_VALUE_SET     =   EEPROM.read(0);
  DEGREE_VALUE_SET   =   EEPROM.read(1);
  SAFE_VALUE_SET_DEG =   EEPROM.read(2);
  EEPROM.get(3, SAFE_VALUE_SET_FAH);
  CAL_SELECT_VALUE      =  EEPROM.read(5);
  EEPROM.get(70, ERROR_CAL1);
  SIGN =  EEPROM.read(7);
  ERROR_SET_IN_DEG_FLAG = EEPROM.read(8);
  ERROR_SET_IN_FAH_FLAG = EEPROM.read(9);
  EEPROM.get(80, CAL2_RAW_LOW_VALUE_CONV);
  EEPROM.get(90, CAL2_RAW_HIGH_VALUE_CONV);
  EEPROM.get(100, MASTER_LOW_CONV);
  EEPROM.get(110, MASTER_HIGH_CONV);  
  EEPROM.get(65, TIME_SOCK_OR_TOTAL);  
  EEPROM.get(152, processLog);
  EEPROM.get(154, processLogNumberOfTimes);

  Proceess_Resume_Enable_Flag =   EEPROM.read(15);
  select_product_type =           EEPROM.read(150);
  select_heating_type =           EEPROM.read(132);  
  

  SET_LONG_VALUE[Stir_ON_OFF]           = STIR_VALUE_SET;
  SET_LONG_VALUE[Unit_C_F]              = DEGREE_VALUE_SET;
  SET_LONG_VALUE[Safe_Value]            = SAFE_VALUE_SET_DEG;
  SET_LONG_VALUE[Safe_In_Fah]           = SAFE_VALUE_SET_FAH;
  SET_LONG_VALUE[Calibration_Selection] = CAL_SELECT_VALUE;
  SET_LONG_VALUE[Time_Mode]             = TIME_SOCK_OR_TOTAL;
  SET_LONG_VALUE[Auto_Resume_Enable]    = Proceess_Resume_Enable_Flag;
  SET_LONG_VALUE[Slow_Fast_Heating]              = select_heating_type;  
  SET_LONG_VALUE[Product_Selection_Parameter]    = select_product_type;; 
  
  
  EEPROM.get(120, SET_VALUE[22] ); 
  SET_VALUE[1] = EEPROM.read(115);
  SET_VALUE[2] = EEPROM.read(117);
  SET_VALUE[3] = EEPROM.read(125);
  SET_VALUE[4] = EEPROM.read(130);  
  Process_Interrupt_flag =  EEPROM.read(135); 
}

void Rclass ::  Process_Time_Set_On_Eeprom()
{  
  EEPROM.write(135, Process_Interrupt_flag );
}

void Rclass :: Process_Set_Values_Store_On_Eeprom()
{
  EEPROM.write(115, SET_VALUE[1]);
  EEPROM.write(117, SET_VALUE[2]);
  EEPROM.put(120, SET_VALUE[22]); 
  EEPROM.write(125, SET_HOUR );  
  EEPROM.write(130, SET_MINUTE ); 
}

void Rclass :: Save_Pump_Data()
{
  if(!PUMP_FLAG) 
    {
      STIR_VALUE_SET = SET_VALUE[5]; 
      EEPROM.write(0, STIR_VALUE_SET);
    }
}

void Rclass ::EEPROM_CLEAR()
{
  STIR_VALUE_SET = 0;
  DEGREE_VALUE_SET = 0; 
  SAFE_VALUE_SET_DEG = 99;
  SAFE_VALUE_SET_FAH = 212;
  CAL_SELECT_VALUE = 0;
  ERROR_CAL1 = 0;
  SIGN=0;
  ERROR_SET_IN_DEG_FLAG =1; 
  ERROR_SET_IN_FAH_FLAG =0;
  CAL2_RAW_LOW_VALUE_CONV = 30;
  CAL2_RAW_HIGH_VALUE_CONV  = 84;
  MASTER_LOW_CONV  = 28;
  MASTER_HIGH_CONV  = 82;
  TIME_SOCK_OR_TOTAL =0; 
  Proceess_Resume_Enable_Flag=0; 
  select_product_type=0; 
  select_heating_type=0; 
    
  SET_LONG_VALUE[Stir_ON_OFF] = STIR_VALUE_SET;
  SET_LONG_VALUE[Unit_C_F]    = DEGREE_VALUE_SET;
  SET_LONG_VALUE[Safe_Value]  = SAFE_VALUE_SET_DEG;
  SET_LONG_VALUE[Safe_In_Fah] = SAFE_VALUE_SET_FAH;
  SET_LONG_VALUE[Calibration_Selection] = CAL_SELECT_VALUE;
  SET_LONG_VALUE[Time_Mode]             = TIME_SOCK_OR_TOTAL;
  SET_LONG_VALUE[Auto_Resume_Enable]    = Proceess_Resume_Enable_Flag;
  SET_LONG_VALUE[Slow_Fast_Heating]              = select_heating_type;
  SET_LONG_VALUE[Product_Selection_Parameter]    = select_product_type;
 
  SET_HOUR =0;
  SET_MINUTE =0;
  SET_VALUE[1]  = 37;
  SET_VALUE[2]  =0;
  SET_VALUE[22] = 32;
  SET_VALUE[3]  =SET_HOUR;  
  SET_VALUE[4]  =SET_MINUTE;
  Process_Interrupt_flag=0;
  
  EEPROM.write(1, DEGREE_VALUE_SET);
  EEPROM.write(2, SAFE_VALUE_SET_DEG);
  EEPROM.put(3, SAFE_VALUE_SET_FAH);
  EEPROM.write(5, CAL_SELECT_VALUE);  
  EEPROM.write(15, Proceess_Resume_Enable_Flag ) ;
  EEPROM.put(70, ERROR_CAL1);  
  EEPROM.write(7, SIGN);
  EEPROM.write(8, ERROR_SET_IN_DEG_FLAG);
  EEPROM.write(9, ERROR_SET_IN_FAH_FLAG);
  EEPROM.put(80, CAL2_RAW_LOW_VALUE_CONV);
  EEPROM.put(90, CAL2_RAW_HIGH_VALUE_CONV);
  EEPROM.put(100, MASTER_LOW_CONV);
  EEPROM.put(110, MASTER_HIGH_CONV);  
  EEPROM.put(65, TIME_SOCK_OR_TOTAL); 
  
  EEPROM.write(115, SET_VALUE[1]);
  EEPROM.write(117, SET_VALUE[2]);
  EEPROM.put(120, SET_VALUE[22] ); 
  EEPROM.write(125, SET_HOUR );  
  EEPROM.write(130, SET_MINUTE ); 
  EEPROM.write(135, Process_Interrupt_flag );
  EEPROM.write(150, select_product_type );
  EEPROM.write(132, 0 );
  
  SELECTION_PARAMETER = Unit_C_F; 
  Service_Menu=0;
  currentMillis=0; 
  Mode_Button_Single_Press_In_Settings = 0;
  LONG_PRESS=0;
}

Rclass Eeprom = Rclass();
