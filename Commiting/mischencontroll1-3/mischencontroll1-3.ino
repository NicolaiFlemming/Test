#include "HX711.h" //Bibliothek für Waage

#define DOUT 3
#define CLK 2

HX711 scale(DOUT, CLK);

float calibration_factor = -92450; //Kalibrierungsfaktoir für unsere Load Cell aus

float weight; //erklären der Variable, welche die Waage nutzt
int weightP;  //Nachkommastellen loswerden, durch int

//=============================================================================================
//=============================================================================================
void setup()
{
    Serial.begin(9600);
    Serial.println("Press T to tare");
    scale.set_scale(calibration_factor); //Kalibrierungsfaktor aus scale_setup.ino
    scale.tare();                        //Tarieren der Waage
}

//=============================================================================================
//=============================================================================================
void loop()
{
    Serial.print("Weight: ");
    Serial.print((scale.get_units() * 1000), 0); //Bis zu 3 Dezimal stellen
    Serial.println(" g");

    weight = (scale.get_units() * 1000); //Umwandlung in Gramm
    weightP = weight;                    //definieren der variable fuer Gewicht

    if (Serial.available())
    {
        char temp = Serial.read();
        if (temp == 't' || temp == 'T')
            scale.tare(); //bei eingabe von 't' oder 'T' auf dem seriellen Monitor wird die waage tariert
    }
}
//=============================================================================================
