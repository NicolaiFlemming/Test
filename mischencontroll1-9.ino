#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//benoetigte Bibliotheken

#define DOUT  3
#define CLK  2      //fuer HX711 Inputs

HX711 scale(DOUT, CLK);

float calibration_factor = -92450; //Kalibrierungsfaktor fuer die Waage

int RelaisAlc = 4;  //Definieren des Ports fuer Relais 1
int RelaisMix = 5;  //Definieren des Ports fuer Relais 2
char But = A0;      //Port für Button
char Pot = A5;      //Port für Potentiometer
char Pot2 =A9;      //Port für Potentiometer nr 2
//int a = 0;
int VolAlc;         //Volumen Alkohol
int VolMix;         //Volumen Mixgetränk
int VolGes;         //Volumen Gesamt
int AlcPerc;        //Prozentsatz Alkohol
int MixPerc;        //Prozentsatz Mischgetränk
float Ratio;        //Mischverhältnis
float PreRatio;     //Unmapped Mischverhältnis
float Vol;
float PreVol;


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LCD-Verbindungen

float weight;   //erklären der Variable, welche die Waage nutzt
int weightP;    //Nachkommastellen loswerden, durch int

bool btn = false;   //Wert für button


//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------


void setup() {

  Serial.begin(9600);                           //Seriellen Monitor mit 9600 Baud starten
  Serial.println("T druecken, um zu tarieren"); //Auf Seriellem Monitor schreiben
  scale.set_scale(calibration_factor);          //kalibriert 2018-05-03
  scale.tare();                                 //erste Kalibrierung

  pinMode(RelaisAlc, OUTPUT); //Definieren des Pins als Output 5V
  pinMode(RelaisMix, OUTPUT); //Definieren des Pins als Output 5V
  pinMode (But, INPUT);       //Definieren des Pins als Analog Input 8Bit
  pinMode (Pot, INPUT);       //Definieren des Pins als Analog Input 8Bit
  pinMode (Pot2, INPUT);      //Definieren des Pins als Analog Input 8Bit

  lcd.begin(16,2);        //Ausgabe LCD-Bildschirm
  lcd.backlight();        //Anstellen Hintergrundlicht LCD

  delay(2000); //benoetigte Verzoegerung

  digitalWrite(RelaisAlc, LOW);
  digitalWrite(RelaisMix, LOW);  //Relais Pins auf 0V stellen
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------------


void loop()
{
  //a = analogRead(But);
  //Serial.println(a);


  //Serial.print("Weight: ");
  //Serial.print((scale.get_units()*1000), 0);  //Up to 3 decimal points
  //Serial.println(" g"); //Change this to kg and re-adjust the calibration factor if you follow lbs

  if(analogRead(But)>1000) //bei Button drück Analog Wert > 1000
  {
  btn = true;     //wenn Knopf gedrückt wurde wird btn bool Variable mit true ueberschrieben
  scale.tare();   //bei knopfdruck tarieren
  delay(500);     //halbe sekunde verzoegerung
  }

if (btn == false) //Alles was passieren soll bevor der knopf gedrückt wird
{
  PreRatio = (constrain(analogRead(Pot), 0, 1023));     //analog werte des Potentiometers zwischen 0 und 1023 (8bit) beschraenken und auf PreRatio Variable schreiben
  Ratio = (PreRatio/1023);                              //Variable umrechnen zu einem Verhaeltnis für VolAlc und VolMix
  VolAlc = (VolGes * Ratio);                            //VolAlc mithilfe der Ratio Variable berechnen
  VolMix = (VolGes - VolAlc);                           //Volmix mithilfe von VolGes und VolAlc berechnen
  AlcPerc = (Ratio*101);                                //Prozentsatz Alkohol mithilfe der Ratio Variablen berechnen. 101 wegen rundungsfehlern
  MixPerc = ((1-Ratio)*101);                            //Prozentsatz Mischgetraenk mithilfe der Ratio Variablen berechnen. 101 wegen rundungsfehlern
  PreVol = (constrain(analogRead(Pot2), 0 , 1023));
  Vol = (PreVol/1023);
  VolGes = (100+(Vol)*400);


  lcd.setCursor(0,0);     //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
  lcd.print("Ratio:");    //Auf LCD-Bildschirm schreiben
  lcd.setCursor (10,0);
  lcd.print (AlcPerc);
  lcd.print ("/");
  lcd.print (MixPerc);
  lcd.print (" ");

  lcd.setCursor(0,1);     //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
  lcd.print("Volumen:");  //Auf LCD-Bildschirm schreiben
  lcd.setCursor (10,1);
  lcd.print (VolGes);
  lcd.print ("ml ");
}

  weight = (scale.get_units()*1000);    //einholen der werte von der Waage und Umwandlung in Gramm
  weightP = weight;                     //definieren der variable fuer Gewicht
if (btn==true)
{
  lcd.setCursor(0,1);     //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
  lcd.print("Gewicht:");  //Auf LCD-Bildschirm schreiben
  lcd.setCursor(10,1);
  lcd.print(weightP);                   //Ausgabe des Gewichts auf dem LCD Bildschirm
  lcd.print(" g  " );                   //Einheit
                     //Ausgabe des Verhaeltnisses auf dem LCD Bildschirm
}

  if( VolAlc > weightP & weightP > -10 & btn == true ) //Schleife fuer Relais Alkohol auf Pin4
  {
  digitalWrite(RelaisAlc, HIGH);
  }
  else
  {
  digitalWrite(RelaisAlc, LOW);
  }

if( VolMix + VolAlc >= weightP & VolAlc < weightP & btn==true )     //Schleife fuer Relais Mischgetraenk auf Pin5
  {
  digitalWrite(RelaisMix, HIGH);
  }
  else
  {
  digitalWrite(RelaisMix, LOW);
  }

  if(weightP >= VolAlc + VolMix)  //beenden der Schleife durch btn bool variable
  {
    btn = false;
  }


   if(Serial.available()) //Bei Input von T -> tarieren
  {
    char temp = Serial.read();        //Eingabe eines charakters
    if(temp == 't' || temp == 'T')    //wenn eingegebener charakter t ist wird tariert
      scale.tare();                   //Befehl zum Tarieren
   }

  delay(10); //Verzoegerung des gesamten Loops
}
