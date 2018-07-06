bool checkPhase(int mom, int phase)           //Erste Funktion Jakob Jehn
{
    if(mom == phase)                          //Vergleich Phase mit aktuellem Wert
    {
        return true;
    }
    else
    {
        return false;
    }
}

int Success(bool state, int phase)              //Zweite Funktion Jakob Jehn
{
    if(state == false)                          //Schickt funktion in die aktuelle phase zurück
    {
        return phase;
    }

    if(state == true)
    {
        return 0;
    }
}

bool checkPoti(float Pot, int Vol)              //Erste Funktion Nicolai Flemming
{
    if(1023 >= Pot & Pot >= 0 & Vol <= 500)     //Ueberprueft ob Eingabewerte in ihren Grenzen bleiben
    {
        return true;
    }
    else
    {
        return false;
    }
}

float volCalc(float poti)                       //Zweite Funktion Nicolai Flemming
{
    float out;                                  //Wechselt Potieingang zu gewünschtem Volumen
    out = (poti / 1023) * 400 + 100;
    return out;

    if(out > 500)                               //gibt 0 bei Werten größer als eigentlich möglich
    {
        return 0;
    }
}

int shortit(float weight)                       //Zweite Funktion Tibor Baedecker
{
    int weightP;
    weightP = (weight * 1000);
    return weightP;
}
