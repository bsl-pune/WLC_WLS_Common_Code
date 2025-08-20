#include "Ext_var.h"
  
Pclass::Pclass()
{}

unsigned int processLog;
unsigned int processLogNumberOfTimes;
int actualRunHours=0;
unsigned long  actualRunMin=0;
//-----------------------------------------------------------------------------------
void Alternate_Display();
Ticker Alter_Disp(Alternate_Display, 2000, 0);

//**************************************************************LONGPRESS*************************************************************************
float ERROR_CAL, CAL2_CALCULATED_LOW_CAL_VALUE, CAL2_CALCULATED_HIGH_CAL_VALUE ;
float CAL2_RAW_LOW_VALUE=0.00, CAL2_RAW_HIGH_VALUE=0.00, RAW_LOW_VALUE=0.00, RAW_HIGH_VALUE=0.00, MASTER_LOW=0.00, MASTER_HIGH=0.00;
float PID_SETPOINT_To_Show;
bool RST_BUTT=0, TIME_SOCK_OR_TOTAL=0; 
bool CAL2_START=0;
bool BLINK_STATS=0, EXECUTE_PID_LOOP=0;
double  ERROR_CAL1;
long CAL2_RAW_LOW_VALUE_CONV, CAL2_RAW_HIGH_VALUE_CONV,MASTER_LOW_CONV, MASTER_HIGH_CONV;
bool STIR_VALUE_SET=0, DEGREE_VALUE_SET, START_CAL_VALUE=0;
int  SAFE_VALUE_SET_DEG=0,SAFE_VALUE_SET_FAH=0, ERROR_VALUE_SET=0;
int  LOW_CAL_VALUE_SET=30, HIGH_CAL_VALUE_SET=0, CAL_SELECT_VALUE=0;
int  CAL2_LOW1_VALUE_SET=0, CAL2_LOW2_VALUE_SET=0, CAL2_HIGH1_VALUE_SET=0, CAL2_HIGH2_VALUE_SET=0;
int  CAL1_VALUE_SAVED_DEG1=0,CAL1_VALUE_SAVED_DEG2=0, CAL1_VALUE_SAVED_FAH=0;
int  SELECTION_PARAMETER=1;
//********************************************************SINGLEPRESS***************************************
float SET_TEMP_DEG=0.00;
int SET_TEMP_FAH=0,SET_TEMP=0, SET_HOUR=0, SET_MIN=0, SET_MINUTE=0;
int i=0, j=0,  REMAIN_MIN=0; 
int STOP_PROCESS=0;
double DISP_INC;
//********************************************************LEVEL_DETECTION***************************************
bool CHECK_RATE_OF_HEATING=0;
float TEMP_VALUE_INITIAL, TEMP_VALUE_FINAL ;
unsigned long START_COUNT_VALUE, STOP_COUNT_VALUE ;
float RATE_OF_HEATING=0;
//-----------------------------------------------------------------------------------------------------------------------------------
int VALUE_FOR_BLINK=25, TOTAL_VALUE_FOR_BLINK=50;
float PREVIOUS_TEMP_COOLING_RATE=0.00, CURRENT_TEMP_COOLING_RATE=0.00;
float COOLING_RATE=0.00, COOLOING_RATE_COMP_VAL=0.04;
unsigned long  PREVIOUS_COOLING_RATE_TIME=0 , CURRENT_COOLING_RATE_TIME=0, CAL_OF_10_MIN=0, TO_CHECK_REPETATION_WITHIN_TIME=0;
int COOLING_CYCLE_COUNT=0;
bool FILL_BATH_ERROR_NOT_CLEAR=0;
bool FOR_BUZZ=0;
bool Process_Stop;
bool Process_Interrupt_flag;
//------------------------------------------------------------------------------------------------------------------------------------
int MIN_SET_TEMP=0, MIN_SET_FAH=0;
bool IN_ERROR=0, REPETATION_FLAG=0;
unsigned short  TIMER_LED_BLINK=0;
uint8_t Blinking_Freq=0;
//------------------------------------------------------------------------------------------------------------------------------
int z=0;
int  currentMillis=0;
bool Alternate_Display_Flag=0;
bool select_product_type=0;
bool checkTempRaised=0; //Siddhesh

void Alternate_Display()                    //------------------to show set and current temoerature alternatively on display
{
  if(Alternate_Display_Flag) Alternate_Display_Flag=0;
  else                       Alternate_Display_Flag=1;
}

void Pclass:: Setup()                       //------------------process file one time execution in setup
{
   
  pinMode(BUZZER, OUTPUT);
  pinMode(HEATER, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(safteyRelay, OUTPUT); 
   
  digitalWrite(PUMP, LOW);  
  digitalWrite(BUZZER, LOW);
  digitalWrite(HEATER, LOW);
  digitalWrite(safteyRelay, LOW);  //---------Initially contactor should OFF(LOW),heater circuit will be open

  pinMode(HEATER_STATUS_LED, OUTPUT);
  pinMode(CURRENT_PARA_LED, OUTPUT);
  pinMode(SET_PARA_LED, OUTPUT);
  pinMode(ALARM_LED, OUTPUT);
  
  ALL_LED_OFF();  
}


void Pclass :: SET_SHORT_PRESS_FUNCTION()   //------------------function for main process execution
{
  switch (ENTER_BUT)
  {
    case 0 :
          DOT1_1 =  DOT1_0 = DOT1_2 = DOT2_2 = DOT2_0 =0;                      //------------------All dots of display should off
          REP= REP1= IN_ERROR= FOR_REP= FOR_REP2=0;                            //------------------All flags should be reset in Home page
          PUMP_FLAG = CAL2_START= Blinking_Freq=0;
          PID_SETPOINT= STOP_PROCESS= EXECUTE_PID_LOOP = RATE_OF_HEATING=0;
          Mode_Button_Single_Press_In_Settings =REPETATION_FLAG =0; 
          CHECK_RATE_OF_HEATING=1;                                            //------------------Fill bath error checking 
          Tick.BUZZ_TICK_STOP();
          digitalWrite(PUMP, LOW); 
          Tick.LED_Off_Call();     
          digitalWrite(HEATER, LOW);             
          Pid.PID_Stop();
          ALL_LED_OFF();
                      
          if(!checkTempRaised)
           { 
                                   
          if(Process_Interrupt_flag && Proceess_Resume_Enable_Flag)           //------------------to display  "ProS strt" 3 times, beep
          { 
             if(z<3)
             {
               if(i<VALUE_FOR_BLINK+50) 
               {
                 digitalWrite(BUZZER, HIGH);
                 led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                 led.DISPLAY_DIGIT2(24, 24, 24, 24) ;
               }
               else 
               {       
                 digitalWrite(BUZZER, LOW);
                 led.DISPLAY_DIGIT2(11, 17, 23, 12);
                 led.DISPLAY_DIGIT1(12, 16, 17, 16);
               }
     
               if(i>TOTAL_VALUE_FOR_BLINK+100) {z ++; i=0; }
             }
             else  ENTER_BUT=5;                                          //------------------start process directly                                   
             digitalWrite(TIMER_LED1, LOW); 
          }
          else
          { 
            if(!DEGREE_VALUE_SET)  
            {
                if(TEMP_DEG>99.9)  { led.BREAK_NUMBER2(TEMP_DEG*10);  DOT2_1=0; }
                else               { led.BREAK_NUMBER2(TEMP_DEG*100); DOT2_1=1; }
                digits2[3] = 10;
            }
            else
            {
               led.BREAK_NUMBER2(TEMP_FAH*10);
               digits2[3] = 21;   
               DOT2_1=0;          
            }
            led.BREAK_NUMBER1(0000);    
            digitalWrite(TIMER_LED1, HIGH); 
          }
           }
           else
          {
        digitalWrite(safteyRelay, LOW); //--------Contactor OFF
        digitalWrite(SET_PARA_LED, HIGH);
        digitalWrite(CURRENT_PARA_LED, HIGH);
        digitalWrite(HEATER_STATUS_LED, HIGH);  
        digitalWrite(ALARM_LED, LOW); 
        digitalWrite(TIMER_LED2, LOW);
        digitalWrite(TIMER_LED1, LOW);
        
        if(i<VALUE_FOR_BLINK) 
        {  
            digitalWrite(BUZZER, LOW);                 
            led.DISPLAY_DIGIT2(24, 24, 24, 24) ; 
            led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
        }
         else 
        {       
            digitalWrite(BUZZER, HIGH);
            DOT2_1=0;                               
            led.DISPLAY_DIGIT2(12, 12, 17, 24);// ssr 
            led.DISPLAY_DIGIT1(21, 13, 25, 26);// FAiL
        }                           
        if(i>TOTAL_VALUE_FOR_BLINK)  i=0;
  }
                      
          break;
    case 1 :
          DOT1_1=0;
          DOT2_1=0;             
          digitalWrite(SET_PARA_LED, LOW);          
          digitalWrite(TIMER_LED1, HIGH);  
          if(i<VALUE_FOR_BLINK) 
          {
              if(!BLINK_STATS) { digits2[0] = 24; digits2[1] = 24;  DOT2_1=0; }
              else i=VALUE_FOR_BLINK;
          }  
          else     
          {
            DOT2_1=1; 
            led.BREAK_NUMBER2((SET_VALUE[ENTER_BUT] * 100) + (SET_VALUE[ENTER_BUT +1]*10));
          }
          if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
          digits2[3] = 10;
          led.BREAK_NUMBER1((SET_VALUE[ENTER_BUT+2] *100) + SET_VALUE[ENTER_BUT+3]);
          break;
    case 2 :
          led.BREAK_NUMBER1((SET_VALUE[ENTER_BUT+1] *100) + SET_VALUE[ENTER_BUT+2]);
          if(!DEGREE_VALUE_SET)  
          {
            led.BREAK_NUMBER2((SET_VALUE[1] * 100) + (SET_VALUE[ENTER_BUT]*10));
            digits2[3] = 10;
            DOT2_1=1; 
            if(i<VALUE_FOR_BLINK) 
            {
             if(!BLINK_STATS) digits2[2] = 24;
              else i=VALUE_FOR_BLINK;  
            }
          }
          else
          {
            led.BREAK_NUMBER2(SET_VALUE[22]*10);
            digits2[3] = 21;   
            DOT2_1=0; 
            if(i<VALUE_FOR_BLINK) 
            {
              if(!BLINK_STATS  ){digits2[0] = 24; digits2[1] = 24;  digits2[2] = 24;}  
              else i=VALUE_FOR_BLINK;  
            }     
          }                               
          if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
          break;     
    case 3 :
          if(!DEGREE_VALUE_SET)  
          {
            led.BREAK_NUMBER2((SET_VALUE[1] * 100) + (SET_VALUE[2]*10));
            digits2[3] = 10;
            DOT2_1=1;  
          }
          else
          { 
            led.BREAK_NUMBER2(SET_VALUE[22]*10);
            digits2[3] = 21;   
            DOT2_1=0;                
          }   
          
          //i++;
          if(i<VALUE_FOR_BLINK)
          {
             if(!BLINK_STATS)    { digits1[0] = 24; digits1[1] = 24; DOT1_1=0;}
             else i=VALUE_FOR_BLINK;  
          }
          else 
          {
            DOT1_1=0;
            led.BREAK_NUMBER1((SET_VALUE[ENTER_BUT] *100) + SET_VALUE[ENTER_BUT+1]);
          }
          if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
          break;
    case 4 :
          DOT1_1=0;
          if(!DEGREE_VALUE_SET)  
          {
            led.BREAK_NUMBER2((SET_VALUE[1] * 100) + (SET_VALUE[2]*10));
            digits2[3] = 10;
            DOT2_1=1; 
          }
          else
          {
            led.BREAK_NUMBER2(SET_VALUE[22]*10);
            digits2[3] = 21;   
            DOT2_1=0;                
          }   
          led.BREAK_NUMBER1( (SET_VALUE[ENTER_BUT-1] *100) + SET_VALUE[ENTER_BUT] );
          if(i<VALUE_FOR_BLINK)
          {
             if(!BLINK_STATS)    { digits1[2] = 24; digits1[3] = 24;}
             else i=VALUE_FOR_BLINK;         
          }
          if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
          REP1=0;               
          SET_VALUE[5] = STIR_VALUE_SET;
          PUMP_FLAG =1;                       
          SET_MIN = (SET_VALUE[3]*60) + SET_VALUE[4];              
          break;
    case 5: 
          SET_TEMP_DEG =(SET_VALUE[1] + (SET_VALUE[2] / 10.0)); 
          SET_TEMP_FAH =  SET_VALUE[22];   
          SET_HOUR=SET_VALUE[3];   
          SET_MINUTE= SET_VALUE[4];                       
          SET_MIN = (SET_HOUR*60) + SET_MINUTE;  
          
          Process_Interrupt_flag=1;
          Eeprom.Process_Set_Values_Store_On_Eeprom();          
          Eeprom.Process_Time_Set_On_Eeprom();
          if(!FILL_BATH_ERROR_NOT_CLEAR)
          {
            
          if( (!DEGREE_VALUE_SET &&  SET_TEMP_DEG >= (TEMP_DEG + 10)) || (DEGREE_VALUE_SET && SET_TEMP_FAH>= (TEMP_FAH +15)) ) { FILL_BATH_ERROR_NOT_CLEAR=0; }
            else                                                                                                               { FILL_BATH_ERROR_NOT_CLEAR=1; }
          }  
//          else     
          if(FILL_BATH_ERROR_NOT_CLEAR)
          {
            RATE_OF_HEATING =0;
            CHECK_RATE_OF_HEATING=1;
            FILL_BATH_ERROR_NOT_CLEAR=0;
          }
          else
          {
            CHECK_RATE_OF_HEATING=0;
            digitalWrite(HEATER, HIGH);
            TEMP_VALUE_INITIAL = TEMP_DEG_RAW;
            START_COUNT_VALUE = millis();
          } 
          if(!DEGREE_VALUE_SET)    
          {
               PID_SETPOINT_To_Show = SET_TEMP_DEG;  
               PID_SETPOINT = SET_TEMP_DEG;              
          }
          else                 
          {
              PID_SETPOINT_To_Show = SET_TEMP_FAH;  
              PID_SETPOINT = (PID_SETPOINT_To_Show - 32) / 1.8;    //-------------------------------------------convert setpoint in degree
          }
          Alter_Disp.start();
          Alternate_Display_Flag =1; 

          digitalWrite(safteyRelay, HIGH);
                
          while(SET_MIN >0) 
          {
            if(TEMP_DEG > SET_TEMP_DEG + 10) //if temp greater than 150 ,it will show SSR fail 
            {
              i=0;
              checkTempRaised=1;
              digitalWrite(safteyRelay, LOW);  //--------if temp raises beyond 480, Contactor will get OFF, heater circuit will be open.
              ENTER_BUT=0;             
              SET_MIN=0;
            }
            else
            {
              checkTempRaised=0;
            }
               digitalWrite(SET_PARA_LED, HIGH);
               digitalWrite(HEATER_STATUS_LED, LOW);               
               Temp.RTD_TEMP_CONV();
               led.DISP_UPDATE();
              
               if(STIR_VALUE_SET && select_product_type)  digitalWrite(PUMP, HIGH); //Siddhesh
               else                                       digitalWrite(PUMP, LOW);
               
               if( !CHECK_RATE_OF_HEATING )  STOP_COUNT_VALUE = millis();
               
               if(((STOP_COUNT_VALUE - START_COUNT_VALUE) >= 180000) && !CHECK_RATE_OF_HEATING )       //---------------------------Rate of heating   //210000
               {
                 TEMP_VALUE_FINAL = TEMP_DEG_RAW;
                 RATE_OF_HEATING  = (TEMP_VALUE_FINAL - TEMP_VALUE_INITIAL)/((STOP_COUNT_VALUE - START_COUNT_VALUE) / 60000);    
                 PREVIOUS_COOLING_RATE_TIME = millis();                                              //-----------------------------To check cooling Rate 
                 CAL_OF_10_MIN=millis();                                                            
                 PREVIOUS_TEMP_COOLING_RATE = TEMP_DEG_RAW;
                 CHECK_RATE_OF_HEATING =1;                                                          //------------------------------Heating rate is checked                            
               } 
               
               if(RATE_OF_HEATING >= 1  && CHECK_RATE_OF_HEATING )                                 //------------------------------- If water leve is less
               {                    
                  digitalWrite(HEATER, LOW);
                  Pid.PID_Stop();                                                                  //-------------------------------stop pid 
                  CURRENT_COOLING_RATE_TIME = millis();
                  EXECUTE_PID_LOOP =0;             
                  CURRENT_TEMP_COOLING_RATE= TEMP_DEG_RAW;      
                if((CURRENT_COOLING_RATE_TIME - PREVIOUS_COOLING_RATE_TIME) == 60000)              //--------------------------------check wAater is pour or not after every 1min
                {
                     COOLING_RATE =((CURRENT_TEMP_COOLING_RATE - PREVIOUS_TEMP_COOLING_RATE) /(( CURRENT_COOLING_RATE_TIME - PREVIOUS_COOLING_RATE_TIME)/60000));
                     if(COOLING_RATE <= - COOLOING_RATE_COMP_VAL) 
                    {
                      if(COOLING_CYCLE_COUNT < 1) 
                      {
                        COOLING_CYCLE_COUNT++;
                        COOLOING_RATE_COMP_VAL = 0.01;
                        TO_CHECK_REPETATION_WITHIN_TIME = millis(); 
                      }
                      else
                      {
                        if((CURRENT_COOLING_RATE_TIME - TO_CHECK_REPETATION_WITHIN_TIME ) < 90000)
                           COOLING_CYCLE_COUNT++;  
                        else   
                          COOLING_CYCLE_COUNT = 0;                     
                      }
                      
                    }
                    else if(( CURRENT_COOLING_RATE_TIME - CAL_OF_10_MIN )> 420000)
                    {
                      FILL_BATH_ERROR_NOT_CLEAR=1;
                      STOP_PROCESS=1;
                      digitalWrite(BUZZER,LOW);
                      ENTER_BUT=0;
                      SET_MIN =0;
                    }
                    if(COOLING_CYCLE_COUNT==2) RATE_OF_HEATING=0;  
                    PREVIOUS_COOLING_RATE_TIME = CURRENT_COOLING_RATE_TIME;                 
                    PREVIOUS_TEMP_COOLING_RATE = CURRENT_TEMP_COOLING_RATE;
                  }    
               }
               else if(RATE_OF_HEATING < 1 && CHECK_RATE_OF_HEATING)    
               {
                  if(!EXECUTE_PID_LOOP) { Pid.PID_Start(); EXECUTE_PID_LOOP=1;}
                  Pid.PID_callingg(); 
               } 
                            
               if(!TIME_SOCK_OR_TOTAL)                     COUNTDOWN_START=1; 
               else
               {
                 if(!DEGREE_VALUE_SET)  
                 {
                     if( TEMP_DEG >= PID_SETPOINT_To_Show )        COUNTDOWN_START=1; 
                     else                                          COUNTDOWN_START=0; 
                 }
                 else  
                 {
                     if( TEMP_FAH >= PID_SETPOINT_To_Show )        COUNTDOWN_START=1; 
                     else                                          COUNTDOWN_START=0; 
                 }
               }                 
               if(TEMP_DEG >= PID_SETPOINT_To_Show )
               {                                       
                   Tick.BUZZ_INTERVAL2(400); 
                   Tick.BUZZ_INTERVAL1(800);                     
                    if(REP == 0)  
                    {
                      REPETATION_FLAG=1;
                      Tick.BUZZ_TICK_START();
                      REP++;
                    }                    
                    if(FOR_REP==2) 
                    {
                      Tick.BUZZ_TICK_STOP();
                      REPETATION_FLAG=0;  
                    }
               }                  
               SET_HOUR = SET_MIN/60;
               REMAIN_MIN = SET_MIN % 60 ;
               DISP_INC++;
               DOT1_1=0; 
                        
               if(RATE_OF_HEATING >= 1 ) 
               {
                 DOT1_1=0;
                 DOT2_1=0;
                 IN_ERROR=1;
                 REPETATION_FLAG=0;  
                 ALL_LED_OFF();
                 digitalWrite(TIMER_LED2, LOW); 
                 led.DISPLAY_DIGIT2(21, 25, 26, 26) ;
                 led.DISPLAY_DIGIT1(14, 13, 16, 30) ;               
                 Tick.BUZZ_INTERVAL2(1000); 
                 Tick.BUZZ_INTERVAL1(2000); 
                 if(REP == 0)  
                 {
                      Tick.BUZZ_TICK_START();
                      REP++;
                 }
               } 
               else
               {
                 if(PUMP_FLAG && select_product_type)
                  {
                    DOT1_1=0;
                    DOT2_1=0; 
                    DOT1_0=0;
                    ALL_LED_OFF();    
                    digitalWrite(TIMER_LED1, LOW);      
                    led.DISPLAY_DIGIT2(12, 16, 25, 17) ;  
                    if(!SET_VALUE[5])     led.DISPLAY_DIGIT1(0, 21, 21, 24);
                    else                  led.DISPLAY_DIGIT1(0, 20, 24, 24);
                  }
                 else
                 {
                   TIMER_LED_BLINK++;
                   if(TIMER_LED_BLINK >=0 && TIMER_LED_BLINK<2200)                    digitalWrite(TIMER_LED1, HIGH); 
                   else  if(TIMER_LED_BLINK>=2200 && TIMER_LED_BLINK<4400)            digitalWrite(TIMER_LED1, LOW); 
                   else                                                                TIMER_LED_BLINK=0;
                      
                   if(IN_ERROR)  Tick.BUZZ_TICK_STOP();
                                     
                   led.BREAK_NUMBER1( (SET_HOUR *100) + REMAIN_MIN );             
                   Alter_Disp.update();
  
                   if( Alternate_Display_Flag) //                      DISP_INC>0 && DISP_INC<=12000)
                     {
                        digitalWrite(CURRENT_PARA_LED, HIGH); 
                        digitalWrite(SET_PARA_LED, LOW);
                        if(!DEGREE_VALUE_SET)  
                        {
                          led.BREAK_NUMBER2(PID_SETPOINT_To_Show*100);  //(SET_TEMP_DEG*100);
                          digits2[3] = 10;
                          DOT2_1=1;
                        }
                        else
                        {
                          led.BREAK_NUMBER2(PID_SETPOINT_To_Show*10);
                          digits2[3] = 21;            
                        }
                     }
                     else   //if(DISP_INC>12000 && DISP_INC<=24000)
                     {
                        digitalWrite(CURRENT_PARA_LED, LOW);
                        digitalWrite(SET_PARA_LED, HIGH);
                        if(!DEGREE_VALUE_SET)  
                        {
                          if(TEMP_DEG>99.9) { led.BREAK_NUMBER2(TEMP_DEG*10);  DOT2_1=0;}    //TEMP_DEG_RAW
                          else              { led.BREAK_NUMBER2(TEMP_DEG*100); DOT2_1=1;}
                          digits2[3] = 10;
                        }
                        else
                        {
                          led.BREAK_NUMBER2(TEMP_FAH*10);
                          digits2[3] = 21;            
                        }
                     }
                    } 
                  }
             }
             
             digitalWrite(safteyRelay, LOW);
             EEPROM.write(160, 0);
             
             ALL_LED_OFF();    
             Tick.LED_On_Call();   
             Process_Interrupt_flag=0;            
             Eeprom.Process_Time_Set_On_Eeprom();
              
             actualRunHours = (actualRunMin/60);
             EEPROM.get(152,processLog );
             processLog = processLog + actualRunHours ; //Hours;
             if(processLog >= 9999) 
             {
              processLogNumberOfTimes = processLogNumberOfTimes+1;
              processLog =0;        
             }      
              EEPROM.put(152,processLog );
              EEPROM.put(154, processLogNumberOfTimes);
              
             actualRunHours = 0;
             actualRunMin    =0;
             
              Alter_Disp.stop();
              while(Blinking_Freq<3 ) // & !checkTempRaised)
              {                  
                 Tick.BUZZ_INTERVAL(500);  
                 Pid.PID_Stop();
                 led.DISP_UPDATE();      
//                 if(!checkTempRaised) Blinking_Freq=3;//siddhesh
              }             
              ENTER_BUT=0;                    
           break;
    default:
           break;
  }
}

void Pclass :: SET_LONG_PRESS_FUNCTION()
{
  digitalWrite(TIMER_LED1, LOW);   
  if(Mode_Button_Single_Press_In_Settings)       // -----------------When User is in edit parameter mode 
  {
      switch (SELECTION_PARAMETER)
      {
        case Stir_ON_OFF : 
                DOT1_1=0;
                DOT2_1=0; 
                DOT1_0=0;
                ALL_LED_OFF();
                led.DISPLAY_DIGIT2(12, 16, 25, 17) ;
                if(i<VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                else 
                {        
                  if(!SET_LONG_VALUE[SELECTION_PARAMETER])   led.DISPLAY_DIGIT1(0, 21, 21, 24);
                  else                                       led.DISPLAY_DIGIT1(0, 20, 24, 24);
                }
                if(i>TOTAL_VALUE_FOR_BLINK)    i=0;
                break;
         case Unit_C_F :                                          //unit                                                                          //stir
                ALL_LED_OFF();
                DOT1_1=0;
                DOT2_1=0; 
                led.DISPLAY_DIGIT2(28, 20, 25, 16) ; 
//                i++;
                if(i<VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                else        
                {
                  if(!SET_LONG_VALUE[SELECTION_PARAMETER])      led.DISPLAY_DIGIT1(29, 10, 24, 24);
                  else                                          led.DISPLAY_DIGIT1(29, 21, 24, 24);
                }
                if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
                break;
                
        case Safe_Value :                                                       //safe
                ALL_LED_OFF();
                DOT1_1=0;
                led.DISPLAY_DIGIT2(12, 13, 21, 15) ;
                //i++;
                if(i<VALUE_FOR_BLINK)  
                {
                  if(!BLINK_STATS)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                  else i=VALUE_FOR_BLINK;         
                }
                else 
                {
                  if(!DEGREE_VALUE_SET)  
                  {
                    led.BREAK_NUMBER1(SET_LONG_VALUE[SELECTION_PARAMETER] * 10);  
                    digits1[0] = 24;  
                  }
                  else        
                    led.BREAK_NUMBER1(SET_LONG_VALUE[Safe_In_Fah] * 10);
                }
                if(!DEGREE_VALUE_SET)   digits1[3] = 10;    
                else                    digits1[3] = 21; 
                if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
                break;
       case Time_Mode:                                              //time selection
             DOT1_0=0;
             DOT1_1=0;
             DOT2_1=0; 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(16, 25, 18, 15) ;
             //i++;
             if(i<VALUE_FOR_BLINK)  
             {
                if(!BLINK_STATS)  led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                else i=VALUE_FOR_BLINK;  
             }
             else 
             { 
                 if(!SET_LONG_VALUE[SELECTION_PARAMETER]) led.DISPLAY_DIGIT1(16, 23, 16, 31);
                 else                                     led.DISPLAY_DIGIT1(12, 23, 13, 10);
             }
             if(i>TOTAL_VALUE_FOR_BLINK)    i=0;
             break;
       case Auto_Resume_Enable: 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(11, 17, 23, 12) ; //Pros
             if(i<VALUE_FOR_BLINK)  
             {
                if(!BLINK_STATS)  led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                else i=VALUE_FOR_BLINK;  
             }
             else 
             { 
                 if(!SET_LONG_VALUE[SELECTION_PARAMETER])  led.DISPLAY_DIGIT1(32, 25, 12, 14); //disb
                 else                                      led.DISPLAY_DIGIT1(15, 20, 14, 26); //enbl 
             }
             if(i>TOTAL_VALUE_FOR_BLINK)    i=0;    
             break;    


        case Product_Selection_Parameter: //calb  200723 //Product_Selection
             DOT1_1=0;            
             DOT2_1=0; 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(11, 17, 32, 16) ; //PRDT
             //led.DISPLAY_DIGIT1(16, 35, 11, 15) ; //type

             if(i<VALUE_FOR_BLINK)  
             {
                if(!BLINK_STATS)  led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                else i=VALUE_FOR_BLINK;  
             }
             else 
             { 
                 if(SET_LONG_VALUE[SELECTION_PARAMETER])  led.DISPLAY_DIGIT1(36, 26, 12, 24); //WLS
                 else                                      led.DISPLAY_DIGIT1(36, 26, 10, 24); //WLC 
             }
             if(i>TOTAL_VALUE_FOR_BLINK)    i=0;  
            // if(select_product_type)   led.DISPLAY_DIGIT1(36, 26, 12, 37) ; //WLS
            // else                      led.DISPLAY_DIGIT1(36, 26, 10, 37) ; //WLC
             break; 


       case Calibration_Selection:                                                                      //calb  
             DOT1_1=0;
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(10, 13, 26, 14) ;
             if(i<VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
             else 
             {
                 if(SET_LONG_VALUE[SELECTION_PARAMETER])   led.DISPLAY_DIGIT1(10, 13, 26, 2) ;
                 else                                      led.DISPLAY_DIGIT1(10, 13, 26, 1) ;
             }
             if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
             break;  
               
       case Cal1_Decimal  :                                                            //cal1 -L
               DOT1_0=0;
               ALL_LED_OFF();  
               led.DISPLAY_DIGIT2(10, 13, 26, 1) ;
               if(i<VALUE_FOR_BLINK)  
                {
                  if(!BLINK_STATS)    {digits1[0] = 24; digits1[1] = 24; }
                  else i=VALUE_FOR_BLINK;         
                }
               else  led.BREAK_NUMBER1((SET_LONG_VALUE[Cal1_Decimal] *100) + ( SET_LONG_VALUE[Cal1_Point] * 10));
               digits1[3] = 10;
               if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
               break;
               
       case Cal1_Point : 
               if(!DEGREE_VALUE_SET)  
               {
                  if(i<VALUE_FOR_BLINK)  
                  {
                    if(!BLINK_STATS)    digits1[2] = 24; 
                    else i=VALUE_FOR_BLINK;         
                  }
                  else   led.BREAK_NUMBER1((SET_LONG_VALUE[Cal1_Decimal] *100) + ( SET_LONG_VALUE[Cal1_Point] * 10)); 
                   digits1[3] = 10;
               }
               else
               {
                   if(i<VALUE_FOR_BLINK)
                   {
                       if(!BLINK_STATS)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                       else i=VALUE_FOR_BLINK; 
                   }
                   else        led.BREAK_NUMBER1(SET_LONG_VALUE[Cal1_In_Fah] *10); 
                   digits1[3] = 21;
               }
               if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
               break;                              
       case Cal2LowTemp_Set_Display :                                                                      //cal2-L                                                                        
               DOT1_0=1;
               DOT1_1=0;
               ALL_LED_OFF();
               CAL2_START=1;
               led.DISPLAY_DIGIT2(10, 13, 26, 2) ;
               if(i<VALUE_FOR_BLINK)   
               {
                   if(!BLINK_STATS) led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                   else i=VALUE_FOR_BLINK; 
               }
               else        
               {
                  led.BREAK_NUMBER1(SET_LONG_VALUE[Cal2LowTemp_Set_Display] *10);  
               }
               digits1[0] = 26;
               digits1[3] = 10; 
               if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
               break;               
       case Cal2HighTemp_Set_Display  :                                                                //cal2-H
               ALL_LED_OFF();
               led.DISPLAY_DIGIT2(10, 13, 26, 2) ;
               if(i<VALUE_FOR_BLINK)  
               {
                   if(!BLINK_STATS)   led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                   else i=VALUE_FOR_BLINK; 
               }
               else 
               {       
                 led.BREAK_NUMBER1(SET_LONG_VALUE[Cal2HighTemp_Set_Display] *10); 
               }
               digits1[0] = 30;
               digits1[3] = 10; ;
               if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
               break;               
           
       case Cal2LowDecimalTemp_Enter_Heat:                       //Inprocess-Entering value
              ALL_LED_OFF();
              DOT1_1=1;
              led.BREAK_NUMBER2(LOW_CAL_VALUE_SET);
              digits2[0] = 26;
              digits2[1] = 23;
              //i++;
              if(i<VALUE_FOR_BLINK)
              {  
                if(!BLINK_STATS)  {digits1[0] = 24; digits1[1] = 24; }
                else i=VALUE_FOR_BLINK; 
              }
              else        
              {
                led.BREAK_NUMBER1((SET_LONG_VALUE[SELECTION_PARAMETER] *100) + (SET_LONG_VALUE[SELECTION_PARAMETER+1] *10));  
                digits1[3] = 10; 
              }
              if(i>TOTAL_VALUE_FOR_BLINK)   i=0;              
              break; 
              
       case Cal2LowPointTemp_Enter_Maintain :                                                           //Inprocess-Entering value decimal
              ALL_LED_OFF();
              led.BREAK_NUMBER2(LOW_CAL_VALUE_SET);
              digits2[0] = 26;
              digits2[1] = 23;
              //i++;
              if(i<VALUE_FOR_BLINK)
              {
                if(!BLINK_STATS)  digits1[2] = 24; 
                else i=VALUE_FOR_BLINK;
              }
              else        
              {
                led.BREAK_NUMBER1((SET_LONG_VALUE[SELECTION_PARAMETER -1] *100) + (SET_LONG_VALUE[SELECTION_PARAMETER] *10));  
              }
              digits1[3] = 10; 
              if(i>TOTAL_VALUE_FOR_BLINK)   i=0;             
              CAL2_CALCULATED_LOW_CAL_VALUE  = ((SET_LONG_VALUE[Cal2LowDecimalTemp_Enter_Heat] ) + (SET_LONG_VALUE[Cal2LowPointTemp_Enter_Maintain] / 10.0));   //f;                     
             break;  
       case Cal2HighDecimalTemp_Enter_Heat :                                                          //Inprocess-Entering value High
              ALL_LED_OFF();
              led.BREAK_NUMBER2(HIGH_CAL_VALUE_SET);
              digits2[0] = 30;
              digits2[1] = 25;
              DOT1_1=1;
              //i++;
              if(i<VALUE_FOR_BLINK)
              {
                  if(!BLINK_STATS)  {digits1[0] = 24; digits1[1] = 24; }
                  else i=VALUE_FOR_BLINK;
              }
              else        
              {
                led.BREAK_NUMBER1((SET_LONG_VALUE[SELECTION_PARAMETER] *100) + (SET_LONG_VALUE[SELECTION_PARAMETER+1] *10));  
              }
              digits1[3] = 10; 
              if(i>TOTAL_VALUE_FOR_BLINK)   i=0;              
              break;               
       case Cal2HighPointTemp_Enter_Maintain :                                                            //Inprocess-Entering value High
              ALL_LED_OFF();
              led.BREAK_NUMBER2(HIGH_CAL_VALUE_SET);
              digits2[0] = 30;
              digits2[1] = 25;
              DOT1_1=1;
              //i++;
              if(i<VALUE_FOR_BLINK) 
              {
                  if(!BLINK_STATS)  digits1[2] = 24; 
                  else i=VALUE_FOR_BLINK;
              }
              else        
              {
                led.BREAK_NUMBER1((SET_LONG_VALUE[SELECTION_PARAMETER -1] *100) + (SET_LONG_VALUE[SELECTION_PARAMETER] *10));  
              }
              digits1[3] = 10; 
              if(i>TOTAL_VALUE_FOR_BLINK)   i=0;
               
               CAL2_CALCULATED_HIGH_CAL_VALUE = ((SET_LONG_VALUE[SELECTION_PARAMETER -1]) + (SET_LONG_VALUE[SELECTION_PARAMETER] / 10.0)); //f; 
              break; 
        case Cal2_Complete_Display:   
             Service_Menu =0;     
             LONG_PRESS=0;
             break;

        case Log:                                          //Siddhesh Log
             DOT1_0=0;
             DOT1_1=0;
             DOT2_1=0; 
             ALL_LED_OFF();
              led.BREAK_NUMBER2(processLog*10);
                digits2[3] = 30;
                led.BREAK_NUMBER1(processLogNumberOfTimes);//processLogNumberOfTimes*10
                digits1[0] = 13;
             currentMillis=0;        
        break;
        
        case FactoryAndSure_Reset:
             DOT1_0=0;
             DOT1_1=0;
             DOT2_1=0; 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(12, 28, 17, 15) ;
             led.DISPLAY_DIGIT1(17, 15, 12, 16) ;
             currentMillis=0;        
        break;
        case ResetDone:
             DOT1_0=0;
             DOT1_1=0;
             DOT2_1=0; 
             ALL_LED_OFF();
             
             if(currentMillis >= 0 && currentMillis <= 30) 
             { 
              digitalWrite(BUZZER, HIGH); 
              led.DISPLAY_DIGIT2(22, 22, 22, 22) ;
              led.DISPLAY_DIGIT1(22, 22, 22, 22) ;             
             }
             else if(currentMillis > 40  &&  currentMillis <= 170) 
             { 
             digitalWrite(BUZZER, LOW); 
             led.DISPLAY_DIGIT2(17, 15, 12, 16)  ;  //rest
             led.DISPLAY_DIGIT1(32, 23, 20, 15) ;   //done
             }
             else  if(currentMillis > 170) 
             {                 
                  Eeprom.EEPROM_CLEAR();
             }
             break;      
        default:
            break;                                                                        
      }
  }
  else                                            // -----------------When User is in not edit parameter mode / scrolling  
  { 
      switch (SELECTION_PARAMETER)
      {
        case Stir_ON_OFF  ://stir
                DOT1_1=0;
                DOT2_1=0; 
                DOT1_0=0;
                ALL_LED_OFF();
                led.DISPLAY_DIGIT2(12, 16, 25, 17) ;
                if(!STIR_VALUE_SET)   led.DISPLAY_DIGIT1(0, 21, 21, 24);
                else                  led.DISPLAY_DIGIT1(0, 20, 24, 24);
                break;
        case Unit_C_F  :  //unit                                                                         
                ALL_LED_OFF();
                DOT1_1=0;
                DOT2_1=0; 
                led.DISPLAY_DIGIT2(28, 20, 25, 16) ; 
                if(!DEGREE_VALUE_SET)   led.DISPLAY_DIGIT1(29, 10, 24, 24);
                else                    led.DISPLAY_DIGIT1(29, 21, 24, 24);
                break;
        case Safe_Value  :  //safe
                ALL_LED_OFF();
                led.DISPLAY_DIGIT2(12, 13, 21, 15) ;
                if(!DEGREE_VALUE_SET)  
                {
                   led.BREAK_NUMBER1(SAFE_VALUE_SET_DEG * 10);  
                   digits1[3] = 10; 
                   digits1[0] = 24;     
                }
                else
                {
                   DOT1_1=0;
                   led.BREAK_NUMBER1(SAFE_VALUE_SET_FAH * 10);  
                   digits1[3] = 21; 
                }
                break;                                              
      case Time_Mode :   //time selection
             DOT1_0=0;
             DOT1_1=0;
             DOT2_1=0; 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(16, 25, 18, 15) ;
             if(!TIME_SOCK_OR_TOTAL) led.DISPLAY_DIGIT1(16, 23, 16, 31);
             else                    led.DISPLAY_DIGIT1(12, 23, 13, 10);
             break;
       case Auto_Resume_Enable: 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(13, 34, 16, 23) ;  //Auto
             led.DISPLAY_DIGIT1(11, 17, 23, 12) ; //Pros
             break;

             
        case Product_Selection_Parameter: //calb  200723 //Product_Selection
             DOT1_1=0;            
             DOT2_1=0; 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(11, 17, 32, 16) ; //PRDT
             led.DISPLAY_DIGIT1(16, 35, 11, 15) ; //type
            // if(select_product_type)   led.DISPLAY_DIGIT1(36, 26, 12, 37) ; //WLS
            // else                      led.DISPLAY_DIGIT1(36, 26, 10, 37) ; //WLC
             break; 
             

       case Calibration_Selection: //calb  
             DOT1_1=0;            
             DOT2_1=0; 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(10, 13, 26, 14) ;
             if(CAL_SELECT_VALUE)   led.DISPLAY_DIGIT1(10, 13, 26, 2) ;
             else                   led.DISPLAY_DIGIT1(10, 13, 26, 1) ;
             break;                
       case Cal1_Decimal  :                           //cal1
               DOT1_0=0;
               ALL_LED_OFF();  
               led.DISPLAY_DIGIT2(10, 13, 26, 1) ;
               if(!DEGREE_VALUE_SET)  
               {
                  if(TEMP_DEG>99.9)  { led.BREAK_NUMBER1(TEMP_DEG_RAW*10);   DOT1_1=0; }
                  else               { led.BREAK_NUMBER1(TEMP_DEG_RAW*100);  DOT1_1=1;}
                  digits1[3] = 10; 
               }
               else
               {
                   DOT1_1=0;
                   led.BREAK_NUMBER1(TEMP_FAH_RAW * 10);
                   digits1[3] = 21; 
               }
               break;
       case Cal1_Point  : //cal1
               DOT1_0=0;
               ALL_LED_OFF();  
               led.DISPLAY_DIGIT2(10, 13, 26, 1) ;
               if(!DEGREE_VALUE_SET)  
               {
                  led.BREAK_NUMBER1(TEMP_DEG_RAW *100);                    
                  if(TEMP_DEG>99.9)  {led.BREAK_NUMBER1(TEMP_DEG_RAW*10);  DOT1_1=0;}
                  else               {led.BREAK_NUMBER1(TEMP_DEG_RAW*100); DOT1_1=1;}
                  digits1[3] = 10; 
               }
               else
               {
                   DOT1_1=0;
                   led.BREAK_NUMBER1(TEMP_FAH_RAW * 10);
                   digits1[3] = 21; 
               }
               break;              
       case Cal2LowTemp_Set_Display :                      //cal2-L                                                                        // CAL1
               DOT1_0=1;
               DOT1_1=0;
               ALL_LED_OFF();
               led.DISPLAY_DIGIT2(10, 13, 26, 2) ;
               led.BREAK_NUMBER1(LOW_CAL_VALUE_SET *10);   
               digits1[0] = 26;
               digits1[3] = 10; 
               break;
       case Cal2HighTemp_Set_Display  :                       //cal2-H
               ALL_LED_OFF();
               led.DISPLAY_DIGIT2(10, 13, 26, 2) ;
               led.BREAK_NUMBER1(HIGH_CAL_VALUE_SET *10);
               digits1[0] = 30;
               digits1[3] = 10; 
               break;
       case Cal2LowDecimalTemp_Enter_Heat:                        //cal2-strt  process heat
              DOT1_0=0;
              DOT2_1=1;
              DOT1_1=0;
              ALL_LED_OFF();
              Pid.PID_Start();
              PID_SETPOINT = SET_LONG_VALUE[Cal2LowTemp_Set_Display];
              digitalWrite(safteyRelay, HIGH);
              while((PID_INPUT < PID_SETPOINT) && RST_BUTT)
              {
                Temp.RTD_TEMP_CONV();
                PID_SETPOINT = SET_LONG_VALUE[Cal2LowTemp_Set_Display];
                Pid.PID_callingg();                
                digitalWrite(HEATER_STATUS_LED, LOW);  
                led.DISP_UPDATE();
                if(i<VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                else                     led.DISPLAY_DIGIT1(30, 15, 13, 16) ;
                if(i>TOTAL_VALUE_FOR_BLINK)    i=0;
                led.BREAK_NUMBER2(LOW_CAL_VALUE_SET);
                digits2[0] = 26;
                digits2[1] = 23;          
             }
             digitalWrite(safteyRelay, LOW);
             SET_MIN=30;
             COUNTDOWN_START=1; 
             SELECTION_PARAMETER++;
             break;
      case Cal2LowPointTemp_Enter_Maintain: 
             digitalWrite(safteyRelay, HIGH);
             while((!Mode_Button_Single_Press_In_Settings) && RST_BUTT  && COUNTDOWN_START)
             {                
                Temp.RTD_TEMP_CONV();
                PID_SETPOINT = SET_LONG_VALUE[Cal2LowTemp_Set_Display]; 
                led.DISP_UPDATE();
                Pid.PID_callingg();
                digitalWrite(ALARM_LED, LOW);
                digitalWrite(HEATER_STATUS_LED, HIGH);
                led.BREAK_NUMBER2(LOW_CAL_VALUE_SET);
                digits2[0] = 26;
                digits2[1] = 23;
                DOT2_1=1;
                             
                if(i<VALUE_FOR_BLINK)  
                {
                  led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                  digitalWrite(BUZZER, LOW); 
                  DOT1_1=0;
                }
                else         
                {
                  digitalWrite(BUZZER, HIGH); 
                  if(TEMP_DEG>99.9)  { led.BREAK_NUMBER1(TEMP_DEG_RAW*10); DOT1_1=0; }
                  else               { led.BREAK_NUMBER1(TEMP_DEG_RAW*100); DOT1_1=1; }
                  DOT1_1=1; 
                  digits1[3] = 10; 
                }
                if(i>TOTAL_VALUE_FOR_BLINK)   i=0;            
             }
             digitalWrite(safteyRelay, LOW); 
              
             if(COUNTDOWN_START)    
                SELECTION_PARAMETER =Cal2LowDecimalTemp_Enter_Heat;        
             else         
             {
                SELECTION_PARAMETER =Cal2HighDecimalTemp_Enter_Heat;   
                Mode_Button_Single_Press_In_Settings=0;     
             }
             break; 
     case Cal2HighDecimalTemp_Enter_Heat :                                               //cal2-strt
            DOT1_0=0;
            DOT1_1=0;
            DOT2_1=1;
            PID_SETPOINT = SET_LONG_VALUE[Cal2HighTemp_Set_Display];
            digitalWrite(safteyRelay, HIGH);
            while((PID_INPUT < PID_SETPOINT) && RST_BUTT  )
            {
                Temp.RTD_TEMP_CONV();
                PID_SETPOINT = SET_LONG_VALUE[Cal2HighTemp_Set_Display]; 
                Pid.PID_callingg();
                led.DISP_UPDATE();
                digitalWrite(HEATER_STATUS_LED, LOW);
                if(i<VALUE_FOR_BLINK)    led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                else                     led.DISPLAY_DIGIT1(30, 15, 13, 16) ;
                if(i>TOTAL_VALUE_FOR_BLINK)   i=0;            
                led.BREAK_NUMBER2(HIGH_CAL_VALUE_SET);
                digits2[0] = 30;
                digits2[1] = 25;
            }
            digitalWrite(safteyRelay, LOW);                   
            SET_MIN=30;
            COUNTDOWN_START=1; 
            SELECTION_PARAMETER++;
            break; 
     case Cal2HighPointTemp_Enter_Maintain : // maintain temp till user interupt for entering temp
             digitalWrite(safteyRelay, HIGH);
             
             while((!Mode_Button_Single_Press_In_Settings)&& RST_BUTT && COUNTDOWN_START )
             {  
                Temp.RTD_TEMP_CONV();
                PID_SETPOINT = SET_LONG_VALUE[Cal2HighTemp_Set_Display];
                Pid.PID_callingg();
                led.DISP_UPDATE();
                digitalWrite(ALARM_LED, LOW);
                digitalWrite(HEATER_STATUS_LED, HIGH);
                led.BREAK_NUMBER2(HIGH_CAL_VALUE_SET);
                digits2[0] = 30;
                digits2[1] = 25;
                DOT2_1=1;
                                
                if(i<VALUE_FOR_BLINK)   
                {
                  led.DISPLAY_DIGIT1(24, 24, 24, 24) ;
                  digitalWrite(BUZZER, LOW); 
                }
                else         
                {
                  digitalWrite(BUZZER, HIGH);
                  if(TEMP_DEG>99.9)  { led.BREAK_NUMBER1(TEMP_DEG_RAW*10); DOT1_1=0; }
                  else               { led.BREAK_NUMBER1(TEMP_DEG_RAW*100);DOT1_1=1; }
                  digits1[3] = 10; 
                }
                if(i>TOTAL_VALUE_FOR_BLINK)   i=0;            
             }
             digitalWrite(safteyRelay, LOW);  
             
             PID_SETPOINT=0; 
                 
             if(COUNTDOWN_START)                    SELECTION_PARAMETER = Cal2HighDecimalTemp_Enter_Heat;        
             else         
             {
                LONG_PRESS =0;   
                Mode_Button_Single_Press_In_Settings=0;     
             }                   
           break;
      case Cal2_Complete_Display :
             DOT2_1=0;
             DOT1_1=0;
             if(i<TOTAL_VALUE_FOR_BLINK+150) 
             {
                led.DISPLAY_DIGIT2(10, 13, 26, 14) ;
                led.DISPLAY_DIGIT1(12, 13, 28, 15) ;
             }
             else
             {
                Service_Menu=0;
                Mode_Button_Single_Press_In_Settings = 0; 
                LONG_PRESS=0;
             }
             break;
       case Log:                                         //Siddhesh (Log Heater Time)
             DOT1_0=0;
             DOT1_1=0;
             DOT2_1=0; 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(26, 23, 27, 24) ;  //Log
             led.DISPLAY_DIGIT1(24, 24, 24, 24) ;  //Blank
             break;
             
       case FactoryAndSure_Reset:
             DOT1_0=0;
             DOT1_1=0;
             DOT2_1=0; 
             ALL_LED_OFF();
             led.DISPLAY_DIGIT2(21, 13, 10, 16) ;  //fact
             led.DISPLAY_DIGIT1(17, 15, 12, 16) ;
             break;
        default:
            break;                 
      }      
  }  
}

void Pclass :: ALL_LED_OFF()
{
  digitalWrite(ALARM_LED, HIGH);
  digitalWrite(SET_PARA_LED, HIGH);
  digitalWrite(CURRENT_PARA_LED, HIGH);
  digitalWrite(HEATER_STATUS_LED, HIGH);
}

Pclass Procss = Pclass();
