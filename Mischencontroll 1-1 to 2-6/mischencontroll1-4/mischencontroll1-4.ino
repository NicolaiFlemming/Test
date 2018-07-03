#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//benoetigte Bibliotheken

#define DOUT  3
#define CLK  2
//fuer HX711 Inputs

HX711 scale(DOUT, CLK);

float calibration_factor = -92450; //Kalbirationsfaktor fuer die Waage

int RelaisAlc = 4;  //Definieren des Ports fuer Relais 1
int RelaisMix = 5;  //Definieren des Ports fuer Relais 2
int VolAlc = 50-20; //50ml als experimentalwert. wird später durch eingabewert ersetzt. 20 ml Nachlauf nachdem Pumpe ausgestellt wurde
int VolMix = 70-20; //70ml als experimentalwert. wird später durch eingabewert ersetzt. 20 ml Nachlauf nachdem Pumpe ausgestellt wurde  

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LCD-Verbindungen

float weight;   //erklären der Variable, welche die Waage nutzt
int weightP;  //Nachkommastellen loswerden, durch int


//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------


void setup() {
  
  Serial.begin(9600);  
  Serial.println("T druecken, um zu tarieren");
  scale.set_scale(calibration_factor); //kalibriert 2018-05-03
  scale.tare(); //erste Kalibrierung

  lcd.begin(16,2); //Ausgabe LCD-Bildschirm
  lcd.backlight();
  lcd.setCursor(0,0);  //Definieren wo auf LCD-Bildschirm Geschrieben wird
  lcd.print("Gewicht:");  

  delay(2000); //benoetigte Verzoegerung fuer tarieren
  scale.tare();  //2. Kalibrierung. scheint aus irgendeinem <Grund besser zu funktionieren wenn man es anstellt 

  digitalWrite(RelaisAlc, LOW);  
  digitalWrite(RelaisMix, LOW);  //Ausstellen der Relais, eventl. an anfang der setup sektion setzen ??
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------


void loop() 
{
  weight = (scale.get_units()*1000); //Umwandlung in Gramm
  weightP = weight;  //definieren der variable fuer Gewicht 
  lcd.setCursor(0,1);
  lcd.print(weightP); //Ausgabe des Gewichts auf dem LCD Bildschirm
  lcd.print(" g  "); //Einheit
  
   if( VolAlc < weightP ) //Schleife fuer Relais 1 auf Pin4
    {
    digitalWrite(RelaisAlc, HIGH);
    }
  else if(RelaisAlc==HIGH)
    {
    digitalWrite(RelaisAlc, LOW); 
    }

  if( VolMix + (VolAlc+20) < weightP) //Schleife fuer Relais 2 Auf Pin5
  {
    digitalWrite(RelaisMix, HIGH);
  }
  else if(RelaisMix==HIGH)
  {
   digitalWrite(RelaisMix, LOW); 
  }


   if(Serial.available()) //Bei Input von T -> tarieren
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'T')
      scale.tare();  //Befehl zum Tarieren wenn der void loop schon begonnen hat
   }
   
  delay(10); //Verzoegerung  //Verzoegerung des gesamten Loops
}
