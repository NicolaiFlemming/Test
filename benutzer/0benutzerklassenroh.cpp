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



