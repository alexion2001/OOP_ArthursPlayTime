/*Aplicatia gestioneaza o afacere cu produse printate 3D.

Reguli de functionare:

-Clase: Produse, Cheltuieli, Vanzari, Comenzi. Pot afisa toate obiectele unei clase.
-Pot adauga un produs, o cheltuiala sau o comanda noua. O noua vanzare se adauga automat atunci cand
am onorat o comanda si am schimbat statusul acelei comenzi in "trimisa".
-Pot afisa toate cheltuielile / vanzarile dintr-o luna si toate comenzile cu un anume deadline.
-Filtru pentru produse - Pot afisa produsele care au anumite proprietati (un anume pret sau stoc )


Clase noi
-Custom (derivata clasei Comenzi); am comenzi speciale, cu dimensiuni custom-> functionalitate- in functie de volum se adauga un cost suplimentar la pretul standard
se pot face interogari si schimbari normale pentru aceste tipuri de comenzi
-Produse_custom (derivata clasei produse)- sunt produsele custom, temporare, care memoreaza toate datele unui produs personalizat. acest tip de produs se adauga obligatoriu atunci
cand avem o comanda custom;
-putem interoga datele acestui tip de produs doar prin id-ul comenzii;


-Filament- retine tipurile de filament, cu toate detaliile acestuia(nume, tem topire, lista de culori)
-Imprimanta- retine toate datele unei imprimante, precum dimensiuni si tip filament -->> functionalitate (verific pe ce imprimante pot printa un produs tinand cont
de dimensiuni si tipul filamentului)

-Clienti( clasa abstracta)
-Persoana fizica si firma sunt clase derivate ale clasei clienti - pt fiecare se calculeaza diferit reducerile si costul transportului
-pot afisa care client a plasat cele mai multe comenzi si pot modifica acest numar

Exista un fisier cu date de intrare pentru cateva clase. Inainte de meniul cu care utilizatorul interactioneaza, se introduc cateva date despre imprimante, produse, cheltuieli si comenzi.
Utilizatorul are posibilitatea de a exporta datele introduse produse si imprimante intr-un fisier text.

Am inlocuit in functiile de afisare ale tuturor claselor vectorul simplu cu cel stl
Am folosit set pt a memora tipuri de filament citite din fisier
Am folosit list pt a memora produsele temporare. cand o comanda custom este trimisa se sterge produsul temporar corespunzator si se adauga o vanzare
Am folosit map pentru a memora un produs la reducere pe o perioada scurta de timp (id produs, noul pret )

*/

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <iterator>
using namespace std;

ifstream f ("date_intrare.txt");
ofstream g ("export.txt");

//clasele mele
class Cheltuieli
{
private:
    string denumire_cheltuiala,data_cheltuirii; //formatul datei este DD/MM/YYYY;
    float valoare_cheltuita;
    const int id_cheltuieli;
    static int contor_id_C, contor_pozitie;



public:
    string get_denumire_cheltuiala();
    string get_data_cheltuirii();
    float  get_valoare_cheltuita();
    int    get_id_cheltuiala();
    int    get_contor();

    void set_denumire_cheltuiala(string nume);
    void set_data_cheltuirii(string data);
    void set_valoare_cheltuita(float val);
    void set_contor();


    Cheltuieli();
    Cheltuieli(string denumire, string data,float valoare);
    Cheltuieli(string denumire, string data);
    Cheltuieli(const Cheltuieli& c);
    Cheltuieli& operator=(const Cheltuieli& c);
    ~Cheltuieli();

    friend ostream& operator<<(ostream& out, const Cheltuieli& c); // operator afisare
    friend istream& operator>>(istream& in,Cheltuieli& c); //operator citire

    friend ofstream& operator<<(ofstream& out, const Cheltuieli& c); // operator afisare fisier
    friend ifstream& operator>>(ifstream& in,Cheltuieli& c); //operator citire fisier

    const Cheltuieli& operator++();  // pre-incremenatre
    const Cheltuieli operator++(int); //post-incrementare

    bool operator==(const Cheltuieli& c);
    bool operator>(const Cheltuieli& c);

    Cheltuieli operator+(Cheltuieli c);
    Cheltuieli operator-(Cheltuieli c);

    explicit operator int();

};

class Vanzari
{ private:
    string denumire_vanzare,data_vanzarii; //formatul datei este DD/MM/YYYY;
    float valoare_vanzare;
    const int id_vanzare;
    static int contor_id_V, contor_pozitie;

public:
    string get_denumire_vanzare();
    string get_data_vanzarii();
    float  get_valoare_vanzare();
    int    get_id_vanzare();
    int    get_contor();


    void set_denumire_vanzare(string nume);
    void set_data_vanzarii(string data);
    void set_valoare_vanzare(float val);
    void set_contor();

    friend ostream& operator<<(ostream& out, const Vanzari& v); // operator afisare
    friend istream& operator>>(istream& in,Vanzari& v); //operator citire

    friend ofstream& operator<<(ofstream& out, const Vanzari& v); // operator afisare
    friend ifstream& operator>>(ifstream& in,Vanzari& v); //operator citire


    Vanzari();
    Vanzari(string denumire, string data,float valoare); //vanzare cu valoare
    Vanzari(string denumire, string data); //vanzare fara valoare
    Vanzari(const Vanzari& v);
    ~Vanzari();

    const Vanzari& operator++();  // pre-incremenatre
    const Vanzari operator++(int); //post-incrementare

    Vanzari& operator=(const Vanzari& v);

    bool operator==(const Vanzari& v);
    bool operator<=(const Vanzari& v);


   Vanzari operator-(Vanzari v);
   Vanzari operator+(Vanzari v);

   explicit operator int();

};

class Comenzi
{
protected: //pt mostenire
    string denumire_comanda,deadline_comanda; //formatul datei este DD/MM/YYYY;
    float valoare_comanda;
    string status_comanda; //neprocesata, executata, trimisa ; daca trimisa -> adaug la vanzare
    const int id_comanda;
    static int contor_id_CO, contor_pozitie;

public:
    string get_denumire_comanda();
    string get_deadline_comanda();
    float  get_valoare_comanda();
    string get_status_comanda();
    int    get_id_comanda();
    int    get_contor();
    int    get_contor_total();


    void set_status_comanda(string status);
    void set_denumire_comanda(string nume);
    void set_deadline_comanda(string data);
    void set_valoare_comanda(float val);
    void set_contor();

    friend ostream& operator<<(ostream& out, const Comenzi& c); // operator afisare
    friend istream& operator>>(istream& in,Comenzi& c); //operator citire

    friend ofstream& operator<<(ofstream& out, const Comenzi& c); // operator afisare
    friend ifstream& operator>>(ifstream& in,Comenzi& c); //operator citire

    Comenzi();
    Comenzi(string denumire, string deadline,float valoare);
    Comenzi(string denumire,float valoare);
    Comenzi(const Comenzi& c);
    virtual ~Comenzi(); //pentru a se dezaloca memoria atunci cand pun obiectul de tip derivat in vectorul de tipul bazei

    const Comenzi& operator++();  // pre-incremenatre
    const Comenzi operator++(int); //post-incrementare
    Comenzi& operator=(const Comenzi& c);

    bool operator==(const Comenzi& c);
    bool operator>=(const Comenzi& c);

    Comenzi operator+(Comenzi c);
    Comenzi operator-(Comenzi c);

    explicit operator int();

};

class Produse
{ protected:
    bool in_vanzare;
    int stoc, numar_culori;
    int* minute_executie;
    float pret;
    float* cost_producere;
    double cantitate_filament;
    string nume_produs;
    string* culori_filament;
    char tip_filament[4];
    char* categorie;
    const int id_produs;
    static int contor_id_P, contor_pozitie;

public:
    bool get_vanzare();
    int get_stoc();
    int* get_minute_executie();
    float get_pret();
    float* get_cost_producere();
    double get_cantitate_filament();
    string get_nume_produs();
    string* get_culori_filament();
    string get_tip_filament();
    char* get_categorie();
    int get_id_produs();
    int    get_contor();
    int get_nr_culori();


    void set_stoc(int stoc);
    void set_vanzare(bool vanzare);
    void set_minute_executie(int* minute);
    void set_pret(float pretul);
    void set_cost_producere(float* cost);
    void set_cantitate_filament(double cant);
    void set_nume_produs(string nume);
    void set_culori_filament(string* culori,int nr);
    void set_tip_filament(char tip[4]);
    void set_categorie(char* categorie);
    void set_contor();
    void set_nr_culori(int nr);

    Produse();
    Produse(int stocul, float pretul, string nume); // fara detalii tehnice ale produselui
    Produse(int stocul, int* minute, float pretul, float* cost, double cantitate,string nume, string* culori, char tip[4], char* categorie, int nr); //toate datele produsului
    Produse(const Produse& p);
    ~Produse();

    friend ostream& operator<<(ostream& out, const Produse& p); // operator afisare
    friend istream& operator>>(istream& in,Produse& p); //operator citire
    friend ofstream& operator<<(ofstream& out, const Produse& p); // operator afisare
    friend ifstream& operator>>(ifstream& in,Produse& p); //operator citire


    const Produse& operator++();  // pre-incremenatre
    const Produse operator++(int); //post-incrementare
    Produse& operator=(const Produse& p);

    bool operator==(const Produse& p);
    bool operator<(const Produse& p);


    friend Produse operator+(Produse p,float x);
    friend Produse operator-(Produse p,float x);
    friend Produse operator*(Produse p,float x);
    friend Produse operator/(Produse p,float x);

    friend Produse operator+(float x, Produse p);
    friend Produse operator*(float x, Produse p);

    explicit operator int();
    string operator[](int index);

};

//clasele derivate- relatia ISA
class Custom:public Comenzi //mosteneste clasa comenzi simple; in clasa derivata am comenzi custom cerute de client
{
private:
    float* dimensiuni; //am adaugat float* care nu era array in etapa1
    //lungime, latime, inaltime ->> se schimba pretul in functie de dimensiuni(functionalitate)

public:
    Custom();
    Custom(string denumire, string deadline,float valoare, float* dim);
    ~Custom();
    Custom(const Custom& c);
    Custom& operator=(const Custom& c);

    friend ostream& operator<<(ostream& out, const Custom& c); // operator afisare
    friend istream& operator>>(istream& in,Custom& c); //operator citire

    friend ofstream& operator<<(ofstream& out, const Custom& c); // operator afisare
    friend ifstream& operator>>(ifstream& in,Custom& c); //operator citire


};

class Produs_custom:public Produse{
private:
    float dimensiuni[3];
    static int contor_pozitie_custom;
    int id_comanda;

public:
    Produs_custom();
    Produs_custom(int stocul, int* minute, float pretul, float* cost, double cantitate,string nume, string* culori, char tip[4], char* categorie, int nr, float dim[3], int id);
    ~Produs_custom();
    Produs_custom(const Produs_custom& c);
    Produs_custom& operator=(const Produs_custom& c);

    friend ostream& operator<<(ostream& out, const Produs_custom& c); // operator afisare
    friend istream& operator>>(istream& in,Produs_custom& c); //operator citire

    friend ofstream& operator<<(ofstream& out, const Produs_custom& c); // operator afisare
    friend ifstream& operator>>(ifstream& in,Produs_custom& c); //operator citire

    void set_id(int id);
    int get_id();
    void set_contor_custom();
    int  get_contor_custom();

};

 //clasa abastracta, interfata
class Transport{ //interfata
public:
    virtual void tip_client() {cout<<"(Client)  ";}
    virtual void cost_transport()=0;
};

class Clienti:public Transport{ // clasa abstracta
protected:
    string nume, telefon;
    int nr_comenzi;
    float reducere; //suma voucher calculata in fct de nr de comenzi
    static int contor_pozitie;

public:
    Clienti();
    Clienti(string nume,string telefon,int nr, float reducere);
    Clienti(const Clienti& c);
    Clienti& operator=(const Clienti& c);
    ~Clienti();

    int get_nr_comenzi();
    string get_telefon();

    void set_nr_comenzi(int nr);

    virtual void calcul_reducere()=0;

    friend ostream& operator<<(ostream& out, const Clienti& c);
    friend istream& operator>>(istream& in,Clienti& c);

    virtual ostream& AfisareVirtuala(ostream& out)const;
    virtual istream& CitireVirtuala (istream& in);

    int get_contor();
    void set_contor();
};

class Persoana_fizica:public Clienti{
private:
    string prenume;

public:
    Persoana_fizica();
    Persoana_fizica(string nume,string telefon,int nr, float reducere, string prenume);
    Persoana_fizica(const Persoana_fizica& c);
    Persoana_fizica& operator=(const Persoana_fizica& c);
    ~Persoana_fizica();

    void calcul_reducere(){reducere=nr_comenzi*0.25;}
    void cost_transport(){
    if(reducere>=20) cout<<"Transport gratuit";
    else {cout<<"\nCostul transportului este de 20 de lei, dar acest client are reducere de "<<reducere<<" lei.";}
    }
    void tip_client() {cout<<"(Persoana fizica)  ";}



    friend ostream& operator<<(ostream& out, const Persoana_fizica& c);
    friend istream& operator>>(istream& in,Persoana_fizica& c);

    virtual ostream& AfisareVirtuala(ostream& out)const;
    virtual istream& CitireVirtuala (istream& in);
};
class Firma:public Clienti{
private:
    string cod_fiscal;

public:
    Firma();
    Firma(string nume,string telefon,int nr,float reducere,string cod_fiscal);
    Firma(const Firma& c);
    Firma& operator=(const Firma& c);
    ~Firma();

    void calcul_reducere(){reducere=nr_comenzi*0.50;}
    void cost_transport(){
    cout<<"\nTransportul este gratuit pentru firme.";cout<<"\nAceasta firma beneficiaza si de o reducere de "<<reducere<<" lei"; }
    void tip_client() {cout<<"(Firma)  ";}


    friend ostream& operator<<(ostream& out, const Firma& c);
    friend istream& operator>>(istream& in,Firma& c);

    virtual ostream& AfisareVirtuala(ostream& out)const;
    virtual istream& CitireVirtuala (istream& in);
};

//has a - compunere
class Filament{
private:
    string tip;
    string* culori;
    float temperatura_topire;
    int nr_culori;

public:
    Filament();
    Filament(string tip,int nr_culori,float temp,string* culori);
    ~Filament();
    Filament(const Filament& f);
    Filament& operator=(const Filament& f);

    void set_tip(string tipul);
    void set_temperatura( float temp);
    void set_culori(int nr, string* cul);

    string get_tip();
    string* get_culori();
    float get_temperatura();
    int get_nr_culori();

    friend ostream& operator<<(ostream& out, const Filament& f);
    friend istream& operator>>(istream& in,Filament& f);
    friend ofstream& operator<<(ofstream& out, const Filament& f);
    friend ifstream& operator>>(ifstream& in,Filament& f);

    void citeste();

};
class Imprimante{
private:
    string nume;
    float dimensiuni[3];
    int nr_tipuri;
    Filament* filament_compatibil;
    static int contor_pozitie;


public:
    Imprimante();
    Imprimante(string nume,float dim[3],int nr, Filament* tip_compatibil);
    ~Imprimante();
    Imprimante(const Imprimante& i);
    Imprimante& operator=(const Imprimante& i);

    friend ostream& operator<<(ostream& out, const Imprimante& i);
    friend istream& operator>>(istream& in,Imprimante& i);

    friend ofstream& operator<<(ofstream& out, const Imprimante& i);
    friend ifstream& operator>>(ifstream& in,Imprimante& i);

    void set_contor();
    int get_contor();

    string get_nume();
    float get_dimensiuni();
    int get_nr_tipuri();
    string get_filament_compatibil(int i);

    void citeste_fisier();

};

//initializari contor
int Cheltuieli::contor_id_C=0;
int Vanzari::contor_id_V=0;
int Comenzi::contor_id_CO=0;
int Produse::contor_id_P=0;


int Cheltuieli::contor_pozitie=0;
int Comenzi::contor_pozitie=0;
int Vanzari::contor_pozitie=0;
int Produse::contor_pozitie=0;

int Clienti::contor_pozitie=0;
int Produs_custom::contor_pozitie_custom=0;
int Imprimante::contor_pozitie=0;


//declarari vectori de obiecte pt toate clasele
Cheltuieli cheltuieli[1000];
Vanzari vanzari[1000];
Comenzi* comenzi[1000];
Produse produse[1000];
Produs_custom produse_temporare[100];
Imprimante imprimante[10];
Clienti* clienti[1000];

//-vector stl
vector <Cheltuieli> vct_cheltuieli;
vector <Vanzari> vct_vanzari;
vector <Comenzi*> vct_comenzi;
vector <Produse> vct_produse;
vector <Imprimante> vct_imprimante;
vector <Clienti*> vct_clienti;
list <Produs_custom> lista_produse_temporare;

//set, map
 map<int, float> reduceri; //id, noul pret
 set<string> filament; //o lista cu tipurile de filament pe care le dentin

//functii de validare a datelor
bool format_valid_data(string data) //verifica daca o data este in formatul stabilit si este valida
{ bool ok=1;
   if(data[2]!='/' || data[5]!='/') ok=0;
   else
   {
       int zi,luna,an;
       zi=((int)data[0]-48)*10 + (int)data[1]-48;
       luna=((int)data[3]-48)*10 + (int)data[4]-48;
       an=((int)data[6]-48)*1000 + ((int)data[7]-48)*100+((int)data[8]-48)*10+(int)data[9]-48;
       if (luna>12 || luna<1)ok=0;
       if (an<2000) ok=0;
       if (zi<1 || zi>31) ok=0;
   }
    return ok;
}
//getters
string Cheltuieli::get_denumire_cheltuiala(){return denumire_cheltuiala;}
string Cheltuieli::get_data_cheltuirii(){return data_cheltuirii;}
float Cheltuieli::get_valoare_cheltuita(){return valoare_cheltuita;}
int Cheltuieli::get_id_cheltuiala(){return id_cheltuieli;}
int Cheltuieli::get_contor(){return contor_pozitie;}


string Vanzari::get_denumire_vanzare(){return denumire_vanzare;}
string Vanzari::get_data_vanzarii(){return data_vanzarii;}
float  Vanzari::get_valoare_vanzare(){return valoare_vanzare;}
int Vanzari::get_id_vanzare(){return id_vanzare;}
int Vanzari::get_contor(){return contor_pozitie;}

string Comenzi::get_denumire_comanda(){return denumire_comanda;}
string Comenzi::get_deadline_comanda(){return deadline_comanda;}
float  Comenzi::get_valoare_comanda(){return valoare_comanda;}
string Comenzi::get_status_comanda(){return status_comanda;}
int Comenzi::get_id_comanda(){return id_comanda;}
int Comenzi::get_contor(){return contor_pozitie;}
int Comenzi::get_contor_total(){return contor_id_CO;}

bool    Produse::get_vanzare(){return in_vanzare;}
int     Produse::get_stoc(){return stoc;}
int*    Produse::get_minute_executie(){return this->minute_executie;}
float   Produse::get_pret(){return pret;}
float*  Produse::get_cost_producere(){return this->cost_producere;}
double  Produse::get_cantitate_filament(){return cantitate_filament;}
string  Produse::get_nume_produs(){return nume_produs;}
string* Produse::get_culori_filament(){return this->culori_filament;}
string    Produse::get_tip_filament(){ string aux(tip_filament); return aux;}
char*   Produse::get_categorie(){return this->categorie;}
int Produse::get_id_produs(){return id_produs;}
int Produse::get_contor(){return contor_pozitie;}
int Produse::get_nr_culori(){return numar_culori;}

int Produs_custom::get_id(){return id_comanda;}
int Produs_custom::get_contor_custom(){return contor_pozitie_custom;}

string Filament::get_tip(){return tip;}
string* Filament::get_culori(){return this->culori;}
float Filament::get_temperatura(){return temperatura_topire;}
int Filament::get_nr_culori(){return nr_culori;}

int Imprimante::get_contor(){return contor_pozitie;}
string Imprimante::get_nume(){return nume;}
float Imprimante::get_dimensiuni(){return dimensiuni[3];}
int Imprimante::get_nr_tipuri(){return nr_tipuri;}
string Imprimante::get_filament_compatibil(int i){return filament_compatibil[i].get_tip();}

int Clienti::get_nr_comenzi() {return nr_comenzi;}
int Clienti::get_contor() {return contor_pozitie;}
string Clienti::get_telefon(){return telefon;}



//setters
void Cheltuieli::set_denumire_cheltuiala(string nume){denumire_cheltuiala=nume;}
void Cheltuieli::set_data_cheltuirii(string data){data_cheltuirii=data;}
void Cheltuieli::set_valoare_cheltuita(float val){valoare_cheltuita=val;}
void Cheltuieli::set_contor(){contor_pozitie++;}

void Vanzari::set_denumire_vanzare(string nume){denumire_vanzare=nume;}
void Vanzari::set_data_vanzarii(string data){data_vanzarii=data;}
void Vanzari::set_valoare_vanzare(float val){valoare_vanzare=val;}
void Vanzari::set_contor(){contor_pozitie++;}

void Comenzi::set_status_comanda(string status){ status_comanda=status; } //comanda nepreluata, executata sau trimisa
void Comenzi::set_denumire_comanda(string nume){denumire_comanda=nume;}
void Comenzi::set_deadline_comanda(string data){deadline_comanda=data;}
void Comenzi::set_valoare_comanda(float val){valoare_comanda=val;}
void Comenzi::set_contor(){contor_pozitie++;}

void Produse::set_contor(){contor_pozitie++;}
void Produse::set_vanzare(bool vanzare) {in_vanzare=vanzare;} //retragere/adaugare produs din vanzare (0/1)
void Produse::set_stoc(int stoc_nou) {stoc=stoc_nou;} //schimbare stoc produs
void Produse::set_minute_executie(int* minute){ minute_executie=minute;}
void Produse::set_pret(float pretul){pret=pretul;}
void Produse::set_cost_producere(float* cost){cost_producere=cost;}
void Produse::set_cantitate_filament(double cant){cantitate_filament=cant;}
void Produse::set_nume_produs(string nume){nume_produs=nume;}
void Produse::set_culori_filament(string* culori,int nr)
{   this->numar_culori=nr;
    if(this->culori_filament!=NULL)
        {delete[] this->culori_filament;culori_filament=new string[nr];}

   culori_filament=new string[nr];
      for(int i=0;i<nr;i++)
        this->culori_filament[i]=culori[i];
}
void Produse::set_tip_filament(char tip[4]){ strcpy(tip_filament,tip);}
void Produse::set_categorie(char* categorie)
{
    if(this->categorie!=NULL)
        delete[] this->categorie;
    this->categorie=new char[strlen(categorie)+1];
    strcpy(this->categorie,categorie);}
void Produse::set_nr_culori(int nr){numar_culori=nr;}

void Produs_custom::set_contor_custom(){contor_pozitie_custom++;}
void Produs_custom::set_id(int id){id_comanda=id;}

void Filament::set_tip(string tipul){tip=tipul;}
void Filament::set_temperatura( float temp){temperatura_topire=temp;}
void Filament::set_culori(int nr, string* cul){
    nr_culori=nr;
    if(this->culori!=NULL)
        {delete[] this->culori;culori=new string[nr];}

   culori=new string[nr];
      for(int i=0;i<nr;i++)
        this->culori[i]=cul[i];}


void Imprimante::set_contor(){ contor_pozitie++;}

void Clienti::set_contor(){contor_pozitie++;}
void Clienti::set_nr_comenzi(int nr){nr_comenzi=nr;}

//constructori
Cheltuieli::Cheltuieli():id_cheltuieli(contor_id_C++) //constructor fara parametrii pt cheltuieli
    {
        denumire_cheltuiala="cheltuiala noua nedefinita";
        data_cheltuirii="dd/mm/yyyy";
        valoare_cheltuita=0.00;

    }
Cheltuieli::Cheltuieli(string denumire, string data,float valoare):id_cheltuieli(contor_id_C++) //constructor cu parametrii pt adaugarea unei cheltuieli cu valoare
    {
        denumire_cheltuiala=denumire;
        data_cheltuirii=data;
        valoare_cheltuita=valoare;
    }
Cheltuieli::Cheltuieli(string denumire, string data):id_cheltuieli(contor_id_C++) //constructor cu parametrii pt adaugarea unei cheltuieli fara valoare
    {
        denumire_cheltuiala=denumire;
        data_cheltuirii=data;
    }
Comenzi::Comenzi():id_comanda(contor_id_CO++) //constructor fara parametrii pt comenzi
        {denumire_comanda="comanda noua nedefinita";
        deadline_comanda="nu are";
        valoare_comanda=0.00;
        status_comanda="nepreluata";
}
Comenzi::Comenzi(string denumire, string deadline,float valoare):id_comanda(contor_id_CO++) //constructor cu parametrii pt adaugarea unei comnzi cu deadline
    {
        denumire_comanda=denumire;
        deadline_comanda=deadline;
        valoare_comanda=valoare;
    }
Comenzi::Comenzi(string denumire, float valoare):id_comanda(contor_id_CO++) //constructor cu parametrii pt adaugarea unei comenzi fara deadline
    {
        denumire_comanda=denumire;
        valoare_comanda=valoare;
    }

Vanzari::Vanzari():id_vanzare(contor_id_V++) //constructor fara parametrii pt vanzari
    {
        denumire_vanzare="vanzare noua nedefinita";
        data_vanzarii="dd/mm/yyyy";
        valoare_vanzare=0.00;


    }
Vanzari::Vanzari(string denumire, string data,float valoare):id_vanzare(contor_id_V++) //constructor cu parametrii pt adaugarea unei vanzari cu valoare
    {
        denumire_vanzare=denumire;
        data_vanzarii=data;
        valoare_vanzare=valoare;
    }
Vanzari::Vanzari(string denumire, string data):id_vanzare(contor_id_V++) //constructor cu parametrii pt adaugarea unei vanzari fara valoare
    {
        denumire_vanzare=denumire;
        data_vanzarii=data;
    }
Produse::Produse():id_produs(contor_id_P++)
{
  this->in_vanzare=1;
  this->stoc=0;
  this->minute_executie=0;
  this->pret=0.0;
  this->cost_producere=0;
  this->cantitate_filament=0;
  this->nume_produs="fara nume";
  this->categorie=new char[strlen("nu stiu")+1];
  strcpy( this->categorie, "nu stim");
  strcpy( this->tip_filament, "PLA");
  this->numar_culori=0;
  this->culori_filament=NULL;
}
Produse::Produse(int stocul, float pretul, string nume):id_produs(contor_id_P++)
{ stoc=stocul;
  pret=pretul;
  nume_produs=nume;

  this->in_vanzare=1;
  this->minute_executie=NULL;
  this->cost_producere=NULL;
  this->cantitate_filament=0;
  this->categorie=new char[strlen("nu stiu")+1];
  strcpy( this->categorie, "nu stim");
  strcpy( this->tip_filament, "PLA");
  this->numar_culori=0;
  this->culori_filament=NULL;

} // fara detalii tehnice ale produsului
Produse::Produse(int stocul, int* minute, float pretul, float* cost, double cantitate,string nume, string* culori, char tip[4], char* categorie,int nr):id_produs(contor_id_P++)
{ stoc=stocul;
  this->minute_executie=minute;
  pret=pretul;
  this->cost_producere=cost;
  cantitate_filament=cantitate;
  nume_produs=nume;
  this->categorie=new char[strlen(categorie)+1];
  strcpy( this->categorie, categorie);
  strcpy( this->tip_filament, tip);
  this->numar_culori=nr;
  this->in_vanzare=1;

  culori_filament=new string[numar_culori];
  for(int i=0;i<numar_culori;i++)
  this->culori_filament[i]=culori[i];
} //toate datele produsului

Custom::Custom():Comenzi(){
this->dimensiuni=new float[3];
this->dimensiuni[0]=0.0;
this->dimensiuni[1]=0.0;
this->dimensiuni[2]=0.0;
}
Custom::Custom(string denumire, string deadline,float valoare, float dim[]):Comenzi(denumire,deadline,valoare){
this->dimensiuni= new float[3];
this->dimensiuni[0]=dim[0];
this->dimensiuni[1]=dim[1];
this->dimensiuni[2]=dim[2];
    }

Produs_custom::Produs_custom():Produse(){
dimensiuni[0]=0.0;
dimensiuni[1]=0.0;
dimensiuni[2]=0.0;

id_comanda=0;
}
Produs_custom::Produs_custom(int stocul, int* minute, float pretul, float* cost, double cantitate,string nume, string* culori, char tip[4], char* categorie, int nr, float dim[3],int id):Produse(stocul,minute,pretul,cost,cantitate,nume,culori,tip,categorie, nr){
dimensiuni[0]=dim[0];
dimensiuni[1]=dim[1];
dimensiuni[2]=dim[2];

id_comanda=id;
}

Filament::Filament()
{
    tip="PLA";
    nr_culori=0;
    temperatura_topire=0.0;
    this->culori=NULL;

}
Filament::Filament(string tipul,int nr,float temp,string* culori)
{
    tip=tipul;
    nr_culori=nr;
    temperatura_topire=temp;
    this->culori=new string[nr];
    for(int i=0;i<nr_culori;i++)
      this->culori[i]=culori[i];

}

Imprimante::Imprimante(){
nume="anonim";
dimensiuni[0]=0.0;
dimensiuni[1]=0.0;
dimensiuni[2]=0.0;
nr_tipuri=0;
this->filament_compatibil=NULL;
 }

Imprimante::Imprimante(string nume,float dim[3],int nr, Filament* tip_compatibil){
nume=nume;
dimensiuni[0]=dim[0];
dimensiuni[1]=dim[1];
dimensiuni[2]=dim[2];
nr_tipuri=nr;
this->filament_compatibil=new Filament[nr];
for(int i=0;i<nr_tipuri;i++)
    filament_compatibil[i]=tip_compatibil[i];


}
Clienti::Clienti(){
nume="Anonim";
telefon="-";
nr_comenzi=0;
reducere=0;
}
Clienti::Clienti(string nume,string telefon,int nr, float reducere){
nume=nume;
telefon=telefon;
nr_comenzi=nr;
reducere=reducere;
}

Persoana_fizica::Persoana_fizica():Clienti(){
prenume="anonim";
}
Persoana_fizica::Persoana_fizica(string nume,string telefon,int nr, float reducere, string prenume):Clienti(nume,telefon,nr,reducere){
prenume=prenume;
}

Firma::Firma():Clienti(){
cod_fiscal="nu are";
}
Firma::Firma(string nume,string telefon,int nr, float reducere, string cod_fiscal):Clienti(nume,telefon,nr,reducere){
cod_fiscal=cod_fiscal;
}




// copy c
Cheltuieli::Cheltuieli(const Cheltuieli& c):id_cheltuieli(c.id_cheltuieli)
{
    this->denumire_cheltuiala=c.denumire_cheltuiala;
    this->data_cheltuirii=c.data_cheltuirii;
    this->valoare_cheltuita=c.valoare_cheltuita;
}

Comenzi::Comenzi(const Comenzi& c):id_comanda(c.id_comanda)
{
    this->denumire_comanda=c.denumire_comanda;
    this->deadline_comanda=c.deadline_comanda;
    this->valoare_comanda=c.valoare_comanda;
}

Vanzari::Vanzari(const Vanzari& v):id_vanzare(v.id_vanzare)
{
    this->denumire_vanzare=v.denumire_vanzare;
    this->data_vanzarii=v.data_vanzarii;
    this->valoare_vanzare=v.valoare_vanzare;
}
Produse::Produse(const Produse& p):id_produs(p.id_produs)
{
  this->in_vanzare=p.in_vanzare;
  this->stoc=p.stoc;
  this->minute_executie=p.minute_executie;
  this->pret=p.pret;
  this->cost_producere=p.cost_producere;
  this->cantitate_filament=p.cantitate_filament;
  this->nume_produs=p.nume_produs;
  this->categorie=new char[strlen(p.categorie)+1];
  strcpy( this->categorie, p.categorie);
  strcpy( this->tip_filament, p.tip_filament);

   this->numar_culori=p.numar_culori;
  culori_filament=new string[p.numar_culori];
      for(int i=0;i<numar_culori;i++)
        this->culori_filament[i]=p.culori_filament[i];
}

Custom::Custom(const Custom& c):Comenzi(c)
{
this->dimensiuni=new float[3];
this->dimensiuni[0]=c.dimensiuni[0];
this->dimensiuni[1]=c.dimensiuni[1];
this->dimensiuni[2]=c.dimensiuni[2];

}

Produs_custom::Produs_custom(const Produs_custom& c):Produse(c)
{

this->dimensiuni[0]=c.dimensiuni[0];
this->dimensiuni[1]=c.dimensiuni[1];
this->dimensiuni[2]=c.dimensiuni[2];
this->id_comanda=c.id_comanda;

}
Filament::Filament(const Filament& f)
{
    this->tip=f.tip;
    this->nr_culori=f.nr_culori;
    this->temperatura_topire=f.temperatura_topire;
    this->culori=new string[f.nr_culori];
    for(int i=0;i<f.nr_culori;i++)
      this->culori[i]=f.culori[i];

}
Imprimante::Imprimante(const Imprimante& i)
{
this->nume=nume;
this->dimensiuni[0]=i.dimensiuni[0];
this->dimensiuni[1]=i.dimensiuni[1];
this->dimensiuni[2]=i.dimensiuni[2];
this->nr_tipuri=nr_tipuri;
this->filament_compatibil=new Filament[i.nr_tipuri];
for(int j=0;j<i.nr_tipuri;j++)
    this->filament_compatibil[j]=i.filament_compatibil[j];

}
Clienti::Clienti(const Clienti& c)
{
this->nume=c.nume;
this->telefon=c.telefon;
this->nr_comenzi=c.nr_comenzi;
this->reducere=c.reducere;
}

Persoana_fizica::Persoana_fizica(const Persoana_fizica& c):Clienti(c)
{
    this->prenume=c.prenume;
}
Firma::Firma(const Firma& c):Clienti(c)
{
    this->cod_fiscal=c.cod_fiscal;
}




// OPERATOR =

Cheltuieli& Cheltuieli::operator=(const Cheltuieli& c)
{
   if(this!=&c)
   {
    this->denumire_cheltuiala=c.denumire_cheltuiala;
    this->data_cheltuirii=c.data_cheltuirii;
    this->valoare_cheltuita=c.valoare_cheltuita;
   }

    return *this;
}

Comenzi& Comenzi::operator=(const Comenzi& c)
{
   if(this!=&c)
   {
    this->denumire_comanda=c.denumire_comanda;
    this->deadline_comanda=c.deadline_comanda;
    this->valoare_comanda=c.valoare_comanda;
   }

    return *this;
}

Vanzari& Vanzari::operator=(const Vanzari& v)
{
   if(this!=&v)
   {
    this->denumire_vanzare=v.denumire_vanzare;
    this->data_vanzarii=v.data_vanzarii;
    this->valoare_vanzare=v.valoare_vanzare;
   }

    return *this;
}
Produse& Produse::operator=(const Produse& p)
{
   if(this!=&p)
   {

       if(this->cost_producere!=NULL)
            delete[] this->cost_producere;

        if(this->minute_executie!=NULL)
            delete[] this->minute_executie;

        if(this->categorie!=NULL)
            delete[] this->categorie;

        if(this->culori_filament!=NULL)
            delete[] this->culori_filament;

      this->in_vanzare=p.in_vanzare;
      this->stoc=p.stoc;
      this->minute_executie=p.minute_executie;
      this->pret=p.pret;
      this->cost_producere=p.cost_producere;
      this->cantitate_filament=p.cantitate_filament;
      this->nume_produs=p.nume_produs;
      this->categorie=new char[strlen(p.categorie)+1];
      strcpy( this->categorie, p.categorie);
      strcpy( this->tip_filament, p.tip_filament);

      this->numar_culori=p.numar_culori;
      culori_filament=new string[p.numar_culori];
      for(int i=0;i<numar_culori;i++)
        this->culori_filament[i]=p.culori_filament[i];
    }


    return *this;
}
Custom& Custom::operator=(const Custom& c)
{
     if(this!=&c)
        {
            Comenzi::operator=(c);
            delete [] this->dimensiuni; this->dimensiuni=new float[3];
            this->dimensiuni[0]=c.dimensiuni[0];
            this->dimensiuni[1]=c.dimensiuni[1];
            this->dimensiuni[2]=c.dimensiuni[2];


        }
        return *this;
}
Produs_custom& Produs_custom::operator=(const Produs_custom& c)
{
if(this!=&c)
        {
            Produse::operator=(c);
            this->dimensiuni[0]=c.dimensiuni[0];
            this->dimensiuni[1]=c.dimensiuni[1];
            this->dimensiuni[2]=c.dimensiuni[2];
            this->id_comanda=c.id_comanda;


        }
    return *this;
}

Filament& Filament::operator=(const Filament& f)
{
    if(this!=&f)
    {this->tip=f.tip;
    this->nr_culori=f.nr_culori;
    this->temperatura_topire=f.temperatura_topire;
    this->culori=new string[f.nr_culori];
    for(int i=0;i<f.nr_culori;i++)
      this->culori[i]=f.culori[i];}

      return *this;

}
Imprimante& Imprimante::operator=(const Imprimante& i)
{
     if(this!=&i){
this->nume=nume;
this->dimensiuni[0]=i.dimensiuni[0];
this->dimensiuni[1]=i.dimensiuni[1];
this->dimensiuni[2]=i.dimensiuni[2];
this->nr_tipuri=nr_tipuri;
this->filament_compatibil=new Filament[i.nr_tipuri];
for(int j=0;j<i.nr_tipuri;j++)
    this->filament_compatibil[j]=i.filament_compatibil[j];}

    return *this;

}
Clienti& Clienti::operator=(const Clienti& c)
{
    if(this!=&c)
    {
this->nume=c.nume;
this->telefon=c.telefon;
this->nr_comenzi=c.nr_comenzi;
this->reducere=c.reducere;
    }
    return *this;
}
Persoana_fizica& Persoana_fizica::operator=(const Persoana_fizica& c)
{if(this!=&c)
    {
Clienti::operator=(c);
 this->prenume=c.prenume;
    }
    return *this;

}
Firma& Firma::operator=(const Firma& c)
{if(this!=&c)
    {
        Clienti::operator=(c);
this->cod_fiscal=c.cod_fiscal;
    }
    return *this;

}
//destructori
Cheltuieli::~Cheltuieli() { /* acest destructor nu face nimic*/}
Vanzari::~Vanzari() { /* acest destructor nu face nimic*/}
Comenzi::~Comenzi() { /* acest destructor nu face nimic*/}
Custom::~Custom() { if(this->dimensiuni!=NULL) delete[] this->dimensiuni;}
Produse::~Produse()
{
        if(this->cost_producere!=NULL)
            delete[] this->cost_producere;

        if(this->minute_executie!=NULL)
            delete[] this->minute_executie;

        if(this->categorie!=NULL)
            delete[] this->categorie;

        if(this->culori_filament!=NULL)
            delete[] this->culori_filament;
}
Produs_custom::~Produs_custom(){ /* acest destructor nu face nimic*/}
Filament::~Filament(){
    if(this->culori!=NULL)
            delete[] this->culori;
}
Imprimante::~Imprimante(){
    if(this->filament_compatibil!=NULL)
            delete[] this->filament_compatibil;
}

Clienti::~Clienti(){ /* acest destructor nu face nimic*/}
Persoana_fizica::~Persoana_fizica(){ /* acest destructor nu face nimic*/}
Firma::~Firma(){ /* acest destructor nu face nimic*/}

//supraincarcare << (afisare un obiect)
ostream& operator<<(ostream& out, const Cheltuieli& c)
{
    out<<"In data de "<<c.data_cheltuirii<<"(id:"<<c.id_cheltuieli<<") s-a cheltuit valoarea de "<<c.valoare_cheltuita<<" RON pentru "<<c.denumire_cheltuiala<<endl;
    return out;
}
ostream& operator<<(ostream& out, const Comenzi& c)
{
    out<<"S-a adaugat o comanda pentru "<<c.denumire_comanda<<"(id:"<<c.id_comanda<<") in valoarea de "<<c.valoare_comanda<<" RON cu deadline "<<c.deadline_comanda<<endl;
    return out;
}

ostream& operator<<(ostream& out, const Vanzari& v)
{
    out<<"In data de "<<v.data_vanzarii<<"(id:"<<v.id_vanzare<<") s-a incasat valoarea de "<<v.valoare_vanzare<<" RON pentru "<<v.denumire_vanzare<<endl;
    return out;
}
ostream& operator<<(ostream& out, const Produse& p)
{   out<<"\nProdusul ("<<p.id_produs<<") "<<p.nume_produs<<" costa "<<p.pret<<" RON si are stocul: "<<p.stoc;
    out<<"\nDetalii produs:";
    if (p.in_vanzare==1) {out<<"\nIn vanzare:DA";}
    else {out<<"\nIn vanzare:NU";}
    out<<"\nCategorie: "<<p.categorie;
    out<<"\nTipul si cantitatea filamentului: "<<p.tip_filament<<", "<<p.cantitate_filament;
    out<<"\nDurata de executie(in minute) si costul de producere: "<<*(p.minute_executie)<<", "<<*(p.cost_producere);
    out<<"\nCulori disponibile: ";
    for(int i=0; i<p.numar_culori; i++)
        out<<p.culori_filament[i]<<" ";
    return out;}

ostream& operator<<(ostream& out, const Custom& c)
{
    out<<(Comenzi&)c;
    out<<"\nDimensiuni(L,l,h):";
    out<<c.dimensiuni[0]<<", "<<c.dimensiuni[1]<<", "<<c.dimensiuni[2]<<endl;
    return out;
}


ostream& operator<<(ostream& out, const Produs_custom& c)
{
    out<<(Produse&)c;
    out<<"\nDimensiuni(L,l,h):";
    out<<c.dimensiuni[0]<<", "<<c.dimensiuni[1]<<", "<<c.dimensiuni[2]<<endl;
    return out;
}
ostream& operator<<(ostream& out, const Filament& f)
{
    out<<"\nTipul filamentului(PLA,TPU etc): "<<f.tip;
    out<<"\nTemperatura de topire: "<<f.temperatura_topire;
    out<<"\nCulorile disponibile: ";
    for(int i=0;i<f.nr_culori;i++) out<<f.culori[i]<<" ";
    return out;

}
ostream& operator<<(ostream& out, const Imprimante& i)
{
    out<<"\nNumele imprimantei este "<<i.nume;
    out<<"\nDimensiunea patului - dimensiuni de printare (L,l,h): "<<i.dimensiuni[0]<<", "<<i.dimensiuni[1]<<", "<<i.dimensiuni[2];
    out<<"\nTipuri de filament compatibil: ";
    for(int j=0;j<i.nr_tipuri;j++) out<<i.filament_compatibil[j]<<endl;
    return out;
}

//afisare virtuala

ostream& Clienti::AfisareVirtuala (ostream& out) const{

    out<<"\nNume: "<<nume;
    out<<"\nNumar telefon: "<<telefon;
    out<<"\nNumar comenzi date: "<<nr_comenzi;
    return out;
}

ostream& Persoana_fizica::AfisareVirtuala (ostream& out) const{

    out<<"\nPrenume: "<<prenume;
    Clienti::AfisareVirtuala(out);
    out<<endl;

    return out;
}
ostream& Firma::AfisareVirtuala (ostream& out) const{


    Clienti::AfisareVirtuala(out);
    out<<"\nCod fiscal: "<<cod_fiscal;
    out<<endl;

    return out;


}


ostream& operator<<(ostream& out, const Clienti& c)
{
    return c.AfisareVirtuala(out);
}

ostream& operator<<(ostream& out, const Persoana_fizica& c)
{
return c.AfisareVirtuala(out);

}

ostream& operator<<(ostream& out, const Firma& c)
{
return c.AfisareVirtuala(out);

}




//afisare in fisier

ofstream& operator<<(ofstream& out, const Cheltuieli& c)
{
    out<<"In data de "<<c.data_cheltuirii<<"(id:"<<c.id_cheltuieli<<") s-a cheltuit valoarea de "<<c.valoare_cheltuita<<" RON pentru "<<c.denumire_cheltuiala<<endl;
    return out;
}
ofstream& operator<<(ofstream& out, const Comenzi& c)
{
    out<<"S-a adaugat o comanda pentru "<<c.denumire_comanda<<"(id:"<<c.id_comanda<<") in valoarea de "<<c.valoare_comanda<<" RON cu deadline "<<c.deadline_comanda<<endl;
    return out;
}

ofstream& operator<<(ofstream& out, const Vanzari& v)
{
    out<<"In data de "<<v.data_vanzarii<<"(id:"<<v.id_vanzare<<") s-a incasat valoarea de "<<v.valoare_vanzare<<" RON pentru "<<v.denumire_vanzare<<endl;
    return out;
}
ofstream& operator<<(ofstream& out, const Produse& p)
{   out<<"\nProdusul ("<<p.id_produs<<") "<<p.nume_produs<<" costa "<<p.pret<<" RON si are stocul: "<<p.stoc;
    out<<"\nDetalii produs:";
    if (p.in_vanzare==1) {out<<"\nIn vanzare:DA";}
    else {out<<"\nIn vanzare:NU";}
    out<<"\nCategorie: "<<p.categorie;
    out<<"\nTipul si cantitatea filamentului: "<<p.tip_filament<<", "<<p.cantitate_filament;
    out<<"\nDurata de executie(in minute) si costul de producere: "<<*(p.minute_executie)<<", "<<*(p.cost_producere);
    out<<"\nCulori disponibile: ";
    for(int i=0; i<p.numar_culori; i++)
        out<<p.culori_filament[i]<<" ";
    return out;}

ofstream& operator<<(ofstream& out, const Custom& c)
{
    out<<(Comenzi&)c;
    out<<"\nDimensiuni(L,l,h):";
    out<<c.dimensiuni[0]<<", "<<c.dimensiuni[1]<<", "<<c.dimensiuni[2]<<endl;
    return out;
}


ofstream& operator<<(ofstream& out, const Produs_custom& c)
{
    out<<(Produse&)c;
    out<<"\nDimensiuni(L,l,h):";
    out<<c.dimensiuni[0]<<", "<<c.dimensiuni[1]<<", "<<c.dimensiuni[2]<<endl;
    return out;
}
ofstream& operator<<(ofstream& out, const Filament& f)
{
    out<<"\nTipul filamentului(PLA,TPU etc): "<<f.tip;
    out<<"\nTemperatura de topire: "<<f.temperatura_topire;
    out<<"\nCulorile disponibile: ";
    for(int i=0;i<f.nr_culori;i++) out<<f.culori[i]<<" ";
    return out;

}
ofstream& operator<<(ofstream& out, const Imprimante& i)
{
    out<<"\nNumele imprimantei este "<<i.nume;
    out<<"\nDimensiunea patului - dimensiuni de printare (L,l,h): "<<i.dimensiuni[0]<<", "<<i.dimensiuni[1]<<", "<<i.dimensiuni[2];
    out<<"\nTipuri de filament compatibil: ";
    for(int j=0;j<i.nr_tipuri;j++) out<<i.filament_compatibil[j]<<endl;
    return out;
}

//supraincarcare >> (citirea un obiect)
istream& operator>>(istream& in, Cheltuieli& c)
{  string aux;

    cout<<"\nIntrodu denumirea:";in>>c.denumire_cheltuiala;
    cout<<"\nIntrodu valoarea:";in>>c.valoare_cheltuita;
    cout<<"\nIntrodu data(format data dd/mm/yyyy ):";cin>>aux;
    while(format_valid_data(aux)!=1)
    {   cout<<"Data incorecta sau format gresit.";
        cout<<"\nIntrodu din nou data(format data dd/mm/yyyy ):";cin>>aux;
    }
    c.data_cheltuirii=aux;

return in;
}
istream& operator>>(istream& in, Comenzi& c)
{
    cout<<"Introdu denumirea:";in>>c.denumire_comanda;
    cout<<"\nIntrodu valoarea:";in>>c.valoare_comanda;
    cout<<"\nIntrodu deadline-ul(format data dd/mm/yyyy ):";in>>c.deadline_comanda;

    while(format_valid_data(c.deadline_comanda)!=1)
    {   cout<<"Data incorecta sau format gresit.";
        cout<<"\nIntrodu din nou data(format data dd/mm/yyyy ):";cin>>c.deadline_comanda;
    }

    return in;
}
istream& operator>>(istream& in,Vanzari& v)
{
    cout<<"Introdu denumirea:";in>>v.denumire_vanzare;
    cout<<"\nIntrodu data(format data dd/mm/yyyy ):";in>>v.data_vanzarii;
    cout<<"\nIntrodu valoarea:";in>>v.valoare_vanzare;

    return in;
}
istream& operator>>(istream& in, Produse& p)
{cout<<"\nTe rog introdu toate datele produsului tau:";
    char aux_tip[4],aux_cat[100];



    cout<<"\nNume:"; in>>p.nume_produs;
    cout<<"\nStoc:"; in>>p.stoc;
    cout<<"\nPret:"; in>>p.pret;
    cout<<"\nCantitate filament (cate grame):"; in>>p.cantitate_filament;

    cout<<"\nCategoria:";in>>aux_cat;
    if(p.categorie!=NULL)
        delete[] p.categorie;
    p.categorie=new char[strlen(aux_cat)+1];
    strcpy(p.categorie,aux_cat);

    cout<<"\nTipul filamentului (PLA, TPU, ABS):"; in>>aux_tip;
    strcpy(p.tip_filament,aux_tip);


    if(p.cost_producere!=NULL) delete  p.cost_producere;
    p.cost_producere=new float[1];
    cout<<"\nCost de producere:";in>>p.cost_producere[0];

    if(p.minute_executie!=NULL) delete  p.minute_executie;
    p.minute_executie= new int[1];
    cout<<"\nMinute de executie:"; in>>p.minute_executie[0];

    cout<<"\nNumarul de culori:"; in>>p.numar_culori;

    if(p.culori_filament!=NULL) delete[] p.culori_filament;
    p.culori_filament=new string[p.numar_culori];
    cout<<"\nCulori filament:";
    for(int i=0; i<p.numar_culori; i++)
      in>>p.culori_filament[i];


    return in;}
istream& operator>>(istream& in, Custom& c)
{
    in>>(Comenzi&)c;
    cout<<"Dimensiuni(L,l,h):";
    in>>c.dimensiuni[0]>>c.dimensiuni[1]>>c.dimensiuni[2];
    float comision,com; comision=c.dimensiuni[0]*c.dimensiuni[1]*c.dimensiuni[2];
    cout<<"Care este procentul comisionului tau raportat la volumul produsului? ";in>>com;
    comision=comision*(com/100);
    c.valoare_comanda=c.valoare_comanda+comision;

    return in;
}

istream& operator>>(istream& in, Produs_custom& c)
{
    in>>(Produse&)c;
    cout<<"Dimensiuni(L,l,h):";
    in>>c.dimensiuni[0]>>c.dimensiuni[1]>>c.dimensiuni[2];
    float comision,com; comision=c.dimensiuni[0]*c.dimensiuni[1]*c.dimensiuni[2];
    cout<<"Care este procentul comisionului tau raportat la volumul produsului? ";in>>com;
    comision=comision*(com/100);
    c.pret=c.pret+comision;

    return in;
}
istream& operator>>(istream& in,Filament& f)
{
    cout<<"\nTipul filamentului(PLA,TPU etc): "; in>>f.tip;
    cout<<"\nTemperatura de topire: "; in>>f.temperatura_topire;
    cout<<"\nNumar culori: "; in>>f.nr_culori;
    cout<<"\nCulorile disponibile: ";
    f.culori=new string[f.nr_culori];
    for(int i=0;i<f.nr_culori;i++) in>>f.culori[i];
    return in;
}
istream& operator>>(istream& in,Imprimante& i)
{
cout<<"\nNumele imprimantei: ";in>>i.nume;
cout<<"\nDimensiunea patului - dimensiuni de printare (L,l,h):";in>>i.dimensiuni[0]>>i.dimensiuni[1]>>i.dimensiuni[2];
cout<<"\nNumar de tipuri de filament compatibil: "; in>>i.nr_tipuri;
cout<<"\nTipuri de filament compatibil: ";
i.filament_compatibil=new Filament[i.nr_tipuri];
    for(int j=0;j<i.nr_tipuri;j++) in>>i.filament_compatibil[j];
    return in;}

//citire virtuala
istream& Clienti::CitireVirtuala(istream& in){
    cout<<"\nNume: "; in>>nume;
    cout<<"\nNumar telefon: ";in>>telefon;
    cout<<"\nNumar comenzi date: ";in>>nr_comenzi;
    return in;

}
istream& Persoana_fizica::CitireVirtuala(istream& in){
    cout<<"\nPrenume: ";in>>prenume;
    Clienti::CitireVirtuala(in);
    return in;
}
istream& Firma::CitireVirtuala(istream& in){
    Clienti::CitireVirtuala(in);
    cout<<"\nCod fiscal: ";in>>cod_fiscal;
    return in;

}


istream& operator>>(istream& in, Clienti& c)
{
return c.CitireVirtuala(in);
}

istream& operator>>(istream& in,Persoana_fizica& c)
{
return c.CitireVirtuala(in);

}

istream& operator>>(istream& in,Firma& c)
{
return c.CitireVirtuala(in);

}

// citire fisier

ifstream& operator>>(ifstream& in, Cheltuieli& c)
{  string aux;

    in>>c.denumire_cheltuiala;
    in>>c.valoare_cheltuita;
    in>>aux;
    c.data_cheltuirii=aux;

return in;
}
ifstream& operator>>(ifstream& in, Comenzi& c)
{
in>>c.denumire_comanda;
in>>c.valoare_comanda;
in>>c.deadline_comanda;

    return in;
}
ifstream& operator>>(ifstream& in,Vanzari& v)
{
    in>>v.denumire_vanzare;
    in>>v.data_vanzarii;
    in>>v.valoare_vanzare;

    return in;
}
ifstream& operator>>(ifstream& in, Produse& p)
{
    char aux_tip[4],aux_cat[100];

   in>>p.nume_produs;
   in>>p.stoc;
   in>>p.pret;
   in>>p.cantitate_filament;
   in>>aux_cat;
    if(p.categorie!=NULL)
        delete[] p.categorie;
    p.categorie=new char[strlen(aux_cat)+1];
    strcpy(p.categorie,aux_cat);

    in>>aux_tip;
    strcpy(p.tip_filament,aux_tip);


    if(p.cost_producere!=NULL) delete  p.cost_producere;
    p.cost_producere=new float[1];
    in>>p.cost_producere[0];

    if(p.minute_executie!=NULL) delete  p.minute_executie;
    p.minute_executie= new int[1];
    in>>p.minute_executie[0];

    in>>p.numar_culori;

    if(p.culori_filament!=NULL) delete[] p.culori_filament;
    p.culori_filament=new string[p.numar_culori];

    for(int i=0; i<p.numar_culori; i++)
      in>>p.culori_filament[i];


    return in;}
ifstream& operator>>(ifstream& in, Custom& c)
{
    in>>(Comenzi&)c;

    in>>c.dimensiuni[0]>>c.dimensiuni[1]>>c.dimensiuni[2];
    float comision,com; comision=c.dimensiuni[0]*c.dimensiuni[1]*c.dimensiuni[2];
    in>>com;
    comision=comision*(com/100);
    c.valoare_comanda=c.valoare_comanda+comision;

    return in;
}

ifstream& operator>>(ifstream& in, Produs_custom& c)
{
    in>>(Produse&)c;

    in>>c.dimensiuni[0]>>c.dimensiuni[1]>>c.dimensiuni[2];
    float comision,com; comision=c.dimensiuni[0]*c.dimensiuni[1]*c.dimensiuni[2];
    in>>com;
    comision=comision*(com/100);
    c.pret=c.pret+comision;

    return in;
}
ifstream& operator>>(ifstream& in,Filament& f)
{
    in>>f.tip;
    in>>f.temperatura_topire;
    in>>f.nr_culori;
    f.culori=new string[f.nr_culori];
    for(int i=0;i<f.nr_culori;i++) in>>f.culori[i];
    return in;
}
ifstream& operator>>(ifstream& in,Imprimante& i)
{
in>>i.nume;
in>>i.dimensiuni[0]>>i.dimensiuni[1]>>i.dimensiuni[2];
in>>i.nr_tipuri;
i.filament_compatibil=new Filament[i.nr_tipuri];
    for(int j=0;j<i.nr_tipuri;j++) in>>i.filament_compatibil[j];
    return in;}


//operator ++(post si pre incrementare)

const Cheltuieli& Cheltuieli::operator++()
{
    this->valoare_cheltuita++;
    return *this;
}

const Cheltuieli Cheltuieli::operator++(int)
{

    Cheltuieli aux(*this);
    this->valoare_cheltuita++;

    return aux;

}

const Comenzi& Comenzi::operator++()
{
    this->contor_pozitie++;
    return *this;
}

const Comenzi Comenzi::operator++(int)
{

    Comenzi aux(*this);
    this->valoare_comanda++;

    return aux;

}
const Vanzari& Vanzari::operator++()
{
    this->valoare_vanzare++;
    return *this;
}

const Vanzari Vanzari::operator++(int)
{

    Vanzari aux(*this);
    this->valoare_vanzare++;

    return aux;

}
const Produse& Produse::operator++()
{
    this->pret++;
    return *this;
}

const Produse Produse::operator++(int)
{

    Produse aux(*this);
    this->pret++;

    return aux;

}

//operatori matematici(+,-,* sau /)
Cheltuieli Cheltuieli::operator+(Cheltuieli c) {
        c.valoare_cheltuita=this->valoare_cheltuita+c.valoare_cheltuita;
        return c;
}
Cheltuieli Cheltuieli::operator-(Cheltuieli c) {
        c.valoare_cheltuita=this->valoare_cheltuita-c.valoare_cheltuita;
        return c;
}

Vanzari Vanzari::operator+(Vanzari v) {
        v.valoare_vanzare=this->valoare_vanzare+v.valoare_vanzare;
        return v;
}
Vanzari Vanzari::operator-(Vanzari v) {
        v.valoare_vanzare=this->valoare_vanzare-v.valoare_vanzare;
        return v;
}

Comenzi Comenzi::operator+(Comenzi c) {
        c.valoare_comanda=this->valoare_comanda+c.valoare_comanda;
        return c;
}
Comenzi Comenzi::operator-(Comenzi c) {
        c.valoare_comanda=this->valoare_comanda-c.valoare_comanda;
        return c;
}

Produse operator+(Produse p,float x){

        p.pret=p.pret+x;
        return p;}

Produse operator-(Produse p,float x){

        p.pret=p.pret-x;
        return p;
}
Produse operator*(Produse p,float x){

        p.pret=p.pret*x;
        return p;
}
Produse operator/(Produse p,float x){

        p.pret=p.pret/x;
        return p;
}


Produse operator+(float x, Produse p)
{p.pret=x+p.pret;
        return p;}
Produse operator*(float x, Produse p)
{p.pret=x*p.pret;
        return p;}

//aperatori  conditionali
bool Cheltuieli::operator>(const Cheltuieli& c){

        if(this->valoare_cheltuita>c.valoare_cheltuita)
            return true;
        return false;}
bool Comenzi::operator>=(const Comenzi& c){

        if(this->valoare_comanda>c.valoare_comanda)
            return true;
        return false;}
bool Vanzari::operator<=(const Vanzari& v){

        if(this->valoare_vanzare>v.valoare_vanzare)
            return true;
        return false;}
bool Produse::operator<(const Produse& p){

        if(this->pret<p.pret)
            return true;
        return false;}

//operator ==
bool Cheltuieli::operator==(const Cheltuieli& c)
{
    if(this->data_cheltuirii==c.data_cheltuirii)
            return true;
        return false;
}

bool Comenzi::operator==(const Comenzi& c)
{
    if(this->deadline_comanda==c.deadline_comanda)
            return true;
        return false;
}

bool Vanzari::operator==(const Vanzari& v)
{
    if(this->data_vanzarii==v.data_vanzarii)
            return true;
        return false;
}

bool Produse::operator==(const Produse& p)
{
    if(this->nume_produs==p.nume_produs && this->pret==p.pret )
            return true;
        return false;
}

//op indexare
string Produse::operator[](int index)
{
    if(0>index || index>numar_culori)
            {cout<<"Index gresit"; exit(0); }
    return culori_filament[index];
}


//op cast explicit
Cheltuieli::operator int(){
        return (int)this->valoare_cheltuita;
    }
Comenzi::operator int(){
        return (int)this->valoare_comanda;
    }
Vanzari::operator int(){
        return (int)this->valoare_vanzare;
    }
Produse::operator int(){
        return (int)this->pret;
    }

//functii pentru adaugari
void adaug_cheltuiala() //  adaugare o noua cheltuiala

{   int indice;
        cheltuieli[1].set_contor();
        indice=cheltuieli[1].get_contor();
        cin>>cheltuieli[indice];
        cout<<cheltuieli[indice];
        vct_cheltuieli.push_back(cheltuieli[indice]);



}
void adaug_comanda() // adaugare o noua comanda simpla
{
    int indice;
        comenzi[1]->set_contor();
        indice=comenzi[1]->get_contor();
        comenzi[indice]=new Comenzi();
        cin>>static_cast<Comenzi&>(*(comenzi[indice]));
        cout<<*comenzi[indice];
        vct_comenzi.push_back(comenzi[indice]);

}
void adaug_comanda_custom() // adaugare o noua comanda custom
{
    cout<<"\nPentru a putea realiza un produs custom te rugam sa introduci dupa adaugarea comenzii toate detaliile acestui produs temporar.\n";
    int indice;
        comenzi[1]->set_contor();
        indice=comenzi[1]->get_contor();
        comenzi[indice]=new Custom();
        cin>>static_cast<Custom&>(*(comenzi[indice]));
        cout<<static_cast<Custom&>(*(comenzi[indice]));
        vct_comenzi.push_back(comenzi[indice]);

    //adaugare produs temporar
    int indice2;
    produse_temporare[1].set_contor_custom();
    indice2=produse_temporare[1].get_contor_custom();
    cin>>produse_temporare[indice2];
    cout<<produse_temporare[indice2];
    int nou; nou=comenzi[indice]->get_id_comanda();
    produse_temporare[indice2].set_id(nou);
    lista_produse_temporare.push_back(produse_temporare[indice2]);

}
void adaug_comanda_noua() // adaugare o noua comanda simpla
{ int nr;
   cout<<"\nComanda simpla - 1 sau comanda custom - 2 ? "; cin>>nr;
   switch(nr){
        case 1:{adaug_comanda(); break;}
        case 2:{adaug_comanda_custom(); break;}
        default: {cout<<"Comanda gresita!"; break;}
   }

}
void adaug_vanzare_noua(Comenzi* c) // adaugare o noua vanzare atunci cand o comanda a fost expediata
{   int indice;
    string nume,data;
    float valoare;
    nume=c->get_denumire_comanda();
    data=c->get_deadline_comanda();
    valoare=c->get_valoare_comanda();

    vanzari[1].set_contor();
    indice=vanzari[1].get_contor();

    vanzari[indice].set_denumire_vanzare(nume);
    vanzari[indice].set_data_vanzarii(data);
    vanzari[indice].set_valoare_vanzare(valoare);

    cout<<vanzari[indice];
    vct_vanzari.push_back(vanzari[indice]);

}
void adaug_produs()

{   int indice;
    produse[1].set_contor();
    indice=produse[1].get_contor();
    cin>>produse[indice];
    cout<<produse[indice];
    //vct_produse.push_back(produse[indice]);

}

void adaug_imprimanta()
{
    int indice;
    imprimante[1].set_contor();
    indice=imprimante[1].get_contor();
    cin>>imprimante[indice];
    cout<<imprimante[indice];
    vct_imprimante.push_back(imprimante[indice]);

}

void adaug_client()
{   int indice,nr;
    cout<<"Clientul tau este persoana fizica(1) sau firma(2)? Alege numarul: "; cin>>nr;
    clienti[1]->set_contor();
    indice=clienti[1]->get_contor();
    switch(nr){
        case 1:{ clienti[indice]=new Persoana_fizica;  break;}
        case 2:{ clienti[indice]=new Firma; break;}
        default: {cout<<"Comanda gresita!"; break;}
   }
    cin>>*clienti[indice];
    clienti[indice]->calcul_reducere();
    vct_clienti.push_back(clienti[indice]);


}

//afisarea tuturor obiectelor dintr-o clasa (cu vector)
/*void afisare_cheltuieli()
{
    int i, numar;
    numar=cheltuieli[1].get_contor();
    for (i=1;i<=numar;i++)
        cout<<cheltuieli[i];
}


void afisare_comenzi()
{
    int i, numar;
    numar=comenzi[1]->get_contor();
    for (i=1;i<=numar;i++)
        {if(typeid(*(comenzi[i]))==typeid(Custom))
                 cout<<static_cast<Custom&>(*(comenzi[i]))<<endl;
        else
                cout<< static_cast<Comenzi&>(*(comenzi[i]))<<endl;

        }
}
void afisare_vanzari()
{
    int i, numar;
    numar=vanzari[1].get_contor();
    for (i=1;i<=numar;i++)
        cout<<vanzari[i];
}
void afisare_produse()
{
     int i, numar,ok=0;
    numar=produse[1].get_contor();
    for (i=1;i<=numar;i++)
    {cout<<produse[i]<<endl;ok=1;}
    if(ok==0) cout<<"Nu exista produse";

    }

*/
void afisare_imprimante()
{
    int i, numar;
    numar=imprimante[1].get_contor();
    for (i=1;i<=numar;i++)
        cout<<imprimante[i]<<endl;
}

void afis(Clienti* c)
{
if(typeid(*(c))==typeid(Persoana_fizica))
    {
    c->tip_client();
    cout<<static_cast<Persoana_fizica&>(*(c));
    c->cost_transport();
    }
    else
    {
    c->tip_client();
    cout<<static_cast<Firma&>(*(c));
    c->cost_transport();
    }

    cout<<endl;
}


void afisare_clienti()
{
    int i, numar;
    numar=clienti[1]->get_contor();
    for (i=1;i<=numar;i++)
        afis(clienti[i]);
}


void afisare_cheltuieli()
{
    for(auto i=vct_cheltuieli.begin(); i!=vct_cheltuieli.end(); i++)
        cout<<(*i);

}
void afisare_comenzi()
{
    for(auto i=vct_comenzi.begin(); i!=vct_comenzi.end(); i++)
        cout<<*(*i);

}
void afisare_vanzari()
{
    for(auto i=vct_vanzari.begin(); i!=vct_vanzari.end(); i++)
        cout<<(*i);
}
void afisare_produse()
{ int ok=0;
     for(auto i=vct_produse.begin(); i!=vct_produse.end(); i++)
        {cout<<(*i)<<endl;ok=1;}
    if(ok==0) cout<<"Nu exista produse";

}

/*void afisare_imprimante()
{
    for(auto i=vct_imprimante.begin(); i!=vct_imprimante.end(); i++)
        cout<<(*i);
}


void afisare_clienti()
{ for(auto i=vct_clienti.begin(); i!=vct_clienti.end(); i++)
       {   (*i)->tip_client();
           cout<<*(*i);
       }
}*/



// in functie de statusul comenzilor se adauga vanzari --- neprocesata, executata, trimisa ; daca trimisa -> adaug la vanzare
void schimbare_status_comanda()
{ int id,i, numar,comanda_mea=-1,id_comanda;

  string status;
    cout<<"Introdu id-ul comenzii tale (Pentru aflarea id-ului interoghiaza comenzile):"; cin>>id;
    cout<<"Care este noul status? (neprocesata, executata, trimisa ):"; cin>>status;
    numar=comenzi[1]->get_contor();
    for (i=1;i<=numar;i++)
    { id_comanda=comenzi[i]->get_id_comanda();
        if(id==id_comanda) comanda_mea=i;
    }
    if (comanda_mea==-1) cout<<"ID inexistent !";
    else{
        comenzi[comanda_mea]->set_status_comanda(status);
        vct_comenzi[comanda_mea]=comenzi[comanda_mea];
        if (status=="trimisa") adaug_vanzare_noua(comenzi[comanda_mea]);

        if(typeid(*(comenzi[comanda_mea]))==typeid(Custom)) //sterg produsul temporar
        {
            for (auto j = lista_produse_temporare.begin(); j != lista_produse_temporare.end(); j++)
                { int id_prod, id_com;
                id_prod=(*j).get_id();
                id_com=comenzi[comanda_mea]->get_id_comanda();
                if(id_com==id_prod){lista_produse_temporare.erase(j); cout<<"S-a sters un produs temporar";}
                }



        }


    }
}
//functionalitati
void cheltuieli_pe_luna(int luna)
{
    int i, numar, luna2,ok=0,d1,d2;
    string data;
    numar=cheltuieli[1].get_contor();
    for (i=1;i<=numar;i++)
    {
        data=cheltuieli[i].get_data_cheltuirii();
        if (luna>9){ d1=(int)data[3]-48; d2=(int)data[4]-48; luna2=d1*10+d2;}
        else luna2=(int)data[4]-48;
        if (luna==luna2) {cout<<cheltuieli[i];ok=1;}
    }
    if(ok==0) cout<<"\nNu exista cheltuieli in aceasta luna.";
}

void vanzari_pe_luna(int luna)
{
    int i, numar, luna2,ok=0,d1,d2;
    string data;
    numar=vanzari[1].get_contor();
    for (i=1;i<=numar;i++)
    {
        data=vanzari[i].get_data_vanzarii();
        if (luna>9){ d1=(int)data[3]-48; d2=(int)data[4]-48; luna2=d1*10+d2;}
        else luna2=(int)data[4]-48;
        if (luna==luna2) {cout<<vanzari[i];ok=1;}
    }
    if(ok==0) cout<<"\nNu exista vanzari in aceasta luna.";
}

void comenzi_pe_luna(string data_dl)
{
    int i, numar,ok=0;
    string data;
    numar=comenzi[1]->get_contor();
    for (i=1;i<=numar;i++)
    {
        data=comenzi[i]->get_deadline_comanda();
        if (data_dl==data) {cout<<*comenzi[i];ok=1;}
    }
    if(ok==0) cout<<"\nNu exista comenzi cu acest deadline.";
}

void filtru()
{ bool pret,stoc,ok=1;
float minim=-1,maxim=-1;
int stocul=-1;

    cout<<"\n\tAlege filtrele pe care doresti sa le aplicam produselor tale, marcand fiecare optiune DOAR cu 0 sau 1";
    cout<<"\nPret:"; cin>>pret;
    cout<<"\nStocul produsului:"; cin>>stoc;


    if(pret==1) {cout<<"\n\tPretul minim:"; cin>>minim; cout<<"\n\tPretul maxim:"; cin>>maxim;}
    if(stoc==1) {cout<<"\n\tAlege stocul:"; cin>>stocul;}

    if(pret==1 &&(minim<0 || maxim<=0 || minim>maxim)) {ok=0; cout<<"\nDate pret gresite";}
    if(stoc==1 && stocul<0) {ok=0; cout<<"\nDate stoc gresite";}

    //filtrarea produselor pe baza optiunilor

int i, numar;
    numar=produse[1].get_contor();
if(ok==1)
{ ok=0;


    for (i=1;i<=numar;i++)
    { bool p=1,s=1;
      if(pret==1 && (produse[i].get_pret()<minim || produse[i].get_pret()>maxim)) p=0;
      if(stoc==1 && produse[i].get_stoc()!=stocul) s=0;
      if (s==1 && p==1) {cout<<produse[i]<<endl; ok=1;}
    }
  if(ok==0) cout<<"\n\tNu exista produse potrivite filtrelor dvs.";
}
}


/*void cautare_produs_custom()
{
    int id,nr,id_prod,ok=0;
    cout<<"Introdu Id-ul comenzii custom: "; cin>>id;
    nr=produse_temporare[0].get_contor_custom();

    for(int i=1;i<=nr;i++)
    {
        id_prod=produse_temporare[i].get_id();
        if(id==id_prod){ ok=i;}
    }

if (ok==0) cout<<"\nNu exista produs cu acest id\n";
else cout<<produse_temporare[ok];

}*/

void cautare_produs_custom()
{
    int id,id_prod,ok=0;
    cout<<"Introdu Id-ul comenzii custom: "; cin>>id;

   for (auto i = lista_produse_temporare.begin(); i != lista_produse_temporare.end(); i++)
    {
        id_prod=(*i).get_id();
        if(id==id_prod){ ok=1; cout<<*i;}
    }

if (ok==0) cout<<"\nNu exista produs cu acest id sau comanda a fost trimisa\n";


}

void compatibilitate()
{
  int i,j, numar,id,avem, numar2,ok=0;
  string tipul;

  numar=imprimante[1].get_contor();
  numar2=produse[1].get_contor();

  cout<<"Introdu id-ul produsului: "; cin>>id;
//caut produsul
for (i=1;i<=numar2;i++)
    if(id==produse[i].get_id_produs()){tipul=produse[i].get_tip_filament(); ok=1;}

if(ok==0) cout<<"\nId inexistent";
else
    { ok=0;


    cout<<"\nImprimante compatibile:\n";
        for (i=1;i<=numar;i++)
            { avem=0;
                int nr_fil;
                nr_fil=imprimante[i].get_nr_tipuri();
                for(j=0;j<nr_fil;j++)
                    {if(imprimante[i].get_filament_compatibil(j)==tipul) {avem=1;ok=1;}}

            if(avem==1) cout<<imprimante[i].get_nume()<<endl;
            }
        if(ok==0) cout<<"nu exista nicio imprimanta compatibila";
    }

}

void fidel()
{
    int numar,maxi=0;
    numar=clienti[1]->get_contor();
    for (int i=1;i<=numar;i++)
         afis(clienti[i]);

    for (int i=1;i<=numar;i++)
        if(maxi<clienti[i]->get_nr_comenzi()) maxi=clienti[i]->get_nr_comenzi();

    cout<<"\n\tAcesti clienti au plasat cele mai multe comenzi\n";
    for (int i=1;i<=numar;i++)
        if(maxi==clienti[i]->get_nr_comenzi()) afis(clienti[i]);
}

void schimare_nr_comenzi()
{ string tel, telefon_client;
    cout<<"Cauta clientul in baza noastra dupa numarul de telefon: "; cin>>tel;
int i, numar,ok=0,nou;
numar=clienti[1]->get_contor();
    for (i=1;i<=numar;i++)
    { telefon_client=clienti[i]->get_telefon();
      if(tel==telefon_client)
        {ok=1;
        cout<<"S-a gasit clientul, cate comenzi are? ";
        cin>>nou; clienti[i]->set_nr_comenzi(nou);
        clienti[i]->calcul_reducere();
        cout<<*clienti[i];
        vct_clienti[i]=clienti[i];
        }
    }
    if(ok==0) cout<<"Client inexistent;";
}

void adaugare_date()
{
    //cheltuieli - o intrare
     int indice;
    cheltuieli[1].set_contor();
    indice=cheltuieli[1].get_contor();
    f>>cheltuieli[indice];
    vct_cheltuieli.push_back(cheltuieli[indice]);
    //comanda - o intrare
    comenzi[1]->set_contor();
    indice=comenzi[1]->get_contor();
    comenzi[indice]=new Comenzi();
    f>>static_cast<Comenzi&>(*(comenzi[indice]));
    vct_comenzi.push_back(comenzi[indice]);
    //produs - 2 intrari
    produse[1].set_contor();
    indice=produse[1].get_contor();
    f>>produse[indice];
    vct_produse.push_back(produse[indice]);
    produse[1].set_contor();
    indice=produse[1].get_contor();
    f>>produse[indice];
    vct_produse.push_back(produse[indice]);
    //imprimante - 2 intrari

    imprimante[0].set_contor();
    indice=imprimante[0].get_contor();
    f>>imprimante[indice];
    //vct_imprimante.push_back(imprimante[indice]);
    imprimante[1].set_contor();
    indice=imprimante[1].get_contor();
    f>>imprimante[indice];
    vct_imprimante.push_back(imprimante[indice]);



   f.close();
}
//pt set
void filamentul_meu_citire_fisier()
{
    int nr;
    string x;
    f>>nr;
    while(nr>0)
    {   f>>x; nr--;
        filament.insert(x);
    }
}

void filamentul_meu() //afisare
{ cout<<"Tipurile de filamente sunt urmatoarele:\n";
  for (auto i = filament.begin(); i != filament.end(); i++) {
        cout <<*i<<" ";
    }
    cout<<endl;
}

//pt map

void reducere() //adaugare
{ int id,ok=0;
  float pret;
    cout<<"Alege produsul pe care il reduci (dupa id): "; cin>>id;
    int numar=produse[1].get_contor();
    for (int i=1;i<=numar;i++)
    if(produse[i].get_id_produs()==id) {ok=1;}
    cout<<"Alege noul pret: "; cin>>pret;

    if(ok==0) cout<<"id inexistent";
    else {reduceri.insert(pair<int, float>(id,pret));cout<<"Am adaugat. Il poti cauta la interogari produse.\n";}

}

void afis_reduceri()
{
    for (auto i = reduceri.begin(); i!= reduceri.end(); ++i)
        {
        cout<<produse[i->first].get_nume_produs()<<", "<< i->second<< endl;}
}

int main()
{

adaugare_date();
filamentul_meu_citire_fisier();



    int categorie_actiune=1, actiune=0,cod_actiune=0,sub_actiune=0, pastrez=0;
   cout<<"Vrei sa pastrezi toate produsele si imprimantele adaugate intr-un fisier ? 1-da, 0-nu -> "; cin>>pastrez;

    while (categorie_actiune!=0)//meniu interactiv
    {
        cout<<"\n\tTe rugam alege categoria actiunii pe care doresti sa o faci."<<endl<<"1-ADAUGARE"<<endl<<"2-INTEROGARE"<<endl<<"3-MODIFICARE"<<endl<<"0-ANULARE SELECTIE";
        cout<<endl<<"Aleg numarul:";
        cin>>categorie_actiune; //stabilirea categoriei actiunii pe care vreau sa o fac

        switch (categorie_actiune) //stabilirea actiunii pe care vreau sa o fac
        {
        case 0:
            {cout<<endl<<"\tO ZI BUNA!"; break;}
        case 1:
            {
                cout<<endl<<"\tAi ales ADAUGARE ! Ce vrei sa adaugi?"<<endl<<"1-PRODUS NOU"<<endl<<"2-CHELTUIALA NOUA"<<endl<<"3-COMANDA NOUA"<<endl<<"4-IMPRIMANTA NOUA"<<endl<<"5-CLIENT NOU"<<endl;
                cout<<endl<<"Aleg numarul:";
                cin>>actiune; sub_actiune=0;
                break;
            }
        case 2:
            {
                cout<<endl<<"\tAi ales INTEROGARE ! Ce vrei sa interoghezi?"<<endl<<"1-PRODUSE"<<endl<<"2-VANZARI"<<endl<<"3-CHELTUIELI"<<endl<<"4-COMENZI"<<endl;
                cout<<"5-IMPRIMANTELE MELE"<<endl<<"6-CLIENTI"<<endl;
                cout<<endl<<"Aleg numarul:";
                cin>>actiune;

                switch(actiune)
                {case 1:{ cout<<"\nAi ales PRODUSE. Alege una dintre optiuni\n"<<"1-AFISAREA TUTUROR PRODUSELOR STANDARD"<<endl<<"2-PRODUSE (standard) CU FILTRE APLICATE"<<endl<<"3-UN ANUME PRODUS CUSTOM"<<endl<<"4-PRODUSELE PROMOTIONALE TEMPORAR"<<endl<<"Aleg numarul:";
                cin>>sub_actiune; break;}
                case 2:{cout<<"\nAi ales VANZARI. Alege una dintre optiuni\n"<<"1-AFISAREA TUTUROR VANZARILOR"<<endl<<"2-VANZARI DINTR-O LUNA"<<endl<<"Aleg numarul:";
                cin>>sub_actiune;  break;}
                case 3:{cout<<"\nAi ales CHELTUIELI. Alege una dintre optiuni\n"<<"1-AFISAREA TUTUROR CHELTUIELILOR"<<endl<<"2-CHELTUIELI DINTR-O LUNA"<<endl<<"Aleg numarul:";
                 cin>>sub_actiune; break;}
                case 4:{cout<<"\nAi ales COMENZI. Alege una dintre optiuni\n"<<"1-AFISAREA TUTUROR COMENZILOR"<<endl<<"2-COMENZI CU UN ANUME DEADLINE"<<endl<<"Aleg numarul:";
                 cin>>sub_actiune; break;}
                case 5:{cout<<"\nAi ales IMPRIMANTELE MELE. Alege una dintre optiuni\n"<<"1-AFISAREA TUTUROR IMPRIMANTELOR"<<endl<<"2-VREAU SA AFLU CE IMPRIMANTE SUNT COMPATIBILE CU UN PRODUS STANDARD"<<endl<<"3-CE TIPURI DE FILAMENTE AM"<<endl;
                  cout<<"Aleg numarul:";
                  cin>>sub_actiune;break;}
                 case 6:{cout<<"\nAi ales CLIENTI. Alege una dintre optiuni\n"<<"1-AFISAREA TUTUROR CLIENTILOR"<<endl<<"2-CLIENTII CU CELE MAI MULTE COMENZI PLASATE"<<endl<<"Aleg numarul:";
                 cin>>sub_actiune; break;}
                 default:{cout<<endl<<"CATEGORIE 1 INEXISTENTA!"<<endl; break;}
                }



                break;
            }
        case 3:
            {
                cout<<endl<<"\tAi ales MODIFICARE! Ce vrei sa modifici?"<<endl<<"1-STATUS COMANDA EXISTENTA"<<endl<<"2-NUMARUL COMENZILOR UNUI CLIENT"<<endl<<"3-REDUCERE TEMPORARA LA UN PRODUS"<<endl<<"Aleg numarul:";
                cin>>actiune; sub_actiune=0;
                break;
            }
        default:{cout<<endl<<"CATEGORIE 2 INEXISTENTA!"<<endl; break;}
        }

      cod_actiune=100*categorie_actiune+10*actiune+sub_actiune; cout<<cod_actiune;
 if(cod_actiune>100)
     {switch (cod_actiune) //execut actiunea selectata
        {
            case 110: {adaug_produs();  if(pastrez==1){ int indice=produse[1].get_contor();g<<produse[indice]<<endl;}  break;}
            case 120: {adaug_cheltuiala(); break;}
            case 130: {adaug_comanda_noua(); break;}
            case 140: {adaug_imprimanta(); if(pastrez==1){ int indice=imprimante[1].get_contor();g<<imprimante[indice]<<endl;} break;}
            case 150: {adaug_client(); break;}
            case 211: {afisare_produse(); break;}
            case 212: {filtru();break;  }
            case 213: {cautare_produs_custom();break;}
            case 214: {afis_reduceri();break;}
            case 221: {afisare_vanzari(); break;}
            case 222:
                {cout<<"\n\tIntrodu luna din care vrei sa vezi vanzarile (exemplu:2):";
                 int luna; cin>>luna;
                 if(luna>=1 && luna<=12)vanzari_pe_luna(luna);
                 else cout<<"Luna inexistenta";
                 break;}
            case 231: {afisare_cheltuieli(); break;}
            case 232:
                 { cout<<"\n\tIntrodu numarul lunii din care vrei sa vezi cheltuielile (exemplu pentru martie: 3):";
                   int luna; cin>>luna;
                   if(luna>=1 && luna<=12)cheltuieli_pe_luna(luna);
                   else cout<<"Luna inexistenta";
                   break;}

            case 241: {afisare_comenzi(); break;}
            case 242:
                {cout<<"\n\tIntrodu data deadline (formatul acceptat este ZZ/MM/YYYY; exemplu:02/05/2021):";
                 string data; cin>>data;
                 if(format_valid_data(data)) comenzi_pe_luna(data);
                 else cout<<"Data incorecta";
                 break;}
             case 251: {afisare_imprimante(); break;}
             case 252: {compatibilitate(); break;}
             case 253: {filamentul_meu(); break;}
             case 261: {afisare_clienti(); break;}
             case 262: {fidel(); break;}
             case 310: {schimbare_status_comanda(); break;}
             case 320: {schimare_nr_comenzi(); break;}
             case 330: {reducere(); break;}
            case 0:{break;}
            default:{cout<<endl<<"COMANDA INEXISTENTA!"<<endl; break;}
        }
    }
    }

/*
// teste pt clasa Cheltuieli (analog pt celelalte clase)
cout<<"\n\tTestez ceea ce nu este inclus in meniul interactiv.\n\n";

Cheltuieli ob("nou","22/12/2020",15); ++ob; cout<<ob; ob++;  cout<<ob<<endl; // test pt op ++ (post si pre incrementare)
ob.set_denumire_cheltuiala("banane"); //test setteri
cout<<ob.get_denumire_cheltuiala()<<endl; //test getteri
Cheltuieli ob2; cout<<endl<<ob2 ;ob2=ob; cout<<ob2<<endl; // test pt op =

Cheltuieli ob3("banane","22/12/2020");
if (ob3==ob2)cout<<"acc data   ";
else cout<<"alta data   ";    //test pt op ==, care verifica daca 2 celtuieli au fost facute in acc data

Cheltuieli ob4("banane","23/11/2020");
if (ob3==ob4)cout<<"acc data  ";
else cout<<"alta data  ";

Produse o1,o2;
o2.set_pret(55);
if (o1==o2)cout<<"acc produs ";
else cout<<"alt produs  ";  //test op==, verifica daca cele 2 obiecte reprezinta acc produs

//adun 2 obiecte - cheltuieli
cout<<"\n\nOb= "<<ob;
cout<<"Ob2= "<<ob2;
ob=ob+ob2; //adaug ob2 la ob
cout<<"Ob+Ob2= "<<ob;
ob3=ob+ob2; cout<<ob3;//pun in ob3 noul ob adunat cu ob2
ob3=ob-ob2; cout<<ob3;//fac scaderea dintre ob si ob2 si le pun in ob3


//modific pretul unui produs prin scadere, adunare, inmultire si impartire
cout<<"\npret initial= "<<o2.get_pret();
o2=o2+3;
cout<<"\npret crescut= "<<o2.get_pret();
o2=8+o2;
cout<<"\npret crescut iar= "<<o2.get_pret();
o2=o2-4;
cout<<"\npret scazut= "<<o2.get_pret();
o2=o2*2.2;
cout<<"\npret inmultit= "<<o2.get_pret();
o2=3*o2;
cout<<"\npret inmultit iar= "<<o2.get_pret();
o2=o2/2;
cout<<"\npret impartit= "<<o2.get_pret();
if (o1<o2) cout<<"\n o1 este mai ieftin decat o2";
else cout<<"\n o1 este mai scump decat o2";
if (ob>ob2) cout<<"\n ob1 valoreaza mai mult decat ob2  "<<ob.get_valoare_cheltuita()<<","<<ob2.get_valoare_cheltuita();
else cout<<"\n ob valoreaza mai putin decat ob2  "<<ob.get_valoare_cheltuita()<<","<<ob2.get_valoare_cheltuita();

//test indici, id
Comenzi c;
cout<<endl<<comenzi[1];
cout<<c;
cout<<comenzi[1].get_contor()<<" "; //cate comenzi am adaugat in vector
cout<<comenzi[1].get_contor_total();//contor pt indici (vectorul de 1000 de comenzi + comenzi in afara vectorului)


//op cast explicit
ob3.set_valoare_cheltuita(55.2);
cout<<endl<<ob3.get_valoare_cheltuita()<<endl;
cout<<(int)ob3;

Produse p(5,15.3,"banana");
cout<<endl<<p.get_pret()<<endl;
cout<<(int)p;


Produse test1,copie;
cin>>test1;
copie=test1;
cout<<"\n\nAfisez copia        ->                "<<copie;

//test op indexare
cout<<"\n\nAfisez copia  de 2      ->          "<<copie[2];

string prod[]= {"Mere", "Pere", "Banane"};
test1.set_culori_filament(prod,3);
cout<<"\n\nAfisez test1        ->                "<<test1;

cout<<"\n\n\t"<<test1.get_nr_culori();
for (int i=0;i<test1.get_nr_culori();i++)
cout<<"\n\n\t"<<*(test1.get_culori_filament()+i)<<" ";


float d[3]={1,2.2,5};
Custom nou("mama","12/12/2020",15.2,d);
Custom copie_nou,comanda_citita;
copie_nou=nou;
cout<<nou<<copie_nou;
cin>>comanda_citita;
cout<<comanda_citita;

Produs_custom customi;
cin>>customi;
cout<<customi;

Filament f;
f.set_temperatura(150);
f.set_tip("tpu");
cout<<f.get_temperatura()<<f.get_tip();

Filament f2;
cin>>f2;
cout<<f2;

Imprimante test_i;
cin>>test_i;
cout<<test_i;
cout<<test_i.get_filament_compatibil(1);

Firma firm; cin>>firm; cout<<firm;

Clienti* client_nou=new Firma;
cin>>*client_nou;
cout<<*client_nou;
*/
return 0;
}
