/*
 Arduino 
 pin 
 2 -> HX711 CLK
 3 -> DOUT
 5V -> VCC
 GND -> GND

 Entschuldigung, dass so viel auf Englisch ist. Ist nunmal meine 2. Muttersprache
 */
#include "HX711.h"  
 
#define DOUT  3
#define CLK  2
 
HX711 scale(DOUT, CLK);
 
//Diesen Kalibrierungsfaktor je nach bedarf anpassen bis die ausgegebenen werte stimmen . Ist für jede load cell anders 
float calibration_factor = -96650;
 
//=============================================================================================
//=============================================================================================
void setup() {
  Serial.begin(9600);
  Serial.println("HX711 Calibration");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press a,s,d,f to increase calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press z,x,c,v to decrease calibration factor by 10,100,1000,10000 respectively");
  Serial.println("Press t for tare");
  scale.set_scale();
  scale.tare(); //Tarieren der Waage 
 
  //long zero_factor = scale.read_average(); //Grundwert für die Waage bekommen
  //Serial.print("Zero factor: "); //Kann benutzt werden um tarieren zu vermeiden
  //Serial.println(0);
}
 
//=============================================================================================
//=============================================================================================
void loop() {
 
  scale.set_scale(calibration_factor); //hier wird der Kalibrierungsfaktor eingfügt 
 
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 3);
  Serial.print(" kg");
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
 
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
    else if(temp == 's')
      calibration_factor += 100;  
    else if(temp == 'x')
      calibration_factor -= 100;  
    else if(temp == 'd')
      calibration_factor += 1000;  
    else if(temp == 'c')
      calibration_factor -= 1000;
    else if(temp == 'f')
      calibration_factor += 10000;  
    else if(temp == 'v')
      calibration_factor -= 10000;  
    else if(temp == 't')
      scale.tare();  //bei t tarieren
  }
}
//=============================================================================================

