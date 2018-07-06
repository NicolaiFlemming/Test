#include "HX711.h"                                              //Bibliothek für Waage
#include <Wire.h>                                               //Bibliothek für LCD 
#include <LiquidCrystal_I2C.h>                                  //Bibliothek für LCD 
#include "benutzerv12.h"                                        //eigene Benutzerbibliothek
#include "functions.h"                                          //eigene Funktionenbibliotek

#define DOUT  3
#define CLK  2                                                  //Für HX711 Inputs

HX711 scale(DOUT, CLK);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //LCD-Verbindungen

float calibration_factor = -92450;                              //Kalibrierungsfaktor für die Waage
float weight;                                                   //Erklären der Variable, welche die Waage nutzt
int weightP;                                                    //Nachkommastellen loswerden, durch int

int RelaisAlc = 4;                                              //Definieren des Ports für Relais 1
int RelaisMix = 5;                                              //Definieren des Ports für Relais 2
int But1 = 10;                                                  //Port für Button 1
int But2 = 11;                                                  //Port für Button 2
int But3 = 12;                                                  //Port für Button 3  
char Pot1 = A5;                                                 //Port für Potentiometer Nr.1
char Pot2 =A9;                                                  //Port für Potentiometer Nr.2

int VolAlc;                                                     //Volumen Alkohol
int VolMix;                                                     //Volumen Mixgetränk
int VolGes;                                                     //Volumen Gesamt
int AlcPerc;                                                    //Prozentsatz Alkohol
int MixPerc;                                                    //Prozentsatz Mischgetränk
float Ratio;                                                    //Mischverhältnis
float PreRatio;                                                 //Unmapped Mischverhältnis
float Vol;                                                      //Volumen
float PreVol;                                                   //Unmapped Volumen

bool btn = false;                                               //Wert für Button zum bestätigen
int phase = 0;                                                  //Phase des Systems
int user = 0;                                                   //Aktuell ausgewählter Nutzer
int usercount = 0;                                              //Anzahl erstellter Benutzer
bool sff;                                                       //Bool damit Sufflvl nur einmal pro Phasendurchlauf im Loop addiert wird
bool check = false;                                             //Bugfix weightP im ersten durchlauf nicht tariert
bool state = false;                                             //Kontrollvariable um sicherzustellen, dass die Funktion komplett durchläuft
const int max = 10;                                             //Anzahl maximaler Benutzer kann vom Programm nicht geändert werden nur manuell

benutzer benutzer[max];                                         //Initialisierung Benutzertabelle

//====================================================================================================================================================
//Void Setup
//====================================================================================================================================================

void setup() 
{
    Serial.begin(9600);                                           //Seriellen Monitor mit 9600 Baud starten
    Serial.println("T druecken, um zu tarieren");                 //Auf Seriellem Monitor schreiben
    scale.set_scale(calibration_factor);                          //Kalibriert 2018-05-03
    scale.tare();                                                 //Erste Kalibrierung

    pinMode(RelaisAlc, OUTPUT);                                   //Definieren des Pins als Output 5V
    pinMode(RelaisMix, OUTPUT);                                   //Definieren des Pins als Output 5V
    pinMode (But1, INPUT);                                        //Definieren des Pins als Digital Input 8Bit
    pinMode (But2, INPUT);                                        //Definieren des Pins als Digital Input 8Bit
    pinMode (But3, INPUT);                                        //Definieren des Pins als Digital Input 8Bit
    pinMode (Pot1, INPUT);                                        //Definieren des Pins als Analog Input 8Bit
    pinMode (Pot2, INPUT);                                        //Definieren des Pins als Analog Input 8Bit

    lcd.begin(16,2);                                              //Ausgabe LCD-Bildschirm
    lcd.backlight();                                              //Anstellen Hintergrundlicht LCD

    digitalWrite(RelaisAlc, LOW);                                 //RelaisAlc Pin auf 0V stellen 
    digitalWrite(RelaisMix, LOW);                                 //RelaisMix Pin auf 0V stellen

    delay(2000);
}

 //====================================================================================================================================================
 //Void Loop / Phase 0
 //====================================================================================================================================================

void loop()
{
    if(phase == 0)                                                      //Auswahl Benutzerauswahl oder -erstellung
    {
        state = false;
        lcd.clear();
        do
        {
            lcd.setCursor(0,0);                                         //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
            lcd.print("Benutzer        ");                              //Auf LCD-Bildschirm schreiben
            lcd.setCursor(0,1);                                         //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
            lcd.print("ausw./erst.     ");                              //Auf LCD-Bildschirm schreiben
        
            if(digitalRead(But2) == HIGH)                               //Benutzer auswählen und Phasenwechsel
            {
                phase = 1;
                delay(1000);
            }
            if(digitalRead(But3) == HIGH)                               //Benutzer erstellen Phasenwechsel
            {
                phase = 2;
                delay(1000);
            }
        }
        while(checkPhase(0, phase) == true);
    }
 //====================================================================================================================================================
 //Phase 1
 //====================================================================================================================================================
    
    if(phase == 1)                                                      //Auswahl Benutzer
    {
        lcd.setCursor(0,1);
        lcd.print("</>       T = ok");
        lcd.setCursor(0,0);
        if(user < usercount )
        {
            lcd.print("Benutzer ");
            lcd.print(user + 1);
        } 
        if(user == usercount)                                           //usercount nicht größer als max.
        {
            lcd.print("Gast       ");                                   //Gastdaten werden nicht gespeichert
        }    
        if(digitalRead(But2) == HIGH)                                   //Durch die User wechseln
        {
            user -= 1;
            if(user < 0) user = usercount;
            delay(500);
        }     
        if(digitalRead(But3) == HIGH)
        {
            user += 1;
            if(user > usercount) user = 0;
            delay(500);
        }   
        if(digitalRead(But1) == HIGH)                                   //Benutzer bestätigen
        {
            phase = 3;                                                  //Einschenkvorgang wechseln (Phase 3)
            delay(1000);
        }      
    }

 //====================================================================================================================================================
 //Phase 2
 //====================================================================================================================================================
    
    if(phase == 2)                                                              //Benutzererstellung
    {
        bool w = 0;                                                             //Wechsel zwischen Bildschirmen
        
        while(checkPhase(2, phase) == true)   	                                        
        {
            if(usercount == max)
            {
                lcd.setCursor(0,0);
                lcd.print("Keine Benutzer  ");                                  //Benutzer voll. Reset des Systems um Tabelle zu löschen
                lcd.setCursor(0,1);
                lcd.print("Weiter als Gast ");
                delay(2000);
                phase = 3;                                                      //Einschenkvorgang
                user = max + 1;
            }
            else
            {
                float gew;                                                      //Temporäre Var um Werte in Tabelle zu speichern
                char gesch;                                                     //Temporäre Var um Werte in Tabelle zu speichern
                
                if(w == 0)                                                      //Wechseln der Eingabe
                {
                    PreRatio = (constrain(analogRead(Pot1), 0, 1023));
                    Ratio = (PreRatio/1023);                                    //Benutzen des Potentiometers um Gewicht einzustellen
                    gew = 40 + (Ratio * 100);

                    lcd.setCursor(0,0);                                         //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
                    lcd.print("Gewicht einst.: ");                              //Auf LCD-Bildschirm schreiben
                    lcd.setCursor(0,1);                                         //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
                    lcd.print(gew);                                             //Auf LCD-Bildschirm schreiben
                    lcd.print(" kg T = ok     ");

                    if(digitalRead(But1) == HIGH)                               //Zu Geschlechtsauswahl weiter gehen
                    {
                        w = 1;
                        delay(1000);
                    }
                }
        
                if(w == 1)                                                      //Geschlecht wichtig für Promille berechnung
                {                    
                    lcd.setCursor(0,0);                                         //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
                    lcd.print("Geschlecht ausw.:");                             //Auf LCD-Bildschirm schreiben
                    lcd.setCursor(0,1);                                         //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
                    lcd.print("Maennl./Weibl. ");                               //Auf LCD-Bildschirm schreiben 

                    if(digitalRead(But2) == HIGH)
                    {
                        gesch = 'M';                                            //Männliches Geschlecht eingegeben
                        benutzer[usercount].set_kge(gew);                       //Eingabe Gewicht in Klasse
                        benutzer[usercount].set_gen(gesch);                     //Eingabe Geschlecht in Klasse
                        usercount += 1;
                        phase = 3;
                        delay(1000);
                    }
                    
                    if(digitalRead(But3) == HIGH)
                    {
                        gesch = 'W';                                            //Weibliches Geschlecht eingegeben
                        benutzer[usercount].set_kge(gew);
                        benutzer[usercount].set_gen(gesch);
                        usercount += 1;
                        phase = 3;
                        delay(1000);
                    }
                    
                    benutzer[usercount].set_sufflvl(0);                         //Promille Zahl auf 0 setzen für den erstellten Benutzer
                    user = usercount - 1;
                }    
            }
        }
    }

 //====================================================================================================================================================
 //Phase 3
 //====================================================================================================================================================
    
    if(phase == 3)                                                              //Einschenkvorgang  
    {
        do                                                                      //check = false btn = false
        {
            if(digitalRead(But1) == HIGH )                                      //Bei Knopfdruck von Button 1
            {
                btn = true;                                                     //Wenn Knopf gedrückt wurde wird btn bool Variable mit true überschrieben
                scale.tare();                                                   //Bei Knopfdruck tarieren
                delay(50);                                                      //Halbe Sekunde Verzoegerung
                check = true;
            }
            
            weightP = shortit(scale.get_units());                          //Einholen der Werte von der Waage und Umwandlung in Gramm
        
            if (btn==true)
            {
                lcd.setCursor(0,1);                                             //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
                lcd.print("Gewicht:");                                          //Auf LCD-Bildschirm schreiben
                lcd.setCursor(10,1);
                lcd.print(weightP);                                             //Ausgabe des Gewichts auf dem LCD Bildschirm
                lcd.print(" g  " );                                             //Einheit
            }

            else                                                                //Alles was passieren soll bevor der Knopf gedrückt wird
            {
                PreRatio = (constrain(analogRead(Pot1), 0, 1023));              //Analog Werte des Potentiometers zwischen 0 und 1023 (8bit) beschränken und auf PreRatio Variable schreiben
                Ratio = (PreRatio/1023);                                        //Variable umrechnen zu einem Verhältnis für VolAlc und VolMix
                VolAlc = (VolGes * Ratio);                                      //VolAlc mithilfe der Ratio Variable berechnen
                VolMix = (VolGes - VolAlc);                                     //Volmix mithilfe von VolGes und VolAlc berechnen
                AlcPerc = (Ratio*101);                                          //Prozentsatz Alkohol mithilfe der Ratio Variablen berechnen. 101 wegen Rundungsfehlern
                MixPerc = ((1-Ratio)*101);                                      //Prozentsatz Mischgetraenk mithilfe der Ratio Variablen berechnen. 101 wegen Rundungsfehlern
                PreVol = (constrain(analogRead(Pot2), 0 , 1023));
                VolGes = volCalc(PreVol);                                       //PreVol unnötig--------------------------------------------------------------------------------------------------------------------------------------------

                lcd.setCursor(0,0);                                             //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
                lcd.print("Ratio:    ");                                        //Auf LCD-Bildschirm schreiben
                lcd.setCursor (10,0);                                           //Ausgabe des Verhältnisses auf dem LCD Bildschirm
                lcd.print (AlcPerc);
                lcd.print ("/");
                lcd.print (MixPerc);
                lcd.print (" ");

                lcd.setCursor(0,1);                                             //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
                lcd.print("Volumen:  ");                                        //Auf LCD-Bildschirm schreiben
                lcd.setCursor (10,1);
                lcd.print (VolGes);
                lcd.print ("ml ");
            }

            if( VolAlc > weightP & weightP > -10 & btn == true )                //Schleife für Relais Alkohol auf Pin4
            {
                digitalWrite(RelaisAlc, HIGH);
            }
            else
            {
                digitalWrite(RelaisAlc, LOW);
            }

            if( VolMix + VolAlc >= weightP & VolAlc < weightP & btn==true )     //Schleife für Relais Mischgetränk auf Pin5
            {
                digitalWrite(RelaisMix, HIGH);
            }
            else
            {
                digitalWrite(RelaisMix, LOW);
            }

            if(weightP >= VolAlc + VolMix)                                      //Beenden der Schleife durch btn bool variable
            {
                btn = false;
            }


            if(Serial.available())                                              //Bei Input von T -> tarieren
            {
                char temp = Serial.read();                                      //Eingabe eines Buchstabens 
                if(temp == 't' || temp == 'T')                                  //Wenn eingegebener Buchstabe t ist wird tariert
                { 
                    scale.tare();                                               //Befehl zum Tarieren
                }
                delay(10);
            }
            if(weightP >= VolAlc + VolMix & btn == false & check == true)
            {
                sff = true;                                                     //Wird einmal ausgeführt für die Berechnung
                phase = 4;                                                      //Nächste Phase (Schluss)
                check = false;                                                  //Zurücksezen des checks
            }
            if(checkPoti(PreRatio, VolAlc + VolMix) == false)
            {
                phase = 0;
                lcd.clear();
                lcd.setCursor(0,1);
                lcd.print("Unerwarteter Fehler");
                delay(2000);
            }
        }
        while(checkPhase(3, phase) == true);
    }

 //====================================================================================================================================================
 //Phase 4
 //====================================================================================================================================================

    if(phase == 4)                                      //Ende und eintragen der Werte in die Benutzertabelle
    {
        int AlcinG, AlcinGges;
        float AlcSet;
        AlcinG = (VolAlc * 0.4);                        //40%er Alkohol
    
        if(sff == true & user != usercount)
        {
            AlcinGges = AlcinG + benutzer[user].get_alkges();
            benutzer[user].set_alkges(AlcinGges);
            benutzer[user].err_sufflvl(benutzer[user].get_kge(), benutzer[user].get_gen(), AlcinGges);
            sff = false;
        } 

        lcd.clear();
        lcd.setCursor(0,0);                             //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
        
        if(user == usercount)
        {
          lcd.print("Gast       ");
        }
        else
        {
          lcd.print("Benutzer");                        //Auf LCD-Bildschirm schreiben
          lcd.print(user + 1);                          //Auf LCD-Bildschirm schreiben
        }
        
        lcd.setCursor(0,1);                             //Definieren wo auf LCD-Bildschirm geschrieben wird (Stelle, Zeile)
        
        if(benutzer[user].get_sufflvl() <= 2)           //Benutzer noch nicht sehr betrunken
        {
            lcd.print("Prost!!!            ");
        }
        if(benutzer[user].get_sufflvl() > 2)            //Benutzer wahrscheinlich betrunken
        {
            lcd.print("Wasser vllt?        ");
        }      
        delay(2000);
        
        lcd.setCursor(0,1);
        lcd.print("Dein Level:");
        lcd.print(benutzer[user].get_sufflvl());        //Ausgabe der ungefähren Promille

        delay(2000);
        state = true; 
    }
    phase = Success(state, phase);
    delay(10);
}
