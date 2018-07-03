#include "HX711.h"
#include <Wire.h>
//benoetigte Bibliotheken

#define DOUT  3
#define CLK  2
//fuer HX711 Inputs

HX711 scale(DOUT, CLK);

float calibration_factor = -92450; //Kalbirationsfaktor fuer die Waage

float weight;   //erklären der Variable, welche die Waage nutzt
int weightP;  //Nachkommastellen loswerden, durch int


//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------


void setup() 
{
  
  Serial.begin(9600);  
  Serial.println("T druecken, um zu tarieren");
  scale.set_scale(calibration_factor); //kalibriert 2018-05-03
  scale.tare(); //erste Kalibrierung

}


//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------


void loop() 
{
  weight = (scale.get_units()*1000); //Umwandlung in Gramm
  weightP = weight;  //definieren der variable fuer Gewicht 
  
   if(Serial.available()) //Bei Input von T -> tarieren
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'T')
      scale.tare();  //Befehl zum Tarieren wenn der void loop schon begonnen hat
   }
   
  delay(10); //Verzoegerung  //Verzoegerung des gesamten Loops
}
