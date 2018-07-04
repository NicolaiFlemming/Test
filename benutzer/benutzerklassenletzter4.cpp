#include <iostream>					

using namespace std;
//Klassendefinition
class benutzer
{
    private:
    // Attribute   
    int kge;
    char gen;        
    float sufflvl;
    int alkges=12; // Insgesamt verzehrter Alkohol zu Demo_Zwecken hier auf einen wahrlosen Wert gesetzt.
    
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


// eventuell kann jemand eine EXIT funktion erstellen
int main()
{ 
    int auswahl;
    int anzben;
    cout<<"wie viele Benutzer soll es geben?"<<endl;
    cin>>anzben;
    benutzer benutzer[anzben];
   for (auswahl=1; auswahl<=anzben ; auswahl++)
   {
   
    
    int gew;
    char gesch;            // provisorische Variable für gender zur übergabe an methode
    char abbruch;
    cout<<"Benutzer  "<<auswahl<<endl;
    cout<<"Koerpergewicht eingeben"<<endl;
    cin>>gew;
    cout<<"bitte Geschlecht angeben [M] || [W]"<<endl;
    cin>> gesch;
      
       

    benutzer[auswahl].set_kge(gew);
    benutzer[auswahl].set_gen(gesch);
    benutzer[auswahl].err_sufflvl(gew, gesch, benutzer[auswahl].get_alkges());

    if (benutzer[auswahl].err_sufflvl(gew,gesch,benutzer[auswahl].get_alkges())== false)
    {
        cout<<"Fehler! bitte M oder W eingeben"<<endl;
        cin>>gesch;
        benutzer[auswahl].set_gen(gesch);
        benutzer[auswahl].err_sufflvl(gew, gesch, benutzer[auswahl].get_alkges());
    } 

    
   }
   
   
   cout<<"Ueber welchen Benutzer wollen sie informationen?"<<endl;
   cin>>info;
   cout<<"   Benutzer:"<<info<<endl;
    cout<<"Gewicht    =    "<<benutzer[info].get_kge()<<endl;
    cout<<"Geschlecht =    "<<benutzer[info].get_gen()<<endl;
    cout<<"Sufflvl    =    "<<benutzer[info].get_sufflvl()<<endl;
    return 0;
}





//Methoden 

int benutzer::get_kge() {
 return kge;
}


char benutzer::get_gen(){
 return gen;
}
float benutzer::get_sufflvl(){
    
    return sufflvl; 
   
}
int benutzer::get_alkges(){
    return alkges;
}



void  benutzer::set_kge(int kge){
    this->kge =kge;
}
void benutzer::set_gen(char gen){
    this->gen=gen;
}
bool benutzer::err_sufflvl(int kge, char gen, int alkges ){
    
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
}



