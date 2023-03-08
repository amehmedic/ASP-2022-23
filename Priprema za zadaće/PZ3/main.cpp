#include <iostream>
#include <exception>

using namespace std;
template<typename Tip>
struct Cvor
{
    Tip clan;
    Cvor<Tip> *pok;
    Cvor(Tip Clan)
    {
        clan=Clan;
        pok=nullptr;
    }
};

template<typename Tip>
class Stek
{
    int VelicinaSteka;
    Cvor<Tip>* PocetakSteka;
public:
    Stek()
    {
        VelicinaSteka=0;
        PocetakSteka=nullptr;
    }
    ~Stek()
    {
        this->brisi();
    }

    Stek(const Stek<Tip> &stek)
    {
        VelicinaSteka=stek.VelicinaSteka;
        Cvor<Tip>* pamti=nullptr;
        Cvor<Tip>* novi=nullptr;
        auto pomocni=stek.PocetakSteka;
        for(int i=0;i<VelicinaSteka;i++)
        {
            pamti=new Cvor<Tip>((*pomocni).clan);
            if(i==0)
            {
                PocetakSteka=pamti;
                novi=pamti;
            }
            else 
            {
                (*novi).pok=pamti;
                novi=(*novi).pok;
            }
            if(i!=VelicinaSteka-1)
            {
                pamti=(*pamti).pok;
            }
            pomocni=(*pomocni).pok;
        }
    }
    Stek(Stek<Tip> &&stek)
    {
        PocetakSteka=stek.PocetakSteka;
        VelicinaSteka=stek.VelicinaSteka;
    }
    Stek<Tip> &operator =(const Stek<Tip> &stek)
    {
        if((*this).PocetakSteka==stek.PocetakSteka && VelicinaSteka==stek.VelicinaSteka)
        {
            return *this;
        }
        auto k=PocetakSteka;
        Cvor<Tip>* l;
        while(k!=nullptr)
        {
            l=(*k).pok;
            delete k;
            k=l;
        }
        VelicinaSteka=stek.VelicinaSteka;
        PocetakSteka=nullptr;
        Cvor<Tip>* pamti=nullptr;
        k=stek.PocetakSteka;
        l=nullptr;
        for(int i=0;i<VelicinaSteka;i++)
        {
            pamti=new Cvor<Tip>((*k).clan);
            if(i==0)
            {
                PocetakSteka=pamti;
                l=pamti;
            }
            else
            {
                (*l).pok=pamti; l=(*l).pok;
            }
            if(i!=VelicinaSteka-1)
            {
                pamti=(*pamti).pok;
            }
            k=(*k).pok;
        }
        return *this;
    }
    Stek<Tip> &operator =(Stek<Tip> &&stek)
    {
        std::swap(this,stek);
    }
    void brisi()
    {
        auto k=PocetakSteka;
        while(k!=nullptr)
        {
            auto pamti=(*k).pok;
            delete k;
            k=pamti;
        }
        PocetakSteka=nullptr;
        VelicinaSteka=0;
    }
    int brojElemenata() const
    {
        return VelicinaSteka;
    }
    void stavi(const Tip& el)
    {
        auto pamti=PocetakSteka;
        auto Novi=new Cvor<Tip>(el);
        PocetakSteka=Novi;
        (*PocetakSteka).pok=pamti;
        VelicinaSteka+=1;
    }
    Tip skini()
    {
        if(VelicinaSteka<=0)
        {
            throw std::range_error("Greska");
        }
        auto pamti=PocetakSteka;
        PocetakSteka=(*PocetakSteka).pok;
        auto vracaj=(*pamti).clan;
        delete pamti;
        VelicinaSteka--;
        return vracaj;
    }
    Tip &vrh()
    {
        if(VelicinaSteka<=0)
        {
            throw std::range_error("Greska");
        }
        return (*PocetakSteka).clan;
    }
};

template<typename Tip>
void Teststavi(Stek<Tip> &stek)
{
    stek.stavi(1);
    stek.stavi(2);
    std::cout<<stek.brojElemenata() << " " << stek.vrh() << "\n";
}

template<typename Tip>
void TestbrojElemenata(Stek<Tip> &stek)
{
    std::cout << stek.brojElemenata() << "\n";
}

template<typename Tip>
void Testvrh(Stek<Tip> &stek)
{
    std::cout << stek.vrh() << "\n";
}

template<typename Tip>
void Testskini(Stek<Tip> &stek)
{
    stek.skini();
    std::cout<<stek.brojElemenata() << " " << stek.vrh() << "\n";
}

template<typename Tip>
void Testbrisi(Stek<Tip> &stek)
{
    stek.brisi();
    std::cout<<stek.brojElemenata();
}

int main ()
{
    Stek<int> stek;
    Teststavi(stek);
    TestbrojElemenata(stek);
    Testvrh(stek);
    Testskini(stek);
    Testbrisi(stek);
    return 0;
}