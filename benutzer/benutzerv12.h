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
    
    void set_alkges(int alkges);   // set alkgesamt muss im Hauptprogramm interagiertwerden und jedes mal nach Abschluss eines Getränkes erhöht werden.
    void set_gen(char gen);
    void set_kge(int kge);
    bool err_sufflvl(int kge, char gen,int alkges); 
    void set_sufflvl(float sufflvl); 
};

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

void benutzer::set_sufflvl(float sufflvl)  // Methode zum manuellen festsetzen des Promillegehalts
{
    this -> sufflvl = sufflvl;
}
void benutzer::set_alkges(int alkges)    // Methode zum festlegen des verzehrten Alkohols
{
    this -> alkges = alkges;
}
void  benutzer::set_kge(int kge){  // Methode zum festlegen des Körpergewichts
    this->kge =kge;
}
void benutzer::set_gen(char gen){  // Methode zum festlegen des Geschlechts
    this->gen=gen;
}
void benutzer::err_sufflvl(int kge, char gen, int alkges ){  // Errechnen des Promille-Gehalts
    
    //Das Errechen des Alkoholgehalts im Blut erforlgt bei Männern und Frauen unterschiedlich, da der Wasser und Fett gehalt des Körpers variiert.
   
    if (this->gen=='m' || this->gen=='M')
    {
        this-> sufflvl = this->alkges/(this->kge*0.68);
        
    }
    if (this->gen=='W' || this->gen=='w')
    {
        this->sufflvl = this->alkges/(this->kge*0.55);
        
    }
  

    // Bei falscheingabe des Geschlechts wird das Wert der Methode auf false gesetzt. Dies wird später in der Hauptfunktion abgefragt
}