class benutzer
{
    private:
    // Attribute
    int kge;            // Körpergewicht
    char gen;           // Geschlecht  
    float sufflvl;      // Promillegehalt im Blut
    int alkges;         // Insgesamt verzehrter Alkohol
    
    public:  
    //Methoden 
    int get_kge();
    char get_gen();
    int get_alkges();
    
    void set_alkges(int alkges);
    void set_gen(char gen);
    void set_kge(int kge);
    
};

// Methoden definition

int benutzer::get_kge()                 // Methode zum zugriff auf Körpergewicht
{
    return kge;
}

char benutzer::get_gen()                // Methode zum Zugriff auf Geschlecht
{
    return gen;
}


int benutzer::get_alkges()              // Methode zum zugriff auf insgesamt verzehrten Alkohol
{
    return alkges;
}


void benutzer::set_alkges(int alkges)   // Methode zum festlegen des verzehrten Alkohols
{
    this -> alkges = alkges;
}

void benutzer::set_kge(int kge)          // Methode zum festlegen des Körpergewichts
{
    this -> kge =kge;
}

void benutzer::set_gen(char gen)         // Methode zum festlegen des Geschlechts
{
    this -> gen=gen;
}






