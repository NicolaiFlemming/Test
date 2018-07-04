#include "HX711.h"              //Bibliothek für Waage
#include <Wire.h>               //Bibliothek für generellen LCD   
#include <LiquidCrystal_I2C.h>  //Bibliothek für I2C LCD

#define DOUT 3
#define CLK 2      //für HX711 Inputs

HX711 scale(DOUT, CLK);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD-Verbindungen

float calibration_factor = -92450;      //Kalibrierungsfaktor für unsere Load Cell
float weight;                           //erklären der Variable, welche die Waage nutzt
int weightP;                            //Nachkommastellen loswerden, durch int

int RelaisAlc = 4;                      //Definieren des Ports fuer Relais 1
int RelaisMix = 5;                      //Definieren des Ports fuer Relais 2
char But = A0;                          //Port für Button
char Pot = A5;                          //Port für Potentiometer

int VolGes = 150;                       //Volumen Gesamt
int VolAlc = 50;                        //50ml als Experimentalwert
int VolMix = 70;                        //70ml als Experimentalwert
int AlcPerc;                            //Prozentsatz Alkohol
int MixPerc;                            //Prozentsatz Mischgetränk
float Ratio;                            //Mischverhältnis
float PreRatio;                         //Unmapped Mischverhältnis

bool btn = false;                       //Wert für Button

//=============================================================================================
//=============================================================================================

void setup()
{
    Serial.begin(9600);
    Serial.println("Press T to tare");
    scale.set_scale(calibration_factor);    //kalibriert 2018-05-03 aus scale_setup.ino
    scale.tare();                           //Tarieren der Waage

    lcd.begin(16, 2);                       //Ausgabe LCD-Bildschirm
    lcd.backlight();                        //Anstellen Hintergrundlicht LCD
    lcd.setCursor(0, 1);                    //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
    lcd.print("Gewicht:");                  //Auf LCD-Bildschirm schreiben
    lcd.setCursor(0, 0);                    //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
    lcd.print("Ratio:");                    //Auf LCD-Bildschirm schreiben

    pinMode(RelaisAlc, OUTPUT);
    pinMode(RelaisMix, OUTPUT);             //Defininieren der Relais Ports als Output
    pinMode(But, INPUT);                    //Definieren des Buttons als Analog Input 8Bit
    pinMode(Pot, INPUT);                    //Definieren des Pins als Analog Input 8Bit

    digitalWrite(RelaisAlc, LOW);
    digitalWrite(RelaisMix, LOW);           //Ausstellen der Relais

    delay(2000);
}

//=============================================================================================
//=============================================================================================

void loop()
{
    if (analogRead(But) > 1000)
    {
        btn = true;   //wenn Knopf gedrückt wurde wird btn bool Variable mit true ueberschrieben
        scale.tare(); //bei knopfdruck tarieren
        delay(500);   //halbe sekunde verzoegerung
    }

    if (btn == false)                                       //Alles was passieren soll bevor der knopf gedrückt wird
    {
        PreRatio = (constrain(analogRead(Pot), 0, 1023));   //analog werte des Potentiometers zwischen 0 und 1023 (8bit) beschraenken und auf PreRatio Variable schreiben
        Ratio = (PreRatio / 1023);                          //Variable umrechnen zu einem Verhaeltnis für VolAlc und VolMix
        VolAlc = (VolGes * Ratio);                          //VolAlc mithilfe der Ratio Variable berechnen
        VolMix = (VolGes - VolAlc);                         //Volmix mithilfe von VolGes und VolAlc berechnen
        AlcPerc = (Ratio * 101);                            //Prozentsatz Alkohol mithilfe der Ratio Variablen berechnen. 101 wegen rundungsfehlern
        MixPerc = ((1 - Ratio) * 101);                      //Prozentsatz Mischgetraenk mithilfe der Ratio Variablen berechnen. 101 wegen rundungsfehlern
    }

    weight = (scale.get_units() * 1000);                    //einholen der werte von der Waage und Umwandlung in Gramm
    weightP = weight;                                       //definieren der variable fuer Gewicht
    lcd.setCursor(10, 1);
    lcd.print(weightP);                                     //Ausgabe des Gewichts auf dem LCD Bildschirm
    lcd.print(" g  ");                                      //Einheit
    lcd.setCursor(10, 0);
    lcd.print(AlcPerc);
    lcd.print("/");
    lcd.print(MixPerc);
    lcd.print(" ");                                         //Ausgabe des Verhaeltnisses auf dem LCD Bildschirm

    if (VolAlc > weightP & weightP > -10 & btn == true)     //Schleife fuer Relais Alkohol auf Pin4
    {
        digitalWrite(RelaisAlc, HIGH);
    }
    else
    {
        digitalWrite(RelaisAlc, LOW);
    }

    if (VolMix + VolAlc >= weightP & VolAlc < weightP & btn == true) //Schleife fuer Relais Mischgetraenk auf Pin5
    {
        digitalWrite(RelaisMix, HIGH);
    }
    else
    {
        digitalWrite(RelaisMix, LOW);
    }

    if (weightP >= VolAlc + VolMix)                     //beenden der Schleife durch btn bool variable
    {
        btn = false;
    }

    if (Serial.available())
    {
        char temp = Serial.read();
        if (temp == 't' || temp == 'T')
            scale.tare();                               //bei eingabe von 't' oder 'T' auf dem seriellen Monitor wird die waage tariert
    }
    
    delay(10);                                          //Verzögerung des gesamten Loops
}
//=============================================================================================
