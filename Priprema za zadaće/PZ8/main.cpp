#include <iostream>
#include <cstdlib>
#include <chrono>
#include <ctime>  
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

template<typename TipKljuca, typename TipVrijednosti>
class Cvor
{
public:
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    Cvor* roditelj;
    Cvor* desno;
    Cvor* lijevo;
    Cvor() {}
    Cvor(const TipKljuca& kljuc, const TipVrijednosti& vrijednost,Cvor* roditelj, Cvor* desno, Cvor* lijevo)
    {
        this->roditelj=roditelj;;
        this->desno=desno;
        this->lijevo=lijevo;
        this->vrijednost=vrijednost;
        this->kljuc=kljuc;
    }
};

template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa: public Mapa<TipKljuca,TipVrijednosti>
{
    Cvor<TipKljuca,TipVrijednosti>* korijen=nullptr;
    int velicina=0;
public:
    BinStabloMapa(){}
    ~BinStabloMapa()
    {
        brisisve(korijen);
        korijen=nullptr;
    }
    void brisisve(Cvor<TipKljuca,TipVrijednosti>* korijen)
    {
        if(korijen)
        {
            brisisve(korijen->desno);
            brisisve(korijen->lijevo);
            delete korijen;
        }
    }
    void umetniIterativno(Cvor<TipKljuca,TipVrijednosti>* &r, Cvor<TipKljuca,TipVrijednosti>* &z)
    {

        Cvor<TipKljuca,TipVrijednosti>* x=r;
        Cvor<TipKljuca,TipVrijednosti>* y=nullptr;
        while(x!=nullptr)
        {
            y=x;
            if(z->kljuc<x->kljuc)
            {
                x=x->lijevo;
            }
            else
            {
                x=x->desno;
            }
        }
        if(y==0)
        {
            r=z;
        }
        else
        {
            if(z->kljuc<y->kljuc)
            {
                y->lijevo=z;
            }
            else
            {
                y->desno=z;
            }
            if(z!=nullptr)
            {
                z->roditelj=y;
            }
        }
    }
    void Inorder(Cvor<TipKljuca,TipVrijednosti>* korijen1, Cvor<TipKljuca,TipVrijednosti>* &korijen2)
    {
        if(korijen1!=nullptr)
        {
            Inorder(korijen1->lijevo,korijen2);
            Cvor<TipKljuca,TipVrijednosti>* novi=new Cvor<TipKljuca,TipVrijednosti>(korijen1->kljuc,korijen1->vrijednost,nullptr,nullptr,nullptr);
            umetniIterativno(korijen2, novi);
            Inorder(korijen1->desno,korijen2);
        }
    }
    BinStabloMapa(const BinStabloMapa<TipKljuca,TipVrijednosti> &a)
    {
        korijen=nullptr;
        Inorder(a.korijen,korijen);
        velicina=a.velicina;
    }
    BinStabloMapa<TipKljuca,TipVrijednosti> &operator =(const BinStabloMapa<TipKljuca,TipVrijednosti> &a)
    {
        if(&a!=this)
        {
            this->obrisi();
            Inorder(a.korijen,korijen);
            velicina=a.velicina;
            return *this;
        }
        return *this;
    }
    TipVrijednosti operator [](const TipKljuca &kljuc) const
    {
        if(velicina==0)
        {
            return TipVrijednosti();
        }
        auto vrijednost=trazi(korijen,kljuc);
        if(vrijednost==nullptr)
        {
            return TipVrijednosti();
        }
        return vrijednost->vrijednost;
    }
    TipVrijednosti& operator [](const TipKljuca &kljuc)
    {
        if(velicina==0)
        {
            korijen=new Cvor<TipKljuca,TipVrijednosti>(kljuc,TipVrijednosti(),nullptr,nullptr,nullptr);
            velicina=velicina+1;
            return korijen->vrijednost;
        }
        auto vrijednost=trazi(korijen,kljuc);
        {
            if(vrijednost!=nullptr)
            {
                return vrijednost->vrijednost;
            }
            velicina=velicina+1;
            auto pokazivac=umetni(korijen,kljuc);
            return pokazivac->vrijednost;
        }
    }
    Cvor<TipKljuca,TipVrijednosti>* trazi(Cvor<TipKljuca,TipVrijednosti>* korijen, const TipKljuca& kljuc) const
    {
        while(korijen!=nullptr && kljuc!=korijen->kljuc)
        {
            if(kljuc<korijen->kljuc)
            {
                korijen=korijen->lijevo;
            }
            else
            {
                korijen=korijen->desno;
            }
        }
        return korijen;
    }
    Cvor<TipKljuca,TipVrijednosti>* &umetni(Cvor<TipKljuca,TipVrijednosti>* &korijen, const TipKljuca& kljuc)
    {
        if(korijen==nullptr)
        {
            korijen=new Cvor<TipKljuca,TipVrijednosti>(kljuc,TipVrijednosti(),nullptr,nullptr,nullptr);
            return korijen;
        }
        else if(kljuc<korijen->kljuc)
        {
            return umetni(korijen->lijevo,kljuc);
        }
        return umetni(korijen->desno,kljuc);
    }
    int brojElemenata() const
    {
        return this->velicina;
    }
    void obrisi()
    {
        brisisve(korijen);
        korijen=nullptr;
        velicina=0;
    }
    void obrisi(const TipKljuca& kljuc)
    {
        obrisiCvor(korijen,kljuc);
        velicina=velicina+(-1);
    }
    void obrisiCvor(Cvor<TipKljuca,TipVrijednosti>* &r, const TipKljuca& kljuc)
    {
        Cvor<TipKljuca,TipVrijednosti>* p=r;
        Cvor<TipKljuca,TipVrijednosti>* q=nullptr;
        Cvor<TipKljuca,TipVrijednosti>* pp=nullptr;
        Cvor<TipKljuca,TipVrijednosti>* rp=nullptr;
        Cvor<TipKljuca,TipVrijednosti>* pomocni=nullptr;
        while(p!=nullptr && kljuc!=p->kljuc)
        {
            q=p;
            if(kljuc<p->kljuc)
            {
                p=p->lijevo;
            }
            else
            {
                p=p->desno;
            }
        }
        if(p==nullptr)
        {
            return;
        }
        if(p->lijevo==nullptr)
        {
            rp=p->desno;
        }
        else if(p->desno==nullptr)
        {
            rp=p->lijevo;
        }
        else
        {
            pp=p;
            rp=p->lijevo;
            pomocni=rp->desno;
            while(pomocni!=nullptr)
            {
                pp=rp;
                rp=pomocni;
                pomocni=rp->desno;
            }
            if(pp!=p)
            {
                pp->desno=rp->lijevo;
                if(rp->lijevo!=nullptr)
                {
                    rp->lijevo->roditelj=pp;
                }
                rp->lijevo=p->lijevo;
                if(p->lijevo!=nullptr)
                {
                    p->lijevo->roditelj=rp;
                }
            }
            rp->desno=p->desno;
            if(p->desno!=nullptr)
            {
                p->desno->roditelj=rp;
            }
        }
        if(q==nullptr)
        {
            korijen=rp;
        }
        else if(p==q->lijevo)
        {
            q->lijevo=rp;
            if(rp!=nullptr)
            {
                rp->roditelj=q;
            }
        }
        else
        {
            q->desno=rp;
            if(rp!=nullptr)
            {
                rp->roditelj=q;
            }
        }
        delete p;
    }
};


int main()
{
    NizMapa<int,int> Nizm;
    BinStabloMapa<int,int> BinSm;
    clock_t prije;
    clock_t poslije;
    auto pocetak = std::chrono::system_clock::now();
    for(int i=0; i<10000; i++)
    {
        int random = rand() % 10000;
        Nizm[i]=random;
    }
    auto kraj = std::chrono::system_clock::now();
    std::chrono::duration<double> vrijeme = kraj-pocetak;
    std::cout << "Dodavanje elemenata u NizMape je trajalo: " << vrijeme.count() << " sekundi";
    pocetak = std::chrono::system_clock::now();
    for(int i=0; i<10000; i++)
    {
        int random = rand() % 10000;
        BinSm[i]=random;
    }
    kraj = std::chrono::system_clock::now();
    vrijeme = kraj-pocetak;
    std::cout << "\nDodavanje elemenata u BinStabloMape je trajalo: " << vrijeme.count() << " sekundi";
    pocetak = std::chrono::system_clock::now();
    for(int i=0; i<10000; i++)
    {
        Nizm[i];
    }
    kraj = std::chrono::system_clock::now();
    vrijeme = kraj-pocetak;
    std::cout << "\nPristup elemenatima u NizMape je trajalo: " << vrijeme.count() << " sekundi";
    pocetak = std::chrono::system_clock::now();
    for(int i=0; i<10000; i++)
    {
        BinSm[i];
    }
    kraj = std::chrono::system_clock::now();
    vrijeme = kraj-pocetak;
    std::cout << "\nPristup elemenatima u BinStabloMape je trajalo: " << vrijeme.count() << " sekundi";
}

// Dodavanje elemenata u NizMape je znacajno brze od dodavanja elemenata u BinStabloMape
// Pristup elemenetima u NizMape je otprilike duplo brzi nego pristup elementima u BinStabloMape