#include <iostream>
#include <stdexcept>

template<typename Tip>
class Lista
{
public:
    Lista() {}
    virtual ~Lista() {};
    virtual int brojElemenata() const = 0;
    virtual Tip trenutni() const = 0;
    virtual Tip &trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip operator [](int indeks) const = 0;
    virtual Tip &operator [](int indeks) = 0;
};

template<typename Tip>
class NizLista: public Lista<Tip>
{
    Tip **Niz=nullptr;
    int Velicina=0;
    int IndeksTrenutnog=0;
    int MaxSize=20000;
public:
    NizLista()
    {
        Velicina=0;
        IndeksTrenutnog=0;
        MaxSize=10000;
        Niz=new Tip*[MaxSize];
    }
    void DajMemorije()
    {
        MaxSize=MaxSize+10000;
        Tip** Novi(new Tip*[MaxSize]);
        int NovaVelicina=Velicina;
        int NoviIndeksTrenutnog=IndeksTrenutnog;
        for(int i=0;i<Velicina;i++)
        {
            Novi[i]=new Tip(*Niz[i]);
        }
        while(Velicina>0)
        {
            obrisi();
        }
        delete[] Niz;
        Niz=Novi;
        Velicina=NovaVelicina;
        IndeksTrenutnog=NoviIndeksTrenutnog;
        for(int i=0;i<Velicina;i++)
        {
            Niz[i]=Novi[i];
        }
    }
    ~NizLista()
    {
        for(int i=0; i<Velicina; i++)
        {
            delete Niz[i];
        }
        delete [] Niz;
        Velicina=0;
    }
    NizLista(const NizLista<Tip> &l)
    {
        try
        {
            this->Niz=new Tip*[l.Velicina];
            for(int i=0; i<l.Velicina; i++)
            {
                Niz[i]= new Tip(*l.Niz[i]);
            }
        }
        catch (...)
        {
            for(int i=0; i<l.Velicina; i++)
            {
                delete Niz[i];
            }
            delete [] Niz;
            throw;
        }
        this->Velicina=l.Velicina;
        this->IndeksTrenutnog=l.IndeksTrenutnog;
    }
    NizLista(NizLista<Tip> &&l)
    {
        this->Niz=l.Niz; this->Velicina=l.Velicina; this->IndeksTrenutnog=l.IndeksTrenutnog;
        l.Niz=nullptr; l.IndeksTrenutnog=0; l.Velicina=0;
    }
    NizLista<Tip> &operator =(const NizLista<Tip> &l)
    {
        for(int i=0; i<Velicina; i++)
        {
            delete Niz[i];
        }
        delete [] Niz;
        try
        {
            this->Niz= new Tip*[l.Velicina];
            for(int i=0; i<l.Velicina; i++)
            {
                Niz[i]= new Tip(*l.Niz[i]);
            }
        }
        catch (...)
        {
            for(int i=0; i<l.Velicina; i++)
            {
                delete Niz[i];
            }
            delete [] Niz; throw;
        }
        this->Velicina=l.Velicina;
        this->IndeksTrenutnog=l.IndeksTrenutnog;
        return *this;
    }
    NizLista<Tip> &operator =(NizLista<Tip> &&l)
    {
        std::swap(Velicina,l.Velicina);
        std::swap(Niz,l.Niz);
        std::swap(IndeksTrenutnog,l.IndeksTrenutnog);
    }
    int brojElemenata() const
    {
        return Velicina;
    }
    Tip trenutni() const
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        return *Niz[IndeksTrenutnog];
    }
    Tip &trenutni()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        return *Niz[IndeksTrenutnog];
    }
    bool prethodni()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        if(IndeksTrenutnog==0)
        {
            return false;
        }
        IndeksTrenutnog=IndeksTrenutnog-1;
        return true;
    }
    bool sljedeci()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        if(IndeksTrenutnog==Velicina-1)
        {
            return false;
        }
        if(Velicina==1)
        {
            return true;
        }
        IndeksTrenutnog=IndeksTrenutnog+1;
        return true;
    }
    void pocetak()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        } 
       IndeksTrenutnog=0;
    }
    void kraj()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        IndeksTrenutnog=Velicina-1;
    }
    void obrisi()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        delete Niz[IndeksTrenutnog];
        for(int i=IndeksTrenutnog;i<Velicina;i++)
        {
            Niz[i]=Niz[i+1];
        }
        Velicina--;
        if(IndeksTrenutnog==Velicina)
        {
            IndeksTrenutnog--;
        }
    }
    void dodajIspred(const Tip& el)
    {
        if(Velicina>=MaxSize-1)
        {
            DajMemorije();
        }
        if(Velicina==0)
        {
            Niz[0]=new Tip(el);
            IndeksTrenutnog=0;
            Velicina++;
        }
        else
        {
            Velicina++;
            for(int i=Velicina; i>IndeksTrenutnog; i--)
            {
                Niz[i]=Niz[i-1];
            }
            Niz[IndeksTrenutnog++]=new Tip(el);
        }
    }
    void dodajIza(const Tip& el)
    {
        if(Velicina>=MaxSize-1)
        {
            DajMemorije();
        }
        if(Velicina==0)
        {
            Niz[0]=new Tip(el);
            IndeksTrenutnog=0;
            Velicina++;
        }
        else if (IndeksTrenutnog==Velicina-1)
        {
            Velicina++;
            Niz[IndeksTrenutnog+1]=new Tip(el);
        }
        else
        {
            Velicina++;
            for(int i=Velicina-1; i>IndeksTrenutnog; i--)
            {
                Niz[i]=Niz[i-1];
            }
            Niz[IndeksTrenutnog+1]=new Tip(el);
        }
    }
    Tip operator [](int indeks) const
    {
        if(indeks<0 || indeks>=Velicina)
        {
            throw std::range_error("Greska");
        }
        return *Niz[indeks];
    }
        Tip &operator [](int indeks)
        {
        if(indeks<0 || indeks>=Velicina)
        { 
            std::range_error("Greska");
        }
        return *Niz[indeks];
    }
};


// Test funkcije

template<typename Tip>
void TestNizListaDodajIspred(NizLista<Tip> &TestLista)
{
    TestLista.dodajIspred(2);
    TestLista.dodajIspred(4);
    TestLista.dodajIspred(6);
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestNizListaDodajIza(NizLista<Tip> &TestLista)
{
    TestLista.dodajIza(1);
    TestLista.dodajIza(3);
    TestLista.dodajIza(5);
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestNizListaObrisi(NizLista<Tip> &TestLista)
{
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
    TestLista.obrisi();
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestNizListaBrojElemenata(NizLista<Tip> &TestLista)
{
    std::cout << TestLista.brojElemenata() << " ";
    TestLista.dodajIspred(10);
    std::cout << TestLista.brojElemenata() << "\n";
}

template<typename Tip>
void TestNizListaPocetak(NizLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.pocetak();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestNizListaKraj(NizLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.kraj();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestNizListaTrenutni(NizLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestNizListaOperator(NizLista<Tip> &TestLista)
{
    for(int i=0;i<TestLista.brojElemenata(); i++)
    {
        std::cout << TestLista[i] << " ";
    }
    std::cout << "\n";
}

template<typename Tip>
void TestNizListaPrethodni(NizLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.prethodni();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestNizListaSljedeci(NizLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.sljedeci();
    std::cout << TestLista.trenutni() << "\n";
}

// JednostrukaLista

template<typename Tip>
struct Cvor
{
    Tip element;
    Cvor<Tip> *veza;
    Cvor(Tip element, Cvor<Tip>* pok)
    {
        this->element=element;
        this->veza=pok;
    }
};

template<typename Tip>
class JednostrukaLista: public Lista<Tip>
{
    int Velicina=0;
    Cvor<Tip>* Trenutni=nullptr;
    Cvor<Tip>* pocetakListe=nullptr;
    Cvor<Tip>* krajListe=nullptr;
public:
    JednostrukaLista() {}
    JednostrukaLista(const JednostrukaLista<Tip> &l)
    {
        this->Velicina=l.Velicina;
        Cvor<Tip>* priv=nullptr;
        Cvor<Tip>* pok=l.pocetakListe;
        Cvor<Tip>* iza=nullptr;
        try
        {
            for(int i=0; i<Velicina; i++)
            {
                priv=new Cvor<Tip>(pok->element,nullptr);
                if(i==0)
                {
                    this->pocetakListe=priv; iza=priv;
                }
                else
                {
                    iza->veza=priv; iza=iza->veza;
                }
                if(l.Trenutni==pok)
                {
                    this->Trenutni=priv;
                }
                if(i!=Velicina-1)
                {
                    priv=priv->veza;
                }
                pok=pok->veza;
            }
            this->krajListe=priv;
        }
        catch(...)
        { 
            Cvor<Tip>* pok=pocetakListe;
            Cvor<Tip>* iza;
            while(pok!=nullptr)
            {
                iza=pok->veza;
                delete pok;
                pok=iza;
            }
            Velicina=0;
            throw;
        }
    }
    JednostrukaLista(JednostrukaLista<Tip> &&l)
    {
        this->pocetakListe=l.pocetakListe; this->krajListe=l.krajListe; this->Velicina=l.Velicina; this->Trenutni=l.Trenutni;
        l.pocetakListe=nullptr; l.krajListe=nullptr; l.Velicina=0; l.Trenutni=nullptr;
    }
    JednostrukaLista<Tip> &operator =(const JednostrukaLista<Tip> &l)
    {
        Cvor<Tip>* pok=pocetakListe;
        Cvor<Tip>* iza;
        while(pok!=nullptr)
        {
            iza=pok->veza;
            delete pok;
            pok=iza;
        }
        this->pocetakListe=nullptr; this->krajListe=nullptr; this->Trenutni=nullptr; 
        this->Velicina=l.Velicina;
        Cvor<Tip>* priv=nullptr;
        pok=l.pocetakListe;
        iza=nullptr;
        try
        {
            for(int i=0; i<Velicina; i++)
            {
                priv= new Cvor<Tip>(pok->element,nullptr);
                if(i==0)
                {
                    this->pocetakListe=priv; iza=priv;
                }
                else
                {
                    iza->veza=priv; iza=iza->veza;
                }
                if(l.Trenutni==pok)
                {
                    this->Trenutni=priv;
                }
                if(i!=Velicina-1)
                {
                    priv=priv->veza;
                }
                pok=pok->veza;
            }
            this->krajListe=priv;
        }
        catch (...)
        {
            Cvor<Tip>* pok=pocetakListe;
            Cvor<Tip>* iza;
            while(pok!=nullptr)
            {
                iza=pok->veza;
                delete pok;
                pok=iza;
            }
            Velicina=0;
            throw;
        }
        return *this;
    }
    JednostrukaLista<Tip> &operator =(JednostrukaLista<Tip> &&l)
    {
        std::swap(Velicina,l.Velicina);
        std::swap(pocetakListe,l.lista_poc);
        std::swap(krajListe,l.lista_kraj);
        std::swap(Trenutni,l.sadasnji);
    }
    ~JednostrukaLista()
    {
        Cvor<Tip>* pok=pocetakListe;
        Cvor<Tip>* iza;
        while(pok!=nullptr)
        {
            iza=pok->veza;
            delete pok;
            pok=iza;
        }
        Velicina=0;
    }
    int brojElemenata() const
    {
        return Velicina;
    }
    Tip trenutni() const
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        return Trenutni->element;
    }
    Tip &trenutni()
    {
        if(Velicina==0)
        {
            throw std::range_error("Geska");
        }
        return Trenutni->element;
    }
    bool prethodni()
    {
        if(Velicina==0 || Trenutni==pocetakListe)
        {
            return false;
        }
        else
        {
            Cvor<Tip>* iza=pocetakListe;
            Cvor<Tip>* pok=pocetakListe->veza;
            while(pok!=Trenutni)
            {
                pok=pok->veza;
                iza=iza->veza;
            }
            Trenutni=iza;
        }
        return true;
    }   
    bool sljedeci()
    {
        if(Velicina==0 || Trenutni==krajListe)
        {
            return false;
        }
        else
        {
            Trenutni=Trenutni->veza;
        }
        return true;
    }
    void pocetak()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        else
        {
            Trenutni=pocetakListe;
        }
    }
    void kraj()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        else
        {
            Trenutni=krajListe;
        }
    }
    void dodajIspred(const Tip& el)
    {
        if(Trenutni!=pocetakListe)
        {
            auto iza=pocetakListe;
            auto pok=pocetakListe->veza;
            while(pok!=Trenutni && pok!=nullptr)
            {
                pok=pok->veza;
                iza=iza->veza;
            }
            iza->veza=new Cvor<Tip>(el,nullptr);
            iza=iza->veza;
            iza->veza=pok;
        }
        else
        {
            pocetakListe=new Cvor<Tip>(el,nullptr);
            pocetakListe->veza=Trenutni;
            if(krajListe==nullptr)
            {
                krajListe=pocetakListe;
                Trenutni=pocetakListe;
            }
        }
        Velicina++;
    }
    void dodajIza(const Tip& el)
    {
        if(Trenutni!=krajListe)
        {
            auto pok=Trenutni->veza;
            auto iza=Trenutni;
            iza->veza=new Cvor<Tip>(el,nullptr);
            iza=iza->veza;
            iza->veza=pok;
        }
        else
        {
            krajListe=new Cvor<Tip>(el,nullptr);
            krajListe->veza=nullptr;
            if(pocetakListe==nullptr)
            {
                pocetakListe=krajListe;
                Trenutni=pocetakListe;
            }
            else
            {
                Trenutni->veza=krajListe;
            }
        }
        Velicina++;
    }
    void obrisi()
    {
        if(Velicina==0)
        {
            throw std::range_error("Greska");
        }
        if(Trenutni!=pocetakListe)
        {
            Cvor<Tip>* pok=pocetakListe;
            while(pok->veza!=Trenutni)
            {
                pok=pok->veza;
            }
            pok->veza=Trenutni->veza;
            delete Trenutni;
            if(pok->veza==nullptr)
            {
                Trenutni=pok;
                krajListe=pok;
            }
            else
            {
                Trenutni=pok->veza;
            }
        }
        else
        {
            Cvor<Tip>* pok=Trenutni;
            Trenutni=Trenutni->veza;
            delete pok;
            if(Trenutni!=nullptr)
            {
                pocetakListe=Trenutni;
            }
            else if(Trenutni==nullptr)
            {
                pocetakListe=nullptr;
                krajListe=nullptr;
            }
        }
        Velicina--;
    }
    Tip operator[](int indeks) const
    {
        if(indeks<0 || indeks>=Velicina)
        {
            throw std::range_error("Greska");
        }
        Cvor<Tip>* pok=pocetakListe;
        for(int i=0;i<indeks;i++)
        {
            pok=pok->veza;
        }
        return pok->element;
    }
    Tip &operator[](int indeks)
    {
        if(indeks<0 || indeks>=Velicina)
        {
            throw std::range_error("Greska");
        }
        Cvor<Tip>* pok=pocetakListe;
        for(int i=0; i<indeks; i++)
        {
            pok=pok->veza;
        }
        return pok->element;
    }
};


// Test Funkcije

template<typename Tip>
void TestJednostrukaListaDodajIspred(JednostrukaLista<Tip> &TestLista)
{
    TestLista.dodajIspred('a');
    TestLista.dodajIspred('b');
    TestLista.dodajIspred('c');
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestJednostrukaListaDodajIza(JednostrukaLista<Tip> &TestLista)
{
    TestLista.dodajIza('d');
    TestLista.dodajIza('e');
    TestLista.dodajIza('f');
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestJednostrukaListaObrisi(JednostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
    TestLista.obrisi();
    std::cout << TestLista.brojElemenata() << " " << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestJednostrukaListaBrojElemenata(JednostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.brojElemenata() << " ";
    TestLista.dodajIspred(10);
    std::cout << TestLista.brojElemenata() << "\n";
}

template<typename Tip>
void TestJednostrukaListaPocetak(JednostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.pocetak();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestJednostrukaListaKraj(JednostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.kraj();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestJednostrukaListaTrenutni(JednostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestJednostrukaListaOperator(JednostrukaLista<Tip> &TestLista)
{
    for(int i=0;i<TestLista.brojElemenata(); i++)
    {
        std::cout << TestLista[i] << " ";
    }
    std::cout << "\n";
}

template<typename Tip>
void TestJednostrukaListaPrethodni(JednostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.prethodni();
    std::cout << TestLista.trenutni() << "\n";
}

template<typename Tip>
void TestJednostrukaListaSljedeci(JednostrukaLista<Tip> &TestLista)
{
    std::cout << TestLista.trenutni() << " ";
    TestLista.sljedeci();
    std::cout << TestLista.trenutni() << "\n";
}
int main ()
{
    NizLista<double> TestLista;
    TestNizListaDodajIspred(TestLista);
    TestNizListaDodajIza(TestLista);
    TestNizListaObrisi(TestLista);
    TestNizListaBrojElemenata(TestLista);
    TestNizListaPocetak(TestLista);
    TestNizListaKraj(TestLista);
    TestNizListaTrenutni(TestLista);
    TestNizListaOperator(TestLista);
    TestNizListaPrethodni(TestLista);
    TestNizListaSljedeci(TestLista);
    std::cout << "\n\n";
    JednostrukaLista<double> TestLista2;
    TestJednostrukaListaDodajIspred(TestLista2);
    TestJednostrukaListaDodajIza(TestLista2);
    TestJednostrukaListaObrisi(TestLista2);
    TestJednostrukaListaBrojElemenata(TestLista2);
    TestJednostrukaListaPocetak(TestLista2);
    TestJednostrukaListaKraj(TestLista2);
    TestJednostrukaListaTrenutni(TestLista2);
    TestJednostrukaListaOperator(TestLista2);
    TestJednostrukaListaPrethodni(TestLista2);
    TestJednostrukaListaSljedeci(TestLista2);
    return 0;
}