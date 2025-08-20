/* 
 *  Edit : Siddhesh Hadawale
 *  Project Description: Low cost waterbath.
 *  Date : 18/07/2024
 *  V4.2 is a common code of WLC & WLS.
 *  Change contactor & SSR pinout.   
 *  This code is for ring heaters only. 
 *  WLC020 unit used ( 1000 X 2 ) = 2000W ring heater.
 *  This code is using All in one PCB V1.2 only.
 */

#include "Arduino.h"
#include "Ext_var.h"

void Probe_Error();
void VerDisp();
Ticker VersionDisplay(VerDisp, 1000, 1);

//*************************************************Buuton***************************************************//
OneButton button1(MODE_KEY,  true);
OneButton button2(UP_KEY,    true);
OneButton button3(DOWN_KEY,  true);
OneButton button4(RST_KEY,   true);

int8_t ENTER_BUT = 0, Mode_Button_Single_Press_In_Settings = 0, LONG_PRESS = 0, INC5 = 0, TIME_WASTE_FLAG=0;
int FOR_REP=0, FOR_REP2=0, REP=0, REP1=0;
bool StopVersionDisplay=1;
bool but1 = 0, Service_Menu=0;
bool PUMP_FLAG=0;
bool toggle =0;
unsigned long start_tt =0;

void toggle_fun();
Ticker toggle_ticker(toggle_fun, 1000, 0, MILLIS);  //Time_speed_show_toggle
void toggle_fun()
{    
   toggle == 0 ? toggle = 1 : toggle = 0;   
}

void setup()
{
  Serial2.pins(38, 39);
  Serial2.begin(9600);
  Butn.SETUP();
  led.SETUP();  
  Pid.Setup();
  Procss.Setup();
  Temp.Setup();
  Safety_probe.Setup();
  Eeprom.Setup(); 
  toggle_ticker.start(); 
  
  pinMode(MODE_KEY, INPUT_PULLUP);
  pinMode(UP_KEY,   INPUT_PULLUP);
  pinMode(DOWN_KEY, INPUT_PULLUP);
  pinMode(RST_KEY,  INPUT_PULLUP);
 
  button1.attachClick(MODE_CLICK);                       //single press
  button1.attachLongPressStart(MODE_LONG_PRESS_START);   //long press
  
  button2.attachClick(INC_CLICK);                        //inc button single press
  button2.attachDuringLongPress(INC_LONGPRESS);          //inc button when long press
  button2.attachLongPressStop(INC_LONGPRESS_STOP);
  
  button3.attachClick(DEC_CLICK);
  button3.attachDuringLongPress(DEC_LONGPRESS);
  button3.attachLongPressStop(DEC_LONGPRESS_STOP);
  
  button4.attachClick(RESET_CLICK);
  button4.attachLongPressStart(RESET_LONG_PRESS_START);

  VersionDisplay.start(); 
}

void loop()
{ 
  toggle_ticker.update();
//  Serial2.println("siddhesh");
  led.DISP_UPDATE();                       //----------------------display write update
  // delayMicroseconds(1000);
  Temp.RTD_TEMP_CONV();  
  if(StopVersionDisplay)                   //----------------------version display
  {  
    VersionDisplay.update();  
    Tick.BUZZ_INTERVAL(25);                //---------------------Buzzz on
    led.DISPLAY_DIGIT2(34, 24, 24, 24) ;   //----------------------V
    led.DISPLAY_DIGIT1(24, 4, 0, 2) ;      //----------------------4.02 
    DOT1_1=1;                              //----------------------dot on                       
  }
  else 
  {
      VersionDisplay.stop();
     
      if(TEMP_DEG_RAW >=120)     
      {   
        digitalWrite(HEATER, LOW);
        Probe_Error();                 //-----------------------Temp error detection loop(Without Probe PCB Working)113
      }
      else
      { 
         but_check();
         if (LONG_PRESS )    Procss.SET_LONG_PRESS_FUNCTION();  //-----------------------Settings
         else                Procss.SET_SHORT_PRESS_FUNCTION(); //-----------------------To set Process
      } 
  } 
}

void Pclass:: toggle_ticker_update()                       //------------------process file one time execution in setup
{
  toggle_ticker.update();
}

void VerDisp()
{
  StopVersionDisplay=0;        //-----------------------To stop version display  
}

void Probe_Error()
{
   DOT1_1= DOT1_0=DOT1_2= DOT2_1= DOT2_2= DOT2_0=0;  
   ENTER_BUT =  LONG_PRESS =  PID_SETPOINT = Mode_Button_Single_Press_In_Settings=0;
   digitalWrite(TIMER_LED1, LOW);            // -------------------------- : off
   digitalWrite(TIMER_LED2, LOW);            // -------------------------- : off
   
   if(toggle==1)
   {
     digitalWrite(BUZZER, HIGH);
   led.DISPLAY_DIGIT2(11, 17, 23, 14) ;      // -------------------------- : PrbE
   led.DISPLAY_DIGIT1(15, 17, 23, 17) ;      // -------------------------- : Erpr
   }
   if(toggle==0)
   {
     digitalWrite(BUZZER, LOW);
   led.DISPLAY_DIGIT2(24, 24, 24, 24) ;      // -------------------------- : blank
   led.DISPLAY_DIGIT1(24, 24, 24, 24) ;      // -------------------------- : blank
   }
//   Tick.BUZZ_INTERVAL2(1000);                // -------------------------- : 1sec on
//   Tick.BUZZ_INTERVAL1(2000);                // -------------------------- : 1sec off
//   if(REP == 0)                              // -------------------------- : To start tick only oncec
//   {
//     Tick.BUZZ_TICK1_Start();                  
//     REP++;
//   }
}

void MODE_CLICK()
{
  if(checkTempRaised==0)
  {
  BLINK_STATS=0;                                                        //-----------------------To blink start parameter
  if (!LONG_PRESS )                                                     //-----------------------no longpress (out of settings)
  {
    i = 0;
         
    if (ENTER_BUT == 4 && SET_MIN == 0 ) ENTER_BUT = 4;                 //-----------------------when selected time is zero, process should not start
    else if(DEGREE_VALUE_SET && !ENTER_BUT)                             //-----------------------at home page and selection of parameter in fah  
    {
//       MIN_SET_FAH =   TEMP_FAH + 15 ; //12                           //-----------------------Level sensing start minimu limit of temp
       ENTER_BUT = 2;
    }
    else   ENTER_BUT++;                                                 //-----------------------single press increment variable for homepage
       
  }
  else                                                                  //-----------------------in settings         
  {
    if (!Mode_Button_Single_Press_In_Settings)                          //-----------------------not press mode button (while scrolling Parameter Passed to Array)
    {
      SET_LONG_VALUE[Stir_ON_OFF] = STIR_VALUE_SET; 
      SET_LONG_VALUE[Unit_C_F]    = DEGREE_VALUE_SET;
      SET_LONG_VALUE[Auto_Resume_Enable]    = Proceess_Resume_Enable_Flag;
      SET_LONG_VALUE[Slow_Fast_Heating]    =  select_heating_type; // New feature added
      SET_LONG_VALUE[Product_Selection_Parameter]    = select_product_type; //Siddhesh
      
      if (!DEGREE_VALUE_SET)    SET_LONG_VALUE[Safe_Value] = SAFE_VALUE_SET_DEG;
      else                      SET_LONG_VALUE[Safe_In_Fah] = SAFE_VALUE_SET_FAH;
      
      SET_LONG_VALUE[Calibration_Selection] = CAL_SELECT_VALUE;
      if (!DEGREE_VALUE_SET)
      {
        if(TEMP_DEG_RAW<100)
        {
          SET_LONG_VALUE[Cal1_Decimal] = TEMP_DEG_RAW; //TEMP_DEG;                    //CAL1_VALUE_SAVED_DEG1;
          TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG =  TEMP_DEG_RAW * 10;
          int CONV_TEMP = (TEMP_DEG_RAW * 10);
          SET_LONG_VALUE[Cal1_Point] = ( CONV_TEMP  % 10) ;        //CAL1_VALUE_SAVED_DEG2 ;
        }
        else
        {
          SET_LONG_VALUE[Cal1_Decimal]=99; 
          SET_LONG_VALUE[Cal1_Point] = 9;
        }
      }
      else
      {
        SET_LONG_VALUE[Cal1_In_Fah] =  TEMP_FAH_RAW ;  // SET_LONG_VALUE[9]  =CAL1_VALUE_SAVED_FAH;
        TEMPERARY_SAVED_VARIABLE_FOR_CAL1_FAH = TEMP_FAH_RAW;
      }
      SET_LONG_VALUE[Cal2LowTemp_Set_Display]  = LOW_CAL_VALUE_SET ;
      SET_LONG_VALUE[Cal2LowDecimalTemp_Enter_Heat]  =  TEMP_DEG_RAW; //TEMP_DEG ;
      
      int CONV = (TEMP_DEG_RAW * 10);
      SET_LONG_VALUE[Cal2LowPointTemp_Enter_Maintain]  =  CONV % 10 ;
      
      if(SELECTION_PARAMETER == Cal2LowPointTemp_Enter_Maintain )      TEMP_CAL2_RAW_LOW_VALUE = TEMP_DEG_RAW;
      if(SELECTION_PARAMETER == Cal2HighPointTemp_Enter_Maintain)      TEMP_CAL2_RAW_HIGH_VALUE = TEMP_DEG_RAW;

      if(TEMP_DEG_RAW<100)
      {
        SET_LONG_VALUE[Cal2HighDecimalTemp_Enter_Heat] = TEMP_DEG_RAW;   //TEMP_DEG;
        int CONV2 = (TEMP_DEG_RAW * 10);
        SET_LONG_VALUE[Cal2HighPointTemp_Enter_Maintain]  = ( CONV2 % 10 ) ;
      }
      else
      {
          SET_LONG_VALUE[Cal2HighDecimalTemp_Enter_Heat]  = 99; 
          SET_LONG_VALUE[Cal2HighPointTemp_Enter_Maintain] = 9;
      }            
    
      SET_LONG_VALUE[Time_Mode] = TIME_SOCK_OR_TOTAL; //13
      if(SELECTION_PARAMETER == FactoryAndSure_Reset )  currentMillis=0;
      
      if (!DEGREE_VALUE_SET && SELECTION_PARAMETER == Cal1_Point)   SELECTION_PARAMETER--;
      
      Mode_Button_Single_Press_In_Settings = 1;
    }
    else
    {
      switch (SELECTION_PARAMETER)
      {
        case Stir_ON_OFF :
          STIR_VALUE_SET = SET_LONG_VALUE[Stir_ON_OFF];
          EEPROM.write(0, STIR_VALUE_SET);  //Siddhesh
          break;
          
        case Unit_C_F :
          DEGREE_VALUE_SET = SET_LONG_VALUE[Unit_C_F];
          EEPROM.write(1, DEGREE_VALUE_SET);
          break;
          
        case Safe_Value :
          if (!DEGREE_VALUE_SET) 
          {
            SAFE_VALUE_SET_DEG = SET_LONG_VALUE[Safe_Value];
            EEPROM.write(2, SAFE_VALUE_SET_DEG);
          }
          else            
          {
            SAFE_VALUE_SET_FAH = SET_LONG_VALUE[Safe_In_Fah];
            EEPROM.put(3, SAFE_VALUE_SET_FAH);
          }
          break;
          
        case Auto_Resume_Enable :
             Proceess_Resume_Enable_Flag = SET_LONG_VALUE[Auto_Resume_Enable];
             EEPROM.write(15, Proceess_Resume_Enable_Flag);

        case Product_Selection_Parameter :
          select_product_type = SET_LONG_VALUE[Product_Selection_Parameter];
          EEPROM.write(150, select_product_type);                            //Siddhesh EEPROM.WRITE
          break;

        case Slow_Fast_Heating :
          select_heating_type = SET_LONG_VALUE[Slow_Fast_Heating];
          EEPROM.write(132, select_heating_type);                            // New feature added
          break;
            
        case Calibration_Selection :
          CAL_SELECT_VALUE = SET_LONG_VALUE[Calibration_Selection];
          EEPROM.write(5, CAL_SELECT_VALUE);
          break;
          
        case Cal1_Point :
          if (!DEGREE_VALUE_SET)
          {
            CAL1_VALUE_SAVED_DEG1 = SET_LONG_VALUE[Cal1_Decimal]; // EEPROM.write(6, CAL1_VALUE_SAVED_DEG1);
            CAL1_VALUE_SAVED_DEG2 = SET_LONG_VALUE[Cal1_Point];  //EEPROM.write(7, CAL1_VALUE_SAVED_DEG2);
            ERROR_CAL = ((( CAL1_VALUE_SAVED_DEG1 + (  CAL1_VALUE_SAVED_DEG2 / 10.0)) * 10) - TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG) / 10; //f; 
             ERROR_SET_IN_DEG_FLAG=1;  ERROR_SET_IN_FAH_FLAG=0;
          }
          else
          {
            CAL1_VALUE_SAVED_FAH = SET_LONG_VALUE[Cal1_In_Fah];  //EEPROM.put(8, CAL1_VALUE_SAVED_FAH);
            ERROR_CAL = SET_LONG_VALUE[20] -  TEMPERARY_SAVED_VARIABLE_FOR_CAL1_FAH;   
            ERROR_SET_IN_DEG_FLAG=0; ERROR_SET_IN_FAH_FLAG=1;
          }   
          if(ERROR_CAL<0)  { SIGN=1;  ERROR_CAL1 = (( ERROR_CAL *10 )* (-1));}
          else             { SIGN=0;  ERROR_CAL1 = ERROR_CAL *10;}
          EEPROM.put(70, ERROR_CAL1);  
          EEPROM.write(7, SIGN);      
          EEPROM.write(8, ERROR_SET_IN_DEG_FLAG);
          EEPROM.write(9, ERROR_SET_IN_FAH_FLAG);         
          break;
          
        case Cal2LowTemp_Set_Display :
          LOW_CAL_VALUE_SET   =  SET_LONG_VALUE[Cal2LowTemp_Set_Display];
          SET_LONG_VALUE[Cal2HighTemp_Set_Display]   = (SET_LONG_VALUE[Cal2LowTemp_Set_Display] + 10);
          break;
          
        case Cal2HighTemp_Set_Display :
             HIGH_CAL_VALUE_SET  = SET_LONG_VALUE[Cal2HighTemp_Set_Display];
             TEMP_CAL2_RAW_LOW_VALUE = TEMP_DEG_RAW;
             break;
             
        case Cal2LowPointTemp_Enter_Maintain : 
             TEMP_CAL2_RAW_HIGH_VALUE = TEMP_DEG_RAW;
             break;
             
        case Cal2HighPointTemp_Enter_Maintain :
              CAL2_RAW_LOW_VALUE_CONV  = ( TEMP_CAL2_RAW_LOW_VALUE*10);
              CAL2_RAW_HIGH_VALUE_CONV =( TEMP_CAL2_RAW_HIGH_VALUE *10);              
              EEPROM.put(80, CAL2_RAW_LOW_VALUE_CONV);
              EEPROM.put(90, CAL2_RAW_HIGH_VALUE_CONV);
              MASTER_LOW_CONV  = (CAL2_CALCULATED_LOW_CAL_VALUE *10);
              MASTER_HIGH_CONV = (CAL2_CALCULATED_HIGH_CAL_VALUE *10);
              EEPROM.put(100, MASTER_LOW_CONV);
              EEPROM.put(110, MASTER_HIGH_CONV);   
              break;
                         
        case Time_Mode :
              TIME_SOCK_OR_TOTAL = SET_LONG_VALUE[Time_Mode];
              EEPROM.put(65, TIME_SOCK_OR_TOTAL);
              break; 
                     
        default :
          break;
      }
      if ((!DEGREE_VALUE_SET && SELECTION_PARAMETER == Cal1_Decimal)  ||
            SELECTION_PARAMETER == Cal2LowTemp_Set_Display || 
            SELECTION_PARAMETER == Cal2LowDecimalTemp_Enter_Heat  || 
            SELECTION_PARAMETER == Cal2HighDecimalTemp_Enter_Heat ||  
            SELECTION_PARAMETER == FactoryAndSure_Reset ) 
      {
        Mode_Button_Single_Press_In_Settings = 1;
        SELECTION_PARAMETER++;
      }
      else if (SELECTION_PARAMETER == Cal2HighTemp_Set_Display  ||
               SELECTION_PARAMETER == Cal2LowPointTemp_Enter_Maintain || 
               SELECTION_PARAMETER == Cal2HighPointTemp_Enter_Maintain)        
      {
        Mode_Button_Single_Press_In_Settings = 0;
        SELECTION_PARAMETER++;
      }
      else        Mode_Button_Single_Press_In_Settings = 0;

    }
    i =VALUE_FOR_BLINK +1;
  }
   if (( ENTER_BUT >= 5) && select_product_type ) //Product Selections
   {
      ENTER_BUT = 5;  
      if(PUMP_FLAG)   PUMP_FLAG=0;
      else            PUMP_FLAG =1;
      Eeprom.Save_Pump_Data(); //Siddhesh
   }
   
   if(ENTER_BUT >= 5 && REP1<1) { Tick.BUZZ_INTERVAL(1000);  REP1++; }  //long pree
   else                           Tick.BUZZ_INTERVAL(50);               //short pree   
}
}

void MODE_LONG_PRESS_START()
{ 
  if(checkTempRaised==0)
  {
  if (!ENTER_BUT)
  {
    if (!LONG_PRESS )      //------------------at home page  user setting
    {
      i = 350;
      if (select_product_type ) SELECTION_PARAMETER = Stir_ON_OFF;
      else                      SELECTION_PARAMETER = Unit_C_F;
      LONG_PRESS = 1;      //------------------go to menu //Siddhesh
      RST_BUTT = 1;
    }   
  }
 Tick.BUZZ_INTERVAL(200); 
}
}

void Bclass :: Button_Tick_Update()
{
  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();  
}

void INC_CLICK()
{ 
  if(checkTempRaised==0)
  {
  if (LONG_PRESS && !Mode_Button_Single_Press_In_Settings )
  {    
    if(!Service_Menu)
    {
      SELECTION_PARAMETER--;                              //------------------- scroll down menu
      if(select_product_type )
      {
         if(SELECTION_PARAMETER < Stir_ON_OFF) SELECTION_PARAMETER = Auto_Resume_Enable;
      }
      else 
      {
        if(SELECTION_PARAMETER < Unit_C_F) SELECTION_PARAMETER = Auto_Resume_Enable;        
      }
    }
    else
    {
      if ( SELECTION_PARAMETER == Cal1_Point ) 
                              SELECTION_PARAMETER = Calibration_Selection;
                              
      else if (SELECTION_PARAMETER == Cal2LowDecimalTemp_Enter_Heat ) 
                                SELECTION_PARAMETER = Cal2LowTemp_Set_Display;
                                
      else if(SELECTION_PARAMETER == Log)   
                                SELECTION_PARAMETER = Slow_Fast_Heating;
                                                         
      else if(SELECTION_PARAMETER == FactoryAndSure_Reset)   
                                SELECTION_PARAMETER = Log;
                                
      else if(SELECTION_PARAMETER == Slow_Fast_Heating)   
                                SELECTION_PARAMETER = Cal2LowTemp_Set_Display;  
                                                              
      else if (SELECTION_PARAMETER < Calibration_Selection)
       SELECTION_PARAMETER = FactoryAndSure_Reset;  
      else                      SELECTION_PARAMETER--;      
          
    }
  }  
  if (!LONG_PRESS  || (LONG_PRESS && Mode_Button_Single_Press_In_Settings ))    Butn.SET_CHANGE_UP();
}
}

void INC_LONGPRESS()
{
  if(checkTempRaised==0)
  {
  if(!but1)
  {
    BLINK_STATS=1;
    INC5++;
    if (INC5 >= 25)
    {
      Butn.SET_CHANGE_UP();
      INC5 = 0;
    }
  }
}
}

void INC_LONGPRESS_STOP()
{
  if(checkTempRaised==0)
  {
  i = 0;
  BLINK_STATS=0;
  }
}

void DEC_CLICK()
{ 
  if(checkTempRaised==0)
  {
  if (LONG_PRESS && !Mode_Button_Single_Press_In_Settings )
  {
    if(!Service_Menu){
        SELECTION_PARAMETER++;

      if(select_product_type )
      {
         if (SELECTION_PARAMETER > Auto_Resume_Enable)  SELECTION_PARAMETER = Stir_ON_OFF;
      }
      else 
      {
         if (SELECTION_PARAMETER > Auto_Resume_Enable)   SELECTION_PARAMETER = Unit_C_F;        
      }
    }
    else 
    {
      if (SELECTION_PARAMETER == Calibration_Selection )     
                   SELECTION_PARAMETER = Cal1_Point;
      else if (SELECTION_PARAMETER == Cal2LowTemp_Set_Display  &&  CAL2_START )      
                      SELECTION_PARAMETER = Cal2LowDecimalTemp_Enter_Heat;
      else if (CAL2_START == 0 && SELECTION_PARAMETER >= Cal2LowTemp_Set_Display && SELECTION_PARAMETER <= Cal2_Complete_Display  )  
                       SELECTION_PARAMETER = Slow_Fast_Heating;
      else if(SELECTION_PARAMETER ==  Slow_Fast_Heating)            
              SELECTION_PARAMETER = Log;                  
      else if(SELECTION_PARAMETER ==  Log)            
              SELECTION_PARAMETER = FactoryAndSure_Reset;       
      else if(SELECTION_PARAMETER >=  FactoryAndSure_Reset)            
              SELECTION_PARAMETER = Product_Selection_Parameter;
      else                                                 
             SELECTION_PARAMETER++;             
    }
  }
  if (!LONG_PRESS  || (LONG_PRESS && Mode_Button_Single_Press_In_Settings ))       Butn.SET_CHANGE_DOWN();
}
}

void DEC_LONGPRESS()
{ 
  if(checkTempRaised==0)
  {
  if(!but1)
  {
    BLINK_STATS=1;
    INC5++;
    if (INC5 >= 25)
    {
      Butn.SET_CHANGE_DOWN();
      INC5 = 0;
    }
  }
}
}

void DEC_LONGPRESS_STOP()
{
  if(checkTempRaised==0)
  {
  i = 0;
  BLINK_STATS=0;
  }
}

void but_check()//--------------------------------------------------------------------------UP DOWN Key Long Press detection
{
  if (digitalRead(UP_KEY) == LOW && digitalRead(DOWN_KEY) == LOW && but1 == 0) //----------Check for button press
  {
    start_tt = millis();
    but1 = 1;
  }
  else if (digitalRead(UP_KEY) == LOW && digitalRead(DOWN_KEY) == LOW && but1 == 1) //--------Check for button is continously
  {
    if ((millis() - start_tt) >= 3000)                                              //-------------------after 3 sec completion
    {
//      lock_set();//------------Go to lock settings mode.
     Tick.BUZZ_INTERVAL(500);  
      if (!ENTER_BUT && !LONG_PRESS )     
      {
          i = 350;
          LONG_PRESS = 1; 
          RST_BUTT = 1;
          Service_Menu=1;
          SELECTION_PARAMETER = Product_Selection_Parameter;   
      }
    }
  }
  else if (but1 == 1)
  {
    if (digitalRead(UP_KEY) == HIGH || digitalRead(DOWN_KEY) == HIGH)
    {
      but1 = 0;
    }
  }
}

void RESET_CLICK()
{
  if(checkTempRaised==0)
  {
   if (!DEGREE_VALUE_SET && SELECTION_PARAMETER == Cal1_Point  || 
        SELECTION_PARAMETER == Cal2LowPointTemp_Enter_Maintain || 
        SELECTION_PARAMETER == Cal2HighPointTemp_Enter_Maintain ) 
        SELECTION_PARAMETER--;
   else
   {
      if (SELECTION_PARAMETER == Cal2LowDecimalTemp_Enter_Heat    || 
          SELECTION_PARAMETER == Cal2HighDecimalTemp_Enter_Heat )    
          Mode_Button_Single_Press_In_Settings =1;
      else
      {
        if(Mode_Button_Single_Press_In_Settings) Mode_Button_Single_Press_In_Settings=0;
      }
   }
  if (!LONG_PRESS )
  {
    if(DEGREE_VALUE_SET && ENTER_BUT==2)   ENTER_BUT=0;
    else if (ENTER_BUT >= 1 && ENTER_BUT <= 4)  ENTER_BUT--;
   if ( ENTER_BUT == 5   &&  RATE_OF_HEATING >= 1  && CHECK_RATE_OF_HEATING == 1 )
   {
     RATE_OF_HEATING =0;
     CHECK_RATE_OF_HEATING =1;
   }
  }
   Tick.BUZZ_INTERVAL(50); 
}
}

void RESET_LONG_PRESS_START()
{
  if(checkTempRaised==0)
  {
  if (!LONG_PRESS )   
  {
  if(!ENTER_BUT)
  {
    RST_BUTT = 0;    
    LONG_PRESS = 0;
    SELECTION_PARAMETER = Unit_C_F;// 0;
  }
  else
  {
      ENTER_BUT = 0;
      SET_MIN = 0;
  }
  }
  if (LONG_PRESS)//&& !Mode_Button_Single_Press_In_Settings ) //07/08/2023 Siddhesh
  {
    if(Service_Menu) Service_Menu=0;
    LONG_PRESS = 0;
  }
 Tick.BUZZ_INTERVAL(200); 
}
}
