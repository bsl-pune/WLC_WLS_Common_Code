#include "Ext_var.h"

Mclass::Mclass()
{}

void RTD_TEMP_CAL();
Ticker TEMP_UPDATE(RTD_TEMP_CAL, 125, 0); //100 to 150
//*********************************************Temp**********************************************//
float TEMP_DEG_RAW = 0.0, TEMP_FAH_RAW = 0.0;
float TEMP_FAH, TEMP_DEG = 0.0;
//----------------------------------------------------------------------------------------------//
#define Rb 100.0
#define Ra 470.0
#define Rc 470.0
#define Vs 5

#define Vb (Rb*Vs)/(Rb+Ra)
#define Alpha  0.00385
float R1 = 120.0; 
float R2 = 120.0; 
float R3 = 120.0;
float Vin=0, Rt=0, Rx, TEMP_UPDATETD;
float  TotalVout = 0, AvgVout = 0;
int avrage_val = 1, sample;
//----------------------------------------------------------------------------------------------//
float  TEMPERARY_SAVED_VARIABLE_FOR_CAL1_FAH, TEMP_CAL2_RAW_LOW_VALUE, TEMP_CAL2_RAW_HIGH_VALUE; 
int TEMPERARY_SAVED_VARIABLE_FOR_CAL1_DEG;
//**********************************************************************************************//
bool ERROR_SET_IN_DEG_FLAG=0, ERROR_SET_IN_FAH_FLAG=0;
float TEMP_DEG_RAW2, TEMP_FAH_RAW2;
//**********************************************************************************************//

void Mclass:: Setup()
{  
  TEMP_UPDATE.start();
}

void RTD_TEMP_CAL()
{
    sample++;
    AvgVout = analogRead(A1);
    TotalVout  += AvgVout ;  
    if(sample >= avrage_val)
    {
      TotalVout /= avrage_val;
      float Vout = ((TotalVout * Vs) / 1023.0);         // Voltage conversion
      Vin = Vout / 10;                                  // Gain
      Rt = ((Vb + Vin) * Rc) / (Vs - (Vb + Vin));       // Va = Vb + Vin 
      TEMP_DEG_RAW = ((Rt / Rb) - 1) / (Alpha);         // Alpha is temp coefficient for paltinum
//      Serial2.println("Product Temp."); //;Serial2.println("\t");
      Serial2.print(TEMP_DEG_RAW); Serial2.print("\t"); Serial2.println(TEMP_DEG_RAW_1);
      TEMP_FAH_RAW = (( TEMP_DEG_RAW * 1.8 ) + 32);
      sample = 0;   AvgVout = 0;   
    }
      avrage_val = 50;
}

void Mclass :: RTD_TEMP_CONV()    //-------------------------PT100 temp read
{
    if(SIGN) {ERROR_CAL =(ERROR_CAL1/10) * -1;}
    else     {ERROR_CAL =(ERROR_CAL1/10) ;}
    
    if(ERROR_SET_IN_DEG_FLAG)           TEMP_DEG_RAW2  = TEMP_DEG_RAW   + ERROR_CAL;
    else  if(ERROR_SET_IN_FAH_FLAG)     TEMP_FAH_RAW2  = TEMP_FAH_RAW   + ERROR_CAL;
    
    if (!CAL_SELECT_VALUE)
    {
        if(ERROR_SET_IN_DEG_FLAG)                         
        {
          TEMP_DEG  =   TEMP_DEG_RAW2;
          TEMP_FAH  =  (( TEMP_DEG_RAW2 * 1.8 ) + 32);
        }
        else if(ERROR_SET_IN_FAH_FLAG)  
        {
          TEMP_DEG  =  (( TEMP_FAH_RAW2 - 32) / 1.8);
          TEMP_FAH  =  TEMP_FAH_RAW2;
        }
    }
    else
    {
        CAL2_RAW_LOW_VALUE = (CAL2_RAW_LOW_VALUE_CONV / 10.0);
        CAL2_RAW_HIGH_VALUE = (CAL2_RAW_HIGH_VALUE_CONV  / 10.0);
        MASTER_LOW =  (MASTER_LOW_CONV  / 10.0);
        MASTER_HIGH = (MASTER_HIGH_CONV  / 10.0);
        TEMP_DEG = mapFloat( TEMP_DEG_RAW, CAL2_RAW_LOW_VALUE, CAL2_RAW_HIGH_VALUE, MASTER_LOW, MASTER_HIGH);
        TEMP_FAH  = ( TEMP_DEG * 1.8 ) + 32;
        if(TEMP_DEG>=100) TEMP_DEG = 99.9; 
   }
}

void Mclass :: Rtd_Temp_Update()  //-------------------------PT100 Temp ticker update
{  
  TEMP_UPDATE.update(); 
}

Mclass Temp = Mclass();
