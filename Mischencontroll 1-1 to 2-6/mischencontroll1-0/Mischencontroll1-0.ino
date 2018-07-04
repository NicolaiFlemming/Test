#include "HX711.h"
 
#define DOUT  3
#define CLK  2
 
HX711 scale(DOUT, CLK);
 

float calibration_factor = -92450; //Kalibrierungsfaktoir für unsere Load Cell aus 
 
//=============================================================================================
//=============================================================================================
void setup() {
  Serial.begin(9600);  
  Serial.println("Press T to tare");
  scale.set_scale(calibration_factor);  //Kalibrierungsfaktor aus scale_setup.ino
  scale.tare();             //Tarieren der Waage  
}
 
//=============================================================================================
//=============================================================================================
void loop() {
  Serial.print("Weight: ");
  Serial.print((scale.get_units()*1000), 0);  //Bis zu 3 Dezimal stellen 
  Serial.println(" g");
 
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'T')
      scale.tare();  //bei eingabe von 't' oder 'T' auf dem seriellen Monitor wird die waage tariert      
  }
}
//=============================================================================================

