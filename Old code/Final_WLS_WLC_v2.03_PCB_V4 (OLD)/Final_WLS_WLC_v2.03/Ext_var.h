#ifndef Ext_Var_h
#define Ext_Var_h

#include "OneButton.h"
#include "MapFloat.h"
#include "procss.h"
#include "Arduino.h"
#include <PID_v1.h>
#include "Ticker.h"
#include "Butn.h"
#include "led.h"
#include "Pid.h"
#include "Eeprom.h"
#include "Tick.h"
#include "Temp.h"
#include <EEPROM.h>


extern unsigned int processLog;
extern unsigned int processLogNumberOfTimes;
extern unsigned long actualRunMin;

extern bool Service_Menu;
extern bool COUNTDOWN_START;   
extern double PID_SETPOINT, PID_INPUT, PID_OUTPUT;
extern int VALUE_FOR_BLINK, TOTAL_VALUE_FOR_BLINK;
extern int8_t ENTER_BUT, LONG_PRESS, Mode_Button_Single_Press_In_Settings, TIME_WASTE_FLAG;
extern float ERROR_CAL, CAL2_CALCULATED_LOW_CAL_VALUE, CAL2_CALCULATED_HIGH_CAL_VALUE,  TEMP_CAL2_CALCULATED_HIGH_CAL_VALUE ;
extern float CAL2_RAW_LOW_VALUE, CAL2_RAW_HIGH_VALUE, RAW_LOW_VALUE, RAW_HIGH_VALUE,MASTER_LOW, MASTER_HIGH;
extern float SET_TEMP_DEG, TEMP_DEG,TEMP_DEG_RAW, TEMP_FAH_RAW, TEMP_FAH;
extern float RATE_OF_HEATING;
extern int  currentMillis, i, digits1[4],  digits2[4] , TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG; //currentMillis, 
extern bool CHECK_RATE_OF_HEATING;
extern bool DOT2_1, DOT2_2, DOT1_1, DOT1_2, DOT2_0,DOT1_0;
extern bool DEGREE_VALUE_SET, CAL2_START;
extern bool STIR_VALUE_SET, DEGREE_VALUE_SET, START_CAL_VALUE;
extern bool TIME_SOCK_OR_TOTAL,RST_BUTT, BLINK_STATS;
extern bool REPETATION_FLAG, EXECUTE_PID_LOOP;
extern bool ERROR_SET_IN_DEG_FLAG, ERROR_SET_IN_FAH_FLAG;
extern bool PUMP_FLAG;
extern int  FOR_REP, FOR_REP2, REP, REP1;
extern int  VALUE_FOR_BLINK,SET_MIN, SET_MINUTE, SET_HOUR;
extern int  SAFE_VALUE_SET_DEG,SAFE_VALUE_SET_FAH, ERROR_VALUE_SET;
extern int  LOW_CAL_VALUE_SET, HIGH_CAL_VALUE_SET, CAL_SELECT_VALUE;
extern int  CAL1_VALUE_SAVED_DEG1,CAL1_VALUE_SAVED_DEG2, CAL1_VALUE_SAVED_FAH;
extern int  CAL2_LOW1_VALUE_SET, CAL2_LOW2_VALUE_SET, CAL2_HIGH1_VALUE_SET, CAL2_HIGH2_VALUE_SET;
extern int  SELECTION_PARAMETER;
extern int  SET_TEMP_FAH, ERROR_CAL_IN_FAH; //currentMillis1, 
extern int  SET_LONG_VALUE[25],SET_VALUE[6];
extern int  MIN_SET_TEMP, MIN_SET_FAH;
extern long   CAL2_RAW_LOW_VALUE_CONV, CAL2_RAW_HIGH_VALUE_CONV,MASTER_LOW_CONV, MASTER_HIGH_CONV;
extern double Kp,Ki,Kd, Ki_saved,consKp, consKi, consKd, PID_SETPOINT;
extern double  ERROR_CAL1;
extern uint8_t Blinking_Freq;
extern bool SIGN,Proceess_Resume_Enable_Flag;
extern float  TEMPERARY_SAVED_VARIABLE_FOR_CAL1_FAH, TEMP_CAL2_RAW_LOW_VALUE, TEMP_CAL2_RAW_HIGH_VALUE; 
extern int TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG;
extern bool ERROR_SET_IN_DEG_FLAG, ERROR_SET_IN_FAH_FLAG;
extern bool Process_Interrupt_flag;
extern bool select_product_type; //select product WLC/WLS

class Vclass
{
  public :
  Vclass();
};

extern Vclass var;
#endif
