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

int VolAlc = 50;                        //50ml als Experimentalwert
int VolMix = 70;                        //70ml als experimentalwert

//=============================================================================================
//=============================================================================================

void setup()
{
    Serial.begin(9600);
    Serial.println("Press T to tare");
    scale.set_scale(calibration_factor);    //kalibriert 2018-05-03 aus scale_setup.ino
    scale.tare();                           //Tarieren der Waage

    lcd.begin(16, 2);                       //Ausgabe LCD-Bildschirm
    lcd.backlight();
    lcd.setCursor(0, 0);                    //Definieren wo auf LCD-Bildschirm Geschrieben wird
    lcd.print("Gewicht:");
    
    pinMode(RelaisAlc, OUTPUT);
    pinMode(RelaisMix, OUTPUT);             //Defininieren der Relais Ports als Output

    digitalWrite(RelaisAlc, LOW);
    digitalWrite(RelaisMix, LOW);           //Ausstellen der Relais

    delay(2000);                            //benötigte Verzoegerung fuer tarieren
    scale.tare();                           //2. Kalibrierung
}

//=============================================================================================
//=============================================================================================

void loop()
{
    weight = (scale.get_units() * 1000);            //Umwandlung in Gramm
    weightP = weight;                               //definieren der Variable für Gewicht
    lcd.setCursor(0, 1);
    lcd.print(weightP);                             //Ausgabe des Gewichts auf dem LCD Bildschirm
    lcd.print(" g  ");                              //Einheit

    if (VolAlc > weightP)                           //Schleife fuer Relais 1 auf Pin4
    {
        digitalWrite(RelaisAlc, HIGH);
    }
    else
    {
        digitalWrite(RelaisAlc, LOW);
    }
    
    if (VolMix + VolAlc > weightP)                  //Schleife fuer Relais 2 Auf Pin5
    {
        digitalWrite(RelaisMix, HIGH);
    }
    else
    {
        digitalWrite(RelaisMix, LOW);
    }

    if (Serial.available())
    {
        char temp = Serial.read();
        if (temp == 't' || temp == 'T')
            scale.tare();                           //bei eingabe von 't' oder 'T' auf dem seriellen Monitor wird die waage tariert
    }
    
    delay(10);                                      //Verzögerung des gesamten Loops
}
//=============================================================================================
