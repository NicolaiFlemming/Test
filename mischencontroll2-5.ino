#include "HX711.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "benutzerv12.h"        //eigene Benutzerbibliothek
//benoetigte Bibliotheken

#define DOUT  3
#define CLK  2      //fuer HX711 Inputs

HX711 scale(DOUT, CLK);

float calibration_factor = -92450; //Kalibrierungsfaktor fuer die Waage

int RelaisAlc = 4;  //Definieren des Ports fuer Relais 1
int RelaisMix = 5;  //Definieren des Ports fuer Relais 2
int But1 = 10;      //Port für Button 1
int But2 = 11;      //Port für Button 2
int But3 = 12;      //Port für Button 3  
char Pot1 = A5;      //Port für Potentiometer
char Pot2 =A9;      //Port für Potentiometer nr 2
//int a = 0;        //Test Variable für seriellen Plotter
int VolAlc;         //Volumen Alkohol
int VolMix;         //Volumen Mixgetränk
int VolGes;         //Volumen Gesamt
int AlcPerc;        //Prozentsatz Alkohol
int MixPerc;        //Prozentsatz Mischgetränk
float Ratio;        //Mischverhältnis
float PreRatio;     //Unmapped Mischverhältnis
float Vol;          //Volumen
float PreVol;       //Unmapped Volumen

const int max = 10;                   //Anzahl maximaler Benutzer


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//LCD-Verbindungen

float weight;   //erklären der Variable, welche die Waage nutzt
int weightP;    //Nachkommastellen loswerden, durch int

bool btn = false;   //Wert für button

int phase = 0;      //Phase des Systems
int user = 0;       //Aktuell ausgewählter Nutzer
int usercount = 0;  //Anzahl erstellter Benutzer
//---------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
benutzer benutzer[max];         //initialisierung Benutzertabelle

bool sff;                       //Bool damit Sufflvl nur einmal pro phasendurchlauf im loop addiert wird

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------


void setup() {

  Serial.begin(9600);                           //Seriellen Monitor mit 9600 Baud starten
  Serial.println("T druecken, um zu tarieren"); //Auf Seriellem Monitor schreiben
  scale.set_scale(calibration_factor);          //kalibriert 2018-05-03
  scale.tare();                                 //erste Kalibrierung

  pinMode(RelaisAlc, OUTPUT); //Definieren des Pins als Output 5V
  pinMode(RelaisMix, OUTPUT); //Definieren des Pins als Output 5V
  pinMode (But1, INPUT);      //Definieren des Pins als Digital Input 8Bit
  pinMode (But2, INPUT);      //Definieren des Pins als Digital Input 8Bit
  pinMode (But3, INPUT);      //Definieren des Pins als Digital Input 8Bit
  pinMode (Pot1, INPUT);       //Definieren des Pins als Analog Input 8Bit
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
    if(phase == 0)          //Auswahl Benutzerauswahl oder -erstellung
    {
        lcd.clear();
        do
        {
            lcd.setCursor(0,0);                 //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
            lcd.print("Benutzer        ");              //Auf LCD-Bildschirm schreiben
            lcd.setCursor(0,1);                 //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
            lcd.print("ausw./erst.     ");   //Auf LCD-Bildschirm schreiben
        
            if(digitalRead(But2) == HIGH)       //Benutzer auswählen und Phasenwechsel
            {
                phase = 1;
                delay(1000);
            }
            if(digitalRead(But3) == HIGH)       //Benutzer erstellen Phasenwechsel
            {
                phase = 2;
                delay(1000);
            }
        }
        while(phase == 0);

    }

    if(phase == 1)          //Auswahl Benutzer
    {

            lcd.setCursor(0,1);
            lcd.print("</>       T = ok");
            lcd.setCursor(0,0);
            if(user <= usercount )
            {
                lcd.print("Benutzer        ");
                lcd.print(user + 1);
            } 
            if(user == (usercount + 1))                 //usercount nicht größer als max. max + 1 ist der Gastbenutzer
            {
                lcd.print("Gast            ");                      //Gastdaten werden nicht gespeichert
            }    
            if(digitalRead(But2) == HIGH)               //Durch die User wechseln
            {
                user -= 1;
                if(user < 0) user = (usercount + 1);
                delay(500);
            }     
            if(digitalRead(But3) == HIGH)
            {
                user += 1;
                if(user > (usercount + 1)) user = 0;
                delay(500);
            }   
            if(digitalRead(But1) == HIGH)                //Benutzer bestätigen
            {
                phase = 3;                              //Einschenkvorgang wechseln
                delay(1000);
            }      

    }

    if(phase == 2)          //Benutzererstellung
    {
        bool w = 0;                                                     //Wechsel zwischen Bildschirmen
        while(phase == 2)
        {
            if(usercount == max)
            {
                lcd.setCursor(0,0);
                lcd.print("Keine Benutzer  ");                       //Benutzer voll. Reset des Systems um Tabelle zu löschen
                lcd.setCursor(0,1);
                lcd.print("Weiter als Gast ");
                delay(2000);
                phase = 3;                                              //Einschenkvorgang
                user = max + 1;
            }
            else
            {
                float gew;
                char gesch;                                             
                if(w == 0)                                              //wechseln der eingabe
                {
                    PreRatio = (constrain(analogRead(Pot1), 0, 1023));
                    Ratio = (PreRatio/1023);                            //benutzen des potis um gewicht einzustellen
                    gew = 40 + (Ratio * 100);

                    lcd.setCursor(0,0);                                 //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
                    lcd.print("Gewicht einst.: ");                   //Auf LCD-Bildschirm schreiben
                    lcd.setCursor(0,1);                                 //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
                    lcd.print(gew);                                     //Auf LCD-Bildschirm schreiben
                    lcd.print(" kg T = ok     ");

                    if(digitalRead(But1) == HIGH)                        //Gewichtvariable Fest
                    {
                        w = 1;
                        delay(1000);
                    }
                }
        
                if(w == 1)                                              //Geschlecht wichtig für Promille berechnung
                {                    
                    lcd.setCursor(0,0);                                 //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
                    lcd.print("Geschlecht ausw.:");                 //Auf LCD-Bildschirm schreiben
                    lcd.setCursor(0,1);                                 //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
                    lcd.print("Maennl./Weibl.");                     //Auf LCD-Bildschirm schreiben 

                    if(digitalRead(But2) == HIGH)
                    {
                        gesch = 'M';                                        //Männliches Geschlecht eingegeben
                        benutzer[usercount].set_kge(gew);
                        benutzer[usercount].set_gen(gesch);
                        usercount += 1;
                        phase = 3;
                        delay(1000);
                    }
                    if(digitalRead(But3) == HIGH)
                    {
                        gesch = 'W';                                        //Weibliches Geschlecht eingegeben
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

    if(phase == 3)              //Einschenkvorgang  
    {
        //a = analogRead(But);
        //Serial.println(a);

        //Serial.print("Weight: ");
        //Serial.print((scale.get_units()*1000), 0);  //Up to 3 decimal points
        //Serial.println(" g"); //Change this to kg and re-adjust the calibration factor if you follow lbs  
        if(digitalRead(But1) == HIGH ) //bei Button drück Analog Wert > 1000
        {
            btn = true;     //wenn Knopf gedrückt wurde wird btn bool Variable mit true ueberschrieben
            scale.tare();   //bei knopfdruck tarieren
            delay(50);     //halbe sekunde verzoegerung
        }

        if (btn == false) //Alles was passieren soll bevor der knopf gedrückt wird
        {
            PreRatio = (constrain(analogRead(Pot1), 0, 1023));     //analog werte des Potentiometers zwischen 0 und 1023 (8bit) beschraenken und auf PreRatio Variable schreiben
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
            sff = true;
            phase = 4;
        }


        if(Serial.available()) //Bei Input von T -> tarieren
        {
            char temp = Serial.read();        //Eingabe eines charakters
            if(temp == 't' || temp == 'T')    //wenn eingegebener charakter t ist wird tariert
            { 
                scale.tare();                   //Befehl zum Tarieren
            }

            delay(10); //Verzoegerung des gesamten Loops
        }
    }

    if(phase == 4)           //Ende und eintragen der Werte in die Benutzertabelle
    {
        int AlcinG, AlcinGges;
        float AlcSet;
        AlcinG = (VolAlc * 0.4);            //40%er Alkohol
    
        if(sff == true)
        {
            AlcinGges = AlcinG + benutzer[user].get_alkges();
            benutzer[user].set_alkges(AlcinGges);
            benutzer[user].err_sufflvl(benutzer[user].get_kge(), benutzer[user].get_gen(), AlcinGges);
            sff = false;
        } 
        
        lcd.setCursor(0,0);                         //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
        lcd.print("Benutzer       ");                      //Auf LCD-Bildschirm schreiben
        lcd.print(user + 1);                        //Auf LCD-Bildschirm schreiben
        lcd.setCursor(0,1);                         //Definieren wo auf LCD-Bildschirm Geschrieben wird (Stelle, Zeile)
        if(benutzer[user].get_sufflvl() < 1.5)      //Benutzer noch nicht sehr betrunken
        {
            lcd.print("Prost!!!        ");
        }
        if(benutzer[user].get_sufflvl() >= 1.5)     //Benutzer wahrscheinlich betrunken
        {
            lcd.print("Wasser vllt?        ");
        }      
        delay(2000);

        phase = 0;                                  //Erster Bildschirm (von vorne)
    }

    delay(10);
}

