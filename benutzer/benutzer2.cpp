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

    int kge;          // Körpergewicht
    char gen;         // Geschlecht  
    float sufflvl;    // Promillegehalt im Blut
    int alkges=12;    // Insgesamt verzehrter Alkohol zu Demo Zwecken hier auf einen wahrlosen Wert gesetzt.
    
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

int main(){ 
int auswahl;          // Benutzerauswahl
// Erstellen der Objekte
benutzer benutzer1;
benutzer benutzer2;
benutzer benutzer3;
benutzer benutzer4;
benutzer benutzer5;
benutzer benutzer6;
// Auswahl des Benutzers
cout<<"druecken sie eine Zahl von 1-6 um einen benutzer auszuwaehlen"<<endl;
cin>>auswahl;
switch (auswahl){
    case 1:{
        
       int gew;                // Provisorische Variable für Gewicht zur Übergabe an Methode
       char gesch;            // Provisorische Variable für gender zur Übergabe an Methode
        
        cout<<"Koerpergewicht eingeben"<<endl;
        cin>>gew;
        cout<<"Bitte Geschlecht angeben [M] || [W]"<<endl;
        cin>> gesch;
        
       

        benutzer1.set_kge(gew);
        benutzer1.set_gen(gesch);
        benutzer1.err_sufflvl(gew, gesch, benutzer1.get_alkges());
        
          while ( benutzer1.err_sufflvl(gew,gesch,benutzer1.get_alkges())== false)
         {
            cout<<"Fehler! bitte M oder W eingeben"<<endl;
            cin>>gesch;
            benutzer1.set_gen(gesch);
            benutzer1.err_sufflvl(gew, gesch, benutzer1.get_alkges());
         } 
        

            cout<<"      Benutzer1:"<<endl;
            cout<<"Gewicht    =    "<<benutzer1.get_kge()<<endl;
            cout<<"Geschlecht =    "<<benutzer1.get_gen()<<endl;
            cout<<"Sufflvl    =    "<<benutzer1.get_sufflvl()<<endl;
        return 0;
       
    }
     case 2:{
        
       int gew;                // Provisorische Variable für Gewicht zur Übergabe an Methode
       char gesch;            // Provisorische Variable für gender zur Übergabe an Methode
       
        
        cout<<"Koerpergewicht eingeben"<<endl;
        cin>>gew;
        cout<<"bitte Geschlecht angeben [M] || [W]"<<endl;
        cin>> gesch;
        
       

        benutzer2.set_kge(gew);
        benutzer2.set_gen(gesch);
        benutzer2.err_sufflvl(gew, gesch, benutzer2.get_alkges());

        
        while (benutzer2.err_sufflvl(gew,gesch,benutzer2.get_alkges())== false)
         {
            cout<<"Fehler! bitte M oder W eingeben"<<endl;
            cin>>gesch;
            benutzer2.set_gen(gesch);
            benutzer2.err_sufflvl(gew, gesch, benutzer2.get_alkges());
         } 
         

        cout<<"      Benutzer2:"<<endl;
        cout<<"Gewicht    =    "<<benutzer2.get_kge()<<endl;
        cout<<"Geschlecht =    "<<benutzer2.get_gen()<<endl;
        cout<<"Sufflvl    =    "<<benutzer2.get_sufflvl()<<endl;
        return 0;
       
    }

    // dies ist nur eine Testversion für das Benutzersystem deshalb gibt es vorerst nur zwei Benutzer
    /* 
        case 3:{}
        case 4:{}
        case 5:{}
        case 6:{}  
    */

}

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




