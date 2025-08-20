#include "Ext_var.h"
  
Sclass::Sclass()
{}

void RTD_TEMP_CAL_1();
Ticker TEMP_UPDATE_1(RTD_TEMP_CAL_1, 131, 0); //100 to 150
//*********************************************Temp**********************************************//
#define Rb1 100.0
#define Ra1 4700.0
#define Rc1 4700.0
#define Vs1 5
#define Vb1 (Rb1*Vs1)/(Rb1+Ra1)
#define Alpha  0.00385

int avrage_val_1 = 25, sample_1;
float TEMP_DEG_RAW_1 = 0.0, TEMP_FAH_RAW_1 = 0.0;
float Vin1=0, Rt1=0, Rx_1, TEMP_UPDATETD_1;
float  TotalVout_1 = 0, AvgVout_1 = 0;

void Sclass:: Setup()
{  
  TEMP_UPDATE_1.start();
}

void RTD_TEMP_CAL_1()
{
    sample_1++;
    AvgVout_1 = analogRead(A0);
    TotalVout_1  += AvgVout_1 ; 
    if(sample_1 >= avrage_val_1)
    {
      TotalVout_1 /= avrage_val_1;
      float Vout1 = ((TotalVout_1 * Vs1) / 1023.0);          // Voltage conversion
      Vin1 = Vout1 / 10;                                     // Gain
      Rt1 = ((Vb1 + Vin1) * Rc1) / (Vs1 - (Vb1 + Vin1));     //   Va = Vb + Vin 
      TEMP_DEG_RAW_1 = ((Rt1 / Rb1) - 1) / (Alpha);          // alpha is temp coefficient for paltinum
      sample_1 = 0;  AvgVout_1 = 0;   
    }
}

void Sclass :: Rtd_Temp_Update_1()  //-------------------------Pt100 Temp ticker update
{  
  TEMP_UPDATE_1.update(); 
}

Sclass Safety_probe = Sclass();
