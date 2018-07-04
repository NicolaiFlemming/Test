#include "HX711.h"                      //Bibliothek für Waage
#include <Wire.h>                       //Bibliothek für generellen LCD   
#include <LiquidCrystal_I2C.h>          //Bibliothek für I2C LCD
#include "benutzerv12.h"                //Eigene Benutzerbibliothek

#define DOUT 3
#define CLK 2                           //für HX711 Inputs

HX711 scale(DOUT, CLK);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //LCD-Verbindungen

float calibration_factor = -92450;      //Kalibrierungsfaktor für unsere Load Cell
float weight;                           //erklären der Variable, welche die Waage nutzt
int weightP;                            //Nachkommastellen loswerden, durch int

int RelaisAlc = 4;                      //Definieren des Ports fuer Relais 1
int RelaisMix = 5;                      //Definieren des Ports fuer Relais 2
char But1 = 10;                         //Port für Button 1
int But2 = 11;                          //Port für Button 2
int But3 = 12;                          //Port für Button 3
char Pot1 = A5;                          //Port für Potentiometer
char Pot2 = A9;                         //Port für Potentiometer nr 2

int VolGes;                             //Volumen Gesamt
int VolAlc;                             //Volumen Alkohol 
int VolMix;                             //Volumen Mischgetränk
int AlcPerc;                            //Prozentsatz Alkohol
int MixPerc;                            //Prozentsatz Mischgetränk
float Ratio;                            //Mischverhältnis
float PreRatio;                         //Unmapped Mischverhältnis
float Vol;                              //Volumen
float PreVol;                           //Unmapped Volumen

bool btn = false;                       //Wert für Button
int phase = 0;                          //Phase des Systems
int user = 0;                           //Aktuell ausgewählter Nutzer
int usercount = 0;                      //Anzahl erstellter Benutzer
bool sff;                               //Bool damit Sufflvl nur einmal pro phasendurchlauf im loop addiert wird
const int max = 10;                     //Anzahl maximaler Benutzer

benutzer benutzer[max];                 //initialisierung Benutzertabelle

//=============================================================================================
//=============================================================================================

void setup()
{
    Serial.begin(9600);
    Serial.println("Press T to tare");
    scale.set_scale(calibration_factor);    //kalibriert 2018-05-03 aus scale_setup.ino
    scale.tare();                           //Tarieren der Waage

    pinMode(RelaisAlc, OUTPUT);
    pinMode(RelaisMix, OUTPUT);             //Defininieren der Relais Ports als Output
    pinMode(But1, INPUT);                   //Definieren des Buttons als Analog Input 8Bit
    pinMode(But2, INPUT);                   //Definieren des Pins als Digital Input 8Bit
    pinMode(But3, INPUT);                   //Definieren des Pins als Digital Input 8Bit
    pinMode(Pot1, INPUT);                   //Definieren des Pins als Analog Input 8Bit
    pinMode(Pot2, INPUT);                   //Definieren des Pins als Analog Input 8Bit

    lcd.begin(16, 2);                       //Ausgabe LCD-Bildschirm
    lcd.backlight();                        //Anstellen Hintergrundlicht LCD

    digitalWrite(RelaisAlc, LOW);
    digitalWrite(RelaisMix, LOW);           //Ausstellen der Relais

    delay(2000);
}

//=============================================================================================
//=============================================================================================

void loop()
{
    if (phase == 0)                                             //Auswahl Benutzerauswahl oder -erstellung
    {
        lcd.clear();
        do
        {
            lcd.setCursor(0, 0);                                //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
            lcd.print("Benutzer");                              //Auf LCD-Bildschirm schreiben
            lcd.setCursor(0, 1);                                //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
            lcd.print("auswählen/erstellen");                   //Auf LCD-Bildschirm schreiben

            if (digitalRead(But2) == HIGH)                      //Benutzer auswählen und Phasenwechsel
            {
                phase = 1;
                delay(1000);
            }
            if (digitalRead(But3) == HIGH)                      //Benutzer erstellen Phasenwechsel
            {
                phase = 2;
                delay(1000);
            }
        } 
        while (phase == 0);
    }

    if (phase == 1)                                             //Auswahl Benutzer
    {
        lcd.clear(); 
        
        lcd.setCursor(0, 1);
        lcd.print("</> T = ok");
        lcd.setCursor(0, 0);
        if (user <= usercount)
        {
            lcd.print("Benutzer ");
            lcd.print(user + 1);
        }
        if (user == (usercount + 1))                        //usercount nicht größer als max. max + 1 ist der Gastbenutzer
        {
            lcd.print("Gast      ");                        //Gastdaten werden nicht gespeichert
        }
        if (digitalRead(But2) == HIGH)                      //Durch die User wechseln
        {
            user -= 1;
            if (user < 0)
                user = (usercount + 1);
            delay(500);
        }
        if (digitalRead(But3) == HIGH)
        {
            user += 1;
            if (user > usercount)
                user = 0;
            delay(500);
        }
        if (digitalRead(But1) == HIGH)                      //Benutzer bestätigen
        {
            phase = 3;                                      //Einschenkvorgang wechseln
            delay(1000);
        }
    
    }

    if (phase == 2)                                             //Benutzererstellung
    {
        lcd.clear();
        bool w = 0;                                             //Wechsel zwischen Bildschirmen
        while (phase == 2)
        {
            if (usercount == max)
            {
                lcd.setCursor(0, 0);
                lcd.print("Keine Benutzer mehr");               //Benutzer voll. Reset des Systems um Tabelle zu löschen
                lcd.setCursor(0, 1);
                lcd.print("Fortfahren als Gast");
                delay(2000);
                phase = 3;                                      //Einschenkvorgang
                user = max + 1;
            }
            else
            {
                float gew;
                char gesch;
                if (w == 0)                                     //wechseln der eingabe
                {
                    PreRatio = (constrain(analogRead(Pot1), 0, 1023));
                    Ratio = (PreRatio / 1023);                  //benutzen des potis um gewicht einzustellen
                    gew = 40 + (Ratio * 100);

                    lcd.setCursor(0, 0);                        //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
                    lcd.print("Gewicht einstellen:");           //Auf LCD-Bildschirm schreiben
                    lcd.setCursor(0, 1);                        //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
                    lcd.print(gew);                             //Auf LCD-Bildschirm schreiben
                    lcd.print(" kg  info = ok");

                    if (digitalRead(But1) == HIGH)              //Gewichtvariable Fest
                    {
                        w = 1;
                        delay(1000);
                    }
                }

                if (w == 1)                                     //Geschlecht wichtig für Promille berechnung
                {
                    lcd.setCursor(0, 0);                        //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
                    lcd.print("Geschlecht auswählen:");         //Auf LCD-Bildschirm schreiben
                    lcd.setCursor(0, 1);                        //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
                    lcd.print("Männlich/Weiblich");             //Auf LCD-Bildschirm schreiben

                    if (digitalRead(But2) == HIGH)
                    {
                        gesch = 'M';                            //Männliches Geschlecht eingegeben
                        benutzer[usercount].set_kge(gew);
                        benutzer[usercount].set_gen(gesch);
                        usercount += 1;
                        phase = 3;
                        delay(1000);
                    }
                    if (digitalRead(But3) == HIGH)
                    {
                        gesch = 'W';                            //Weibliches Geschlecht eingegeben
                        benutzer[usercount].set_kge(gew);
                        benutzer[usercount].set_gen(gesch);
                        usercount += 1;
                        phase = 3;
                        delay(1000);
                    }
                    benutzer[usercount].set_sufflvl(0);
                }
            }
        }
    }

    if (phase == 3)
    {
    
    if (digitalRead(But1) == HIGH)
        {
            btn = true;                                         //wenn Knopf gedrückt wurde wird btn bool Variable mit true ueberschrieben
            scale.tare();                                       //bei knopfdruck tarieren
            delay(500);                                         //halbe sekunde verzoegerung
        }

        if (btn == false)                                       //Alles was passieren soll bevor der knopf gedrückt wird
        {
            PreRatio = (constrain(analogRead(Pot1), 0, 1023));   //analog werte des Potentiometers zwischen 0 und 1023 (8bit) beschraenken und auf PreRatio Variable schreiben
            Ratio = (PreRatio / 1023);                          //Variable umrechnen zu einem Verhaeltnis für VolAlc und VolMix
            VolAlc = (VolGes * Ratio);                          //VolAlc mithilfe der Ratio Variable berechnen
            VolMix = (VolGes - VolAlc);                         //Volmix mithilfe von VolGes und VolAlc berechnen
            AlcPerc = (Ratio * 101);                            //Prozentsatz Alkohol mithilfe der Ratio Variablen berechnen. 101 wegen rundungsfehlern
            MixPerc = ((1 - Ratio) * 101);                      //Prozentsatz Mischgetraenk mithilfe der Ratio Variablen berechnen. 101 wegen rundungsfehlern

            PreVol = (constrain(analogRead(Pot2), 0, 1023));
            Vol = (PreVol / 1023);
            VolGes = (100 + (Vol)*400);

            lcd.clear();
            lcd.setCursor(0, 0);                                //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
            lcd.print("Ratio:");                                //Auf LCD-Bildschirm schreiben
            lcd.setCursor(10, 0);
            lcd.print(AlcPerc);
            lcd.print("/");
            lcd.print(MixPerc);
            lcd.print(" ");

            lcd.setCursor(0, 1);                                //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
            lcd.print("Volumen:");                              //Auf LCD-Bildschirm schreiben
            lcd.setCursor(10, 1);
            lcd.print(VolGes);
            lcd.print("ml ");
        }

        weight = (scale.get_units() * 1000);                    //einholen der werte von der Waage und Umwandlung in Gramm
        weightP = weight;                                       //definieren der variable fuer Gewicht

        if (btn == true)
        {
            lcd.setCursor(0, 1);                                //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
            lcd.print("Gewicht:  ");                            //Auf LCD-Bildschirm schreiben
            lcd.setCursor(10, 1);
            lcd.print(weightP);                                 //Ausgabe des Gewichts auf dem LCD Bildschirm
            lcd.print(" g  ");                                  //Einheit
        }

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

        if (weightP >= VolAlc + VolMix)                         //beenden der Schleife durch btn bool variable
        {
            btn = false;
            sff = true;
            phase = 4;
        }

        if (Serial.available())
        {
            char temp = Serial.read();
            if (temp == 't' || temp == 'T')
            scale.tare();                                       //bei eingabe von 't' oder 'T' auf dem seriellen Monitor wird die waage tariert
        }
    } 
    delay(10);                                                  //Verzögerung des gesamten Loops

    if (phase == 4)                                             //Ende und eintragen der Werte in die Benutzertabelle
    {
        int AlcinG, AlcinGges;
        float AlcSet;
        AlcinG = (VolAlc * 0.4);                                //40%er Alkohol

        if (sff == true)
        {
            AlcinGges = AlcinG + benutzer[user].get_alkges();
            benutzer[user].set_alkges(AlcinGges);
            benutzer[user].err_sufflvl(benutzer[user].get_kge(), benutzer[user].get_gen(), AlcinGges);
            sff = false;
        }

        lcd.clear();
        lcd.setCursor(0, 0);                                    //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
        lcd.print("Benutzer");                                  //Auf LCD-Bildschirm schreiben
        lcd.print(user + 1);                                    //Auf LCD-Bildschirm schreiben
        lcd.setCursor(0, 1);                                    //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
        if (benutzer[user].get_sufflvl() < 1.5)                 //Benutzer noch nicht sehr betrunken
        {
            lcd.print("Prost!!!");
        }
        if (benutzer[user].get_sufflvl() >= 1.5)                //Benutzer wahrscheinlich betrunken
        {
            lcd.print("Als nächstes vielleicht Wasser?");
        }
        delay(2000);
        phase = 0;                                              //Erster Bildschirm (von vorne)
    }
}
//=============================================================================================
