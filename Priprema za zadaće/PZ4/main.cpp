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
class Red
{
    int VelicinaReda;
    Cvor<Tip>* PocetakReda;
    Cvor<Tip>* KrajReda;
public:
    Red()
    {
        VelicinaReda=0;
        PocetakReda=nullptr;
        KrajReda=nullptr;
    }
    ~Red()
    {
        this->brisi();
    }
    Red(const Red<Tip> &r)
    {
        this->VelicinaReda=r.VelicinaReda;
        auto pokazivac=r.PocetakReda;
        auto pomocni=pokazivac;
        auto sljedeci=pokazivac;
        for(int i=0;i<this->VelicinaReda;i++)
        {
            pomocni=new Cvor<Tip>(pokazivac->clan);
            if(i==0)
            {
                this->PocetakReda=pomocni;
                sljedeci=pomocni;
            }
            else
            {
                sljedeci->pok=pomocni;
                sljedeci=sljedeci->pok;
            }
            if(i!=this->VelicinaReda-1)
            {
                pomocni=pomocni->pok;
            }
            else
            {
                KrajReda=pomocni;
            }
            pokazivac=pokazivac->pok;
        }
    }
    Red(Red<Tip> &&r)
    {
        this->PocetakReda=r.PocetakReda;
        this->VelicinaReda=r.VelicinaReda;
        r.PocetakReda=nullptr;
        r.VelicinaReda=0;
    }
    Red<Tip> &operator=(const Red<Tip> &r)
    {
        if(this->PocetakReda!=r.PocetakReda && this->KrajReda!=r.KrajReda)
        {
            auto pokazivac=PocetakReda;
            auto sljedeci=pokazivac;
            while(pokazivac!=nullptr)
            {
                sljedeci=pokazivac->pok;
                delete pokazivac;
                pokazivac=sljedeci;
            }
            this->PocetakReda=nullptr;
            this->VelicinaReda=r.VelicinaReda;
            auto pomocni=sljedeci;
            pokazivac=r.PocetakReda;
            for(int i=0;i<this->VelicinaReda;i++)
            {
                pomocni=new Cvor<Tip>(pokazivac->clan);
                if(i==0)
                {
                    this->PocetakReda=pomocni;
                    sljedeci=pomocni;
                }
                else
                {
                    sljedeci->pok=pomocni;
                    sljedeci=sljedeci->pok;
                }
                if(i!=this->VelicinaReda-1)
                {
                    pomocni=pomocni->pok;
                }
                else
                {
                    KrajReda=pomocni;
                }
                pokazivac=pokazivac->pok;
            }
        }
        return *this;
    }
    Cvor<Tip> &operator =(Red<Tip> &&r)
    {
        std::swap(this->PocetakReda,r.PocetakReda);
        std::swap(this->KrajReda,r.KrajReda);
        std::swap(this->VelicinaReda,r.VelicinaReda);
    }
    void brisi()
    {
        auto pokazivac=PocetakReda;
        auto sljedeci=PocetakReda;
        while(pokazivac!=nullptr)
        {
            sljedeci=pokazivac->pok;
            delete pokazivac;
            pokazivac=sljedeci;
        }
        VelicinaReda=0;
        PocetakReda=nullptr;
    }
    void stavi(const Tip& el)
    {
        auto dodajovaj=new Cvor<Tip>(el);
        if(VelicinaReda==0)
        {
            KrajReda=dodajovaj;
            PocetakReda=KrajReda;
        }
        else
        {
            KrajReda->pok=dodajovaj;
            KrajReda=KrajReda->pok;
        }
        VelicinaReda++;
    }
    Tip skini()
    {
        if(VelicinaReda<1)
        {
            throw std::range_error("Greska");
        }
        auto brisime=PocetakReda;
        auto vratime=brisime->clan;
        PocetakReda=PocetakReda->pok;
        delete brisime;
        VelicinaReda+=-1;
        if(VelicinaReda==0)
        {
            KrajReda=nullptr;
        }
        return vratime;
    }
    Tip &celo()
    {
        if(VelicinaReda<1)
        {
            throw std::range_error("Greska");
        }
        return PocetakReda->clan;
    }
    int brojElemenata() const
    {
        return this->VelicinaReda;
    }
};


template<typename Tip>
void Teststavi(Red<Tip> &red)
{
    red.stavi(1);
    red.stavi(2);
    std::cout<<red.brojElemenata() << " " << red.celo() << "\n";
}

template<typename Tip>
void TestbrojElemenata(Red<Tip> &red)
{
    std::cout << red.brojElemenata() << "\n";
}

template<typename Tip>
void Testcelo(Red<Tip> &red)
{
    std::cout << red.celo() << "\n";
}

template<typename Tip>
void Testskini(Red<Tip> &red)
{
    red.skini();
    std::cout<<red.brojElemenata() << " " << red.celo() << "\n";
}

template<typename Tip>
void Testbrisi(Red<Tip> &red)
{
    red.brisi();
    std::cout<<red.brojElemenata();
}

int main ()
{
    Red<int> red;
    Teststavi(red);
    TestbrojElemenata(red);
    Testcelo(red);
    Testskini(red);
    Testbrisi(red);
    return 0;
}