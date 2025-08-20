
#ifndef Eeprom_h
#define Eeprom_h

class Rclass
{
   public :
   Rclass();
   void Setup();
   void EEPROM_CLEAR();
   void Process_Set_Values_Store_On_Eeprom();
   void Process_Time_Set_On_Eeprom();
   void Save_Pump_Data();
};

extern Rclass Eeprom;
#endif
