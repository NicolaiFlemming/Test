/*-------------------------------------------Beschreibung-------------------------------------------------
Dies ist die fertige Testversion für die Benutzerklassen. Der Code in der Hauptfunktion wird in stark 
abgewandelter Form in der loop-Funktion der ino-Datei eingefügt werden. Alle Befehle sind zur einfachen
Überprüfung für das Terminal (io stream) geschrieben und müssen durch für den Arduino interpretierbare 
Anweisungen umgeschrieben werden. 
---------------------------------------------------------------------------------------------------------*/




#include <iostream>					

using namespace std;
//Klassendefinition
class benutzer
{
    private:
    // Attribute   
    int kge;       // Körpergewicht
    char gen;      // Geschlecht
    float sufflvl; // Promillegehalt im Blut
    int alkges=12; // Insgesamt verzehrter Alkohol zu Demo Zwecken hier auf einen wahrlosen Wert gesetzt.
    
    public:
    // Methoden               
    int get_kge();
    char get_gen();
    float get_sufflvl();
    int get_alkges();
    
    void set_gen(char gen);
    void set_kge(int kge);
    bool err_sufflvl(int kge, char gen,int alkges); 
     // set alkgesamt muss im Hauptprogramm interagiertwerden und jedes mal nach Abschluss eines Getränkes erhöht werden.
};



int main()
{ 
    int auswahl;   // Momentaner benutzer indize
    int anzben;    // Anzahl der Benutzer
    cout<<"wie viele Benutzer soll es geben?"<<endl;
    cin>>anzben;
    benutzer benutzer[anzben];         // Erstellen der Objekte
   for (auswahl=1; auswahl<=anzben ; auswahl++)     // Schrittweise füllen der Objekte
   {
   
    
    int gew;                // Provisorische Variable für Gewicht zur Übergabe an Methode
    char gesch;            // Provisorische Variable für gender zur Übergabe an Methode
    cout<<"Benutzer  "<<auswahl<<endl;
    cout<<"Koerpergewicht eingeben"<<endl;
    cin>>gew;
    cout<<"bitte Geschlecht angeben [M] || [W]"<<endl;
    cin>> gesch;
      
       

    benutzer[auswahl].set_kge(gew);                                               // festlegen des Körpergewichts
    benutzer[auswahl].set_gen(gesch);                                             // festlegen des Geschlechts
    benutzer[auswahl].err_sufflvl(gew, gesch, benutzer[auswahl].get_alkges());    // Berechnen des Promille Gehalts

 

        if (benutzer[auswahl].err_sufflvl(gew,gesch,benutzer[auswahl].get_alkges())== false)  // Fehlermeldung wenn Geschlecht falsch angegeben
    {
        cout<<"Fehler! bitte M oder W eingeben"<<endl;
        cin>>gesch;
        benutzer[auswahl].set_gen(gesch);
        benutzer[auswahl].err_sufflvl(gew, gesch, benutzer[auswahl].get_alkges());
    }
   }
   
   int info;
   cout<<"Ueber welchen Benutzer wollen sie informationen?"<<endl;
   cin>>info;
   cout<<"   Benutzer:"<<info<<endl;
    cout<<"Gewicht    =    "<<benutzer[info].get_kge()<<endl;
    cout<<"Geschlecht =    "<<benutzer[info].get_gen()<<endl;
    cout<<"Sufflvl    =    "<<benutzer[info].get_sufflvl()<<endl;
    return 0;
}





//Methoden 

int benutzer::get_kge() {        // Methode zum zugriff auf Körpergewicht
 return kge;
}

char benutzer::get_gen(){       // Methode zum Zugriff auf Geschlecht
 return gen;
}
float benutzer::get_sufflvl(){  // Methode zum zugriff auf Promille-Gehalt
    
    return sufflvl;   
}
int benutzer::get_alkges(){     // Methode zum zugriff auf insgesamt verzehrten Alkohol
    return alkges;
}

void  benutzer::set_kge(int kge){  // Methode zum festlegen des Körpergewichts
    this->kge =kge;
}
void benutzer::set_gen(char gen){  // Methode zum festlegen des Geschlechts
    this->gen=gen;
}
bool benutzer::err_sufflvl(int kge, char gen, int alkges ){  // Errechnen des Promille-Gehalts
    
    //Das Errechen des Alkoholgehalts im Blut erforlgt bei Männern und Frauen unterschiedlich, da der Wasser und Fett gehalt des Körpers variiert.
   
    if (this->gen=='m' || this->gen=='M'){
        this-> sufflvl = this->alkges/(this->kge*0.68);
        return true;
    }
    if (this->gen=='W' || this->gen=='w'){
        this->sufflvl = this->alkges/(this->kge*0.55);
        return true;
    }
    if (this->gen!='m','M','w','W')
    {
        return false;
    }

    // Bei falscheingabe des Geschlechts wird das Wert der Methode auf false gesetzt. Dies wird später in der Hauptfunktion abgefragt
}


