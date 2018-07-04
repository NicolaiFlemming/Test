#include "HX711.h"

#define DOUT 3
#define CLK 2

HX711 scale(DOUT, CLK);

//Diesen Kalibrierungsfaktor je nach Bedarf anpassen bis die ausgegebenen Werte stimmen . Ist für jede Load Cell anders
float calibration_factor = -96650;

//=============================================================================================
//=============================================================================================
void setup()
{
    Serial.begin(9600);
    Serial.println("HX711 Kalibrierung");
    Serial.println("Alles Gewicht von Waage entfernen");
    Serial.println("Wenn Ausgabe beginnt, bekanntes Gewicht auf Waage stellen");
    Serial.println("Drücken sie a,s,d,f um Kalibrierungsfaktor um 10,100,1000,10000 zu erhöhen");
    Serial.println("Drücken sie z,x,c,v um Kalibrierungsfaktor um 10,100,1000,10000 zu verringern");
    Serial.println("Drücken sie T zum tarieren");
    scale.set_scale();
    scale.tare(); //Tarieren der Waage

    //long zero_factor = scale.read_average(); //Grundwert für die Waage bekommen
    //Serial.print("Zero factor: "); //Kann benutzt werden um tarieren zu vermeiden
    //Serial.println(0);
}

//=============================================================================================
//=============================================================================================
void loop()
{

    scale.set_scale(calibration_factor); //hier wird der Kalibrierungsfaktor eingfügt

    Serial.print("Reading: ");
    Serial.print(scale.get_units(), 3);
    Serial.print(" kg");
    Serial.print(" calibration_factor: ");
    Serial.print(calibration_factor);
    Serial.println();

    if (Serial.available())
    {
        char temp = Serial.read();
        if (temp == '+' || temp == 'a')
            calibration_factor += 10;
        else if (temp == '-' || temp == 'z')
            calibration_factor -= 10;
        else if (temp == 's')
            calibration_factor += 100;
        else if (temp == 'x')
            calibration_factor -= 100;
        else if (temp == 'd')
            calibration_factor += 1000;
        else if (temp == 'c')
            calibration_factor -= 1000;
        else if (temp == 'f')
            calibration_factor += 10000;
        else if (temp == 'v')
            calibration_factor -= 10000;
        else if (temp == 't')
            scale.tare(); //bei t tarieren
    }
}
//=============================================================================================
