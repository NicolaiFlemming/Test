#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//benoetigte Bibliotheken

#define DOUT  3
#define CLK  2      //fuer HX711 Inputs

HX711 scale(DOUT, CLK);

float calibration_factor = -92450; //Kalbirationsfaktor fuer die Waage

int RelaisAlc = 4;  //Definieren des Ports fuer Relais 1
int RelaisMix = 5;  //Definieren des Ports fuer Relais 2
int VolAlc = 100; //50ml als experimentalwert. wird später durch eingabewert ersetzt. 20 ml Nachlauf nachdem Pumpe ausgestellt wurde
int VolMix = 100; //70ml als experimentalwert. wird später durch eingabewert ersetzt. 20 ml Nachlauf nachdem Pumpe ausgestellt wurde  
char But = A0;  //Port für Button

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LCD-Verbindungen

float weight;   //erklären der Variable, welche die Waage nutzt
int weightP;  //Nachkommastellen loswerden, durch int

bool btn = false;   //Wert für button


//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------


void setup() {
  
  Serial.begin(9600);  
  Serial.println("T druecken, um zu tarieren");
  scale.set_scale(calibration_factor); //kalibriert 2018-05-03
  scale.tare(); //erste Kalibrierung

  pinMode(RelaisAlc, OUTPUT);
  pinMode(RelaisMix, OUTPUT);
  pinMode (But, INPUT);

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

  Serial.print("Weight: ");
  Serial.print((scale.get_units()*1000), 0);  //Up to 3 decimal points
  Serial.println(" g"); //Change this to kg and re-adjust the calibration factor if you follow lbs
  if(analogRead(But)>1000)
  {
  btn = true;
  }
  
  weight = (scale.get_units()*1000); //Umwandlung in Gramm
  weightP = weight;  //definieren der variable fuer Gewicht 
  lcd.setCursor(0,1);
  lcd.print(weightP); //Ausgabe des Gewichts auf dem LCD Bildschirm
  lcd.print(" g  "); //Einheit
  
  if( VolAlc > weightP & weightP > -10 & btn == true ) //Schleife fuer Relais 1 auf Pin4
  {
  digitalWrite(RelaisAlc, HIGH);
  }
  else
  {
  digitalWrite(RelaisAlc, LOW); 
  }

if( VolMix + VolAlc >= weightP & VolAlc < weightP & btn==true ) //Schleife fuer Relais 1 auf Pin4
  {
  digitalWrite(RelaisMix, HIGH);
  }
  else
  {
  digitalWrite(RelaisMix, LOW); 
  }

  if(weightP >= VolAlc + VolMix)
  {
    btn = false;
  }

 //beide Schleifen zusammen funktionieren noch nicht zusammen. einzeln haben sie beide schon funktioniert. wo ist der Denkfehler? Neue Schleife aufbauen ? 
 //While schleifen funktionieren hier nicht weil sie erst komplett abgearbeitet werden und man dann keine Kontinuierlichen Werte von der Waage zurück bekommt.
 //Kann man 2 loop funktionen (Void Loop () ) gleichzeitig laufen lassen? Eine fuer die waage und die andere fuer die Relais?  
   
   if(Serial.available()) //Bei Input von T -> tarieren
  {
    char temp = Serial.read();
    if(temp == 't' || temp == 'T')
      scale.tare();  //Befehl zum Tarieren wenn der void loop schon begonnen hat
   }
   
  delay(10); //Verzoegerung  //Verzoegerung des gesamten Loops
}
