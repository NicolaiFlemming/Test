#include <iostream>					// Praeprozessoranweisung

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
    void err_sufflvl(int kge, char gen,int alkges); 
     // set alkgesamt muss im Hauptprogramm interagiertwerden und jedes mal nach Abschluss eines Getränkes erhöht werden.
};

int main(){ 
int auswahl;
benutzer benutzer1;
benutzer benutzer2;
benutzer benutzer3;
benutzer benutzer4;
benutzer benutzer5;
benutzer benutzer6;
cout<<"druecken sie eine Zahl von 1-6 um einen benutzer auszuwaehlen"<<endl;
cin>>auswahl;
switch (auswahl){
    case 1:{
        
        int gew;
        char gesch;      // provisorische Variable für gender zur übergabe an methode
       
        
        cout<<"Koerpergewicht eingeben"<<endl;
        cin>>gew;
        cout<<"bitte Geschlecht angeben [M] || [W]"<<endl;
        cin>> gesch;
        
       

        benutzer1.set_kge(gew);
        benutzer1.set_gen(gesch);
        benutzer1.err_sufflvl(gew, gesch, benutzer1.get_alkges());
       
            cout<<"      Benutzer1:"<<endl;
            cout<<"Gewicht    =    "<<benutzer1.get_kge()<<endl;
            cout<<"Geschlecht =    "<<benutzer1.get_gen()<<endl;
            cout<<"Sufflvl    =    "<<benutzer1.get_sufflvl()<<endl;
        return 0;
       
    }
     case 2:{
        
        int gew;
        char gesch;      // provisorische Variable für gender zur übergabe an methode
       
        
        cout<<"Koerpergewicht eingeben"<<endl;
        cin>>gew;
        cout<<"bitte Geschlecht angeben [M] || [W]"<<endl;
        cin>> gesch;
        
       

        benutzer2.set_kge(gew);
        benutzer2.set_gen(gesch);
        benutzer2.err_sufflvl(gew, gesch, benutzer2.get_alkges());
   
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
void benutzer::err_sufflvl(int kge, char gen, int alkges )
{      
    //Das Errechen des Alkoholgehalts im Blut erforlgt bei Männern und Frauen unterschiedlich, da der Wasser und Fett gehalt des Körpers variiert.
    
    if (this->gen=='m' || this->gen=='M'){
        this-> sufflvl = this->alkges/(this->kge*0.68);
        return true;
    }
    if (this->gen=='W' || this->gen=='w'){
        this->sufflvl = this->alkges/(this->kge*0.55);
        return true;
    }
    
}



