#include <iostream>
#include <vector>
using namespace std;

template<typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual TipVrijednosti operator [](const TipKljuca &kljuc) const =0;
    virtual TipVrijednosti& operator [](const TipKljuca &kljuc) =0;
    virtual int brojElemenata() const =0;
    virtual void obrisi() =0;
    virtual void obrisi(const TipKljuca& kljuc) =0;
};


template<typename TipKljuca, typename TipVrijednosti>
class NizMapa: public Mapa<TipKljuca,TipVrijednosti>
{
    std::pair<TipKljuca,TipVrijednosti>* niz;
    int VelicinaMape;
    int KapacitetMape;
public:
    NizMapa()
    {
        niz = new std::pair<TipKljuca,TipVrijednosti>[50];
        KapacitetMape=50;
        VelicinaMape=0;
    }
    ~NizMapa()
    {
        delete[] niz;
        niz=nullptr;
    }
    NizMapa(const NizMapa<TipKljuca,TipVrijednosti> &mapa)
    {
        KapacitetMape=mapa.KapacitetMape;
        VelicinaMape=mapa.VelicinaMape;
        niz=new std::pair<TipKljuca,TipVrijednosti>[KapacitetMape];
        for(int i=0;i<VelicinaMape;i++)
        {
            niz[i]=mapa.niz[i];
        }
    }
    NizMapa(const NizMapa<TipKljuca,TipVrijednosti> &&mapa)
    {
        niz=mapa.niz;
        KapacitetMape=mapa.KapacitetMape;
        VelicinaMape=mapa.VelicinaMape;
        mapa.niz=nullptr;
    }
    NizMapa<TipKljuca,TipVrijednosti> &operator =(NizMapa<TipKljuca,TipVrijednosti> &mapa)
    {
        if(mapa.niz==niz)
        {
            return *this;
        }
        KapacitetMape=mapa.KapacitetMape;
        VelicinaMape=mapa.VelicinaMape;
        delete [] niz;
        niz=new std::pair<TipKljuca,TipVrijednosti>[KapacitetMape];
        for(int i=0; i<VelicinaMape;i++)
        {
            niz[i]=mapa.niz[i];
        }
        return *this;
    }
    NizMapa<TipKljuca,TipVrijednosti> &operator =(NizMapa<TipKljuca,TipVrijednosti> &&mapa)
    {
        if(mapa.niz==niz)
        {
            return *this;
        }
        std::swap(KapacitetMape,mapa.KapacitetMape);
        std::swap(VelicinaMape,mapa.VelicinaMape);
        std::swap(niz,mapa.niz);
        return *this;
    }
    TipVrijednosti operator [](const TipKljuca &kljuc) const
    {
        for(int i=0;i<VelicinaMape;i++)
        {
            if(niz[i].first==kljuc)
            {
                return niz[i].second;
            }
        }
        return TipVrijednosti();
    }
    TipVrijednosti& operator [](const TipKljuca &kljuc)
    {
        for(int i=0;i<VelicinaMape;i++)
        {
            if(niz[i].first==kljuc)
            {
                return niz[i].second;
            }
        }
        if(VelicinaMape<KapacitetMape)
        {
            niz[VelicinaMape]=std::make_pair(kljuc,TipVrijednosti());
            VelicinaMape++;
        }
        else
        {
            KapacitetMape=KapacitetMape+50;
            auto novi = new std::pair<TipKljuca,TipVrijednosti>[KapacitetMape];
            for(int i=0;i<VelicinaMape;i++)
            {
                novi[i]=niz[i];
            }
            novi[VelicinaMape]=std::make_pair(kljuc,TipVrijednosti());
            delete[] niz;
            niz=novi;
            VelicinaMape=VelicinaMape+1;
        }
        return niz[VelicinaMape-1].second;
    }
    int brojElemenata() const
    {
        return VelicinaMape;
    }
    void obrisi()
    {
        delete[] niz;
        KapacitetMape=50;
        VelicinaMape=0;
        niz=nullptr;
        niz = new std::pair<TipKljuca,TipVrijednosti>[50];
    }
    void obrisi(const TipKljuca& kljuc)
    {
        bool greska=true;
        for(int i=0;i<VelicinaMape;i++)
        {
            if(niz[i].first==kljuc)
            {
                greska=false;
                for(int j=i;j<VelicinaMape-1;j++)
                {
                    niz[j]=niz[j+1];
                }
                VelicinaMape=VelicinaMape-1;
                break;
            }
        }
        if(greska==true)
        {
            throw std::logic_error("Kljuc ne postoji");
        }
    }
};

void Test1()
{
    NizMapa<std::string,std::string> Mapa;
    std::cout << Mapa.brojElemenata() << "\n"; // 0
    Mapa["BiH"]="Sarajevo";
    std::cout << Mapa.brojElemenata() << "\n"; // 1
    Mapa["Srbija"]="Beograd";
    std::cout << Mapa.brojElemenata() << "\n"; // 2
    std::cout << Mapa["BiH"] << "\n"; // Sarajevo
    std::cout << Mapa["Srbija"] << "\n"; // Beograd
    std::cout << "\n";
}

void Test2()
{
    NizMapa<int,std::string> Mapa;
    Mapa[0]="Amer";
    Mapa[1]="Dzenita";
    Mapa[2]="Edin";
    std::cout << Mapa.brojElemenata() << "\n"; // 3
    NizMapa<int,std::string> Mapa2;
    Mapa2=Mapa;
    std::cout << Mapa2.brojElemenata() << "\n"; // 3
    std::cout << "\n";
}

void Test3()
{
    NizMapa<int,std::string> Mapa;
    Mapa[0]="Amer";
    Mapa[1]="Dzenita";
    Mapa[2]="Edin";
    std::cout << Mapa.brojElemenata() << "\n"; // 3
    Mapa.obrisi();
    std::cout << Mapa.brojElemenata() << "\n"; // 0
    std::cout << "\n";
}

void Test4()
{
    NizMapa<int,std::string> Mapa;
    Mapa[0]="Amer";
    Mapa[1]="Dzenita";
    Mapa[2]="Edin";
    std::cout << Mapa.brojElemenata() << "\n"; // 3
    Mapa.obrisi(0);
    Mapa.obrisi(1);
    std::cout << Mapa.brojElemenata() << "\n"; // 1
    std::cout << Mapa[2] << "\n"; // Edin
    std::cout << "\n";
}

void Test5()
{
    NizMapa<int,std::vector<std::string>> Mapa;
    std::vector<std::string> Vektor1;
    Vektor1.push_back("Amer");
    Vektor1.push_back("Dzenita");
    Vektor1.push_back("Edin");
    std::vector<std::string> Vektor2;
    Vektor2.push_back("Zenica");
    Vektor2.push_back("Sarajevo");
    Vektor2.push_back("Mostar");
    Mapa[0]=Vektor1;
    Mapa[1]=Vektor2;
    for(int i=0;i<Mapa[0].size();i++)
    {
        std::cout << Mapa[0][i] << " "; // Amer Dzenita Edin 
    }
    std::cout << "\n";
    for(int i=0;i<Mapa[1].size();i++)
    {
        std::cout << Mapa[1][i] << " "; // Zenica Sarajevo Mostar 
    }
    std::cout << "\n";
    std::cout << Mapa.brojElemenata() << "\n"; // 2
    Mapa.obrisi(0);
    std::cout << Mapa.brojElemenata() << "\n"; // 1
    Mapa.obrisi(1);
    std::cout << Mapa.brojElemenata() << "\n"; // 0
}

int main()
{
    Test1();
    Test2();
    Test3();
    Test4();
    Test5();
    return 0;
}