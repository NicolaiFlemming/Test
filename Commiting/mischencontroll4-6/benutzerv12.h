class benutzer
{
    private:
    int kge;
    char gen;        
    float sufflvl;
    int alkges;
    
    public:            
    int get_kge();
    char get_gen();
    float get_sufflvl();
    int get_alkges();
    
    void set_alkges(int alkges);
    void set_gen(char gen);
    void set_kge(int kge);
    void err_sufflvl(int kge, char gen,int alkges);
    void set_sufflvl(float sufflvl);
};

int benutzer::get_kge() 
{
    return kge;
}

char benutzer::get_gen()
{
    return gen;
}

float benutzer::get_sufflvl()
{
    return sufflvl;
}

int benutzer::get_alkges()
{
    return alkges;
}


void benutzer::set_alkges(int alkges)
{
    this -> alkges = alkges;
}

void benutzer::set_kge(int kge)
{
    this -> kge =kge;
}

void benutzer::set_gen(char gen)
{
    this -> gen=gen;
}

void benutzer::set_sufflvl(float sufflvl)
{
    this -> sufflvl = sufflvl;
}

void benutzer::err_sufflvl(int kge, char gen, int alkges )
{    
    if (this -> gen == 'm' || this -> gen == 'M')
    {
        this -> sufflvl = this -> alkges / (this -> kge * 0.68);
    }
    if (this -> gen == 'W' || this -> gen == 'w')
    {
        this -> sufflvl = this -> alkges / (this -> kge * 0.55);
    }
}



//anzal gramm durch mei männern kgwe *0,68
//bei frauen*0,55
