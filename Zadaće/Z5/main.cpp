#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>
using namespace std;

template <typename TipOznake>
class Grana;
template <typename TipOznake>
class Cvor;
template <typename TipOznake>
class GranaIterator;
template<typename TipKljuca,typename TipVrijednosti>
struct  CvorN
{
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    CvorN *next=nullptr;
};

template<typename TipKljuca, typename TipVrijednosti>
class Mapa
{
public:
    Mapa() {}
    virtual ~Mapa() {}
    virtual TipVrijednosti operator [](const TipKljuca kljuc) const =0;
    virtual TipVrijednosti& operator [](const TipKljuca kljuc) =0;
    virtual int brojElemenata() const =0;
    virtual void obrisi() =0;
    virtual void obrisi(const TipKljuca& kljuc) =0;
};

template<typename TipKljuca, typename TipVrijednosti>
class HashMapaLan: public Mapa<TipKljuca,TipVrijednosti>
{
    unsigned int brojEl;
    unsigned int KapacitetMape;
    CvorN<TipKljuca,TipVrijednosti>** nizElemenata;
    unsigned int (*f)(TipKljuca, unsigned int)=0;
public:
    HashMapaLan()
    {
        nizElemenata=nullptr;
        brojEl=0;
        KapacitetMape=50;
    }
    ~HashMapaLan()
    {
        this->obrisi();
    }
    HashMapaLan(HashMapaLan<TipKljuca,TipVrijednosti> &mapa)
    {
        KapacitetMape=mapa.KapacitetMape;
        brojEl=mapa.brojEl;
        f=mapa.f;
        nizElemenata=new CvorN<TipKljuca,TipVrijednosti>*[KapacitetMape];
        for(int i=0;i<KapacitetMape;i++)
        {
            nizElemenata[i]=nullptr;
        }
        for(int i=0;i<KapacitetMape;i++)
        {
            if(mapa.nizElemenata[i]!=nullptr)
            {
                nizElemenata[i]=new CvorN<TipKljuca,TipVrijednosti>();
                nizElemenata[i]->kljuc=mapa.nizElemenata[i]->kljuc;
                nizElemenata[i]->vrijednost=mapa.nizElemenata[i]->vrijednost;
                auto r=nizElemenata[i];
                auto p=mapa.nizElemenata[i];
                while(p->next!=nullptr)
                {
                    r->next=new CvorN<TipKljuca,TipVrijednosti>();
                    p=p->next;
                    r->next->kljuc=p->kljuc;
                    r->next->vrijednost=p->vrijednost;
                }
            }
        }
    }
    HashMapaLan(HashMapaLan<TipKljuca,TipVrijednosti> &&mapa)
    {
        KapacitetMape=mapa.KapacitetMape;
        brojEl=mapa.brojEl;
        f=mapa.f;
        nizElemenata=mapa.nizElemenata;
    }
    HashMapaLan &operator = (const HashMapaLan<TipKljuca,TipVrijednosti> &mapa)
    {
        if(&mapa==this)
        {
            return *this;
        }
        if(&mapa!=this)
        {
            for(int i=0;i<KapacitetMape;i++)
            {
                if(nizElemenata[i]!=nullptr)
                {
                    while(nizElemenata[i]!=nullptr)
                    {
                        auto trenutni=nizElemenata[i];
                        nizElemenata[i]=nizElemenata[i]->next;
                        delete trenutni;
                    }
                    nizElemenata[i]=nullptr;
                }
            }
            delete [] nizElemenata;
            KapacitetMape=mapa.KapacitetMape;
            brojEl=mapa.brojEl;
            f=mapa.f;
            nizElemenata=new CvorN<TipKljuca,TipVrijednosti>*[KapacitetMape];
            for(int i=0;i<KapacitetMape;i++)
            {
                nizElemenata[i]=nullptr;
            }
            for(int i=0;i<mapa.KapacitetMape;i++)
            {
                if(mapa.nizElemenata[i]!=nullptr)
                {
                    nizElemenata[i]=new CvorN<TipVrijednosti,TipVrijednosti>();
                    nizElemenata[i]->kljuc=mapa.nizElemenata[i]->kljuc;
                    nizElemenata[i]->vrijednost=mapa.nizElemenata[i]->vrijednost;
                    auto r=nizElemenata[i];
                    auto p=mapa.nizElemenata[i];
                    while(p->next!=nullptr)
                    {
                        r->next=new CvorN<TipKljuca,TipVrijednosti>();
                        p=p->next;
                        r->next->kljuc=p->kljuc;
                        r->next->vrijednost=p->vrijednost;
                    }
                }
            }
        }
        return *this;
    }
    HashMapaLan &operator = (HashMapaLan<TipKljuca,TipVrijednosti> &&mapa)
    {
        if(this!=&mapa)
        {
            for(int i=0;i<KapacitetMape;i++)
            {
                if(nizElemenata[i]!=nullptr)
                {
                    while(nizElemenata[i]->next!=nullptr)
                    {
                        auto trenutni=nizElemenata[i];
                        nizElemenata[i]=nizElemenata[i]->next;
                        delete trenutni;
                    }
                    delete nizElemenata[i];
                }
            }
            delete [] nizElemenata;
            nizElemenata=mapa.nizElemenata;
            brojEl=mapa.brojEl;
            KapacitetMape=mapa.KapacitetMape;
            f=mapa.f;
        }
        return *this;
    }
    TipVrijednosti operator[] (TipKljuca k) const
    {
        if(f==0)
        {
            throw std::domain_error("Nema hash funckije");
        }
        unsigned int adresa=f(k,KapacitetMape);
        if(nizElemenata[adresa]!=nullptr)
        {
            auto p=nizElemenata[adresa];
            while(p!=nullptr)
            {
                if(p->kljuc==k)
                {
                    return p->vrijednost;
                }
                p=p->next;
            }
        }
        return TipVrijednosti();
    }
    TipVrijednosti &operator[] (TipKljuca k)
    {
        if(f==0)
        {
            throw std::domain_error("Nema hash funckije");
        }
        unsigned int adresa=f(k,KapacitetMape);
        if(brojEl==0)
        {
            nizElemenata=new CvorN<TipKljuca,TipVrijednosti>*[KapacitetMape];
            nizElemenata[adresa]=new CvorN<TipKljuca,TipVrijednosti>();
            nizElemenata[adresa]->kljuc=k;
            nizElemenata[adresa]->vrijednost=TipVrijednosti();
            brojEl=brojEl+1;
            for(int i=0;i<KapacitetMape;i++)
            {
                if(i!=adresa)
                {
                    nizElemenata[i]=nullptr;
                }
            }
            return nizElemenata[adresa]->vrijednost;
        }
        else
        {
            if(nizElemenata[adresa]!=nullptr)
            {
                auto p=nizElemenata[adresa];
                while(p!=nullptr)
                {
                    if(p->kljuc==k)
                    {
                        return p->vrijednost;
                    }
                    p=p->next;
                }
            }
            if(nizElemenata[adresa]==nullptr)
            {
                nizElemenata[adresa]=new CvorN<TipKljuca,TipVrijednosti>();
                nizElemenata[adresa]->kljuc=k;
                nizElemenata[adresa]->vrijednost=TipVrijednosti();
                brojEl=brojEl+1;
                return nizElemenata[adresa]->vrijednost;
            }
            else
            {
                if(k<nizElemenata[adresa]->kljuc)
                {
                    auto pomocni=nizElemenata[adresa];
                    nizElemenata[adresa]=new CvorN<TipKljuca,TipVrijednosti>();
                    nizElemenata[adresa]->kljuc=k;
                    nizElemenata[adresa]->vrijednost=TipVrijednosti();
                    nizElemenata[adresa]->next=pomocni;
                    brojEl=brojEl+1-1+1;
                    return nizElemenata[adresa]->vrijednost;
                }
                auto prije=nizElemenata[adresa];
                auto p=nizElemenata[adresa];
                while(p->next!=nullptr && p->kljuc<k)
                {
                    prije=p;
                    p=p->next;
                }
                if(p->next==nullptr)
                {
                    p->next=new CvorN<TipKljuca,TipVrijednosti>();
                    p->next->kljuc=k;
                    p->next->vrijednost=TipVrijednosti();
                    brojEl=brojEl+1;
                    return p->next->vrijednost;
                }
                else
                {
                    auto p1=new CvorN<TipKljuca,TipVrijednosti>();
                    p1->kljuc=k;
                    p1->vrijednost=TipVrijednosti();
                    prije->next=p1;
                    p1->next=nizElemenata[adresa];
                    brojEl=brojEl+1-1+1;
                    return p1->vrijednost;
                }
            }
        }
    }
    void obrisi()
    {
        if(brojEl==0)
        {
            return;
        }
        for(int i=0;i<KapacitetMape;i++)
        {
            if(nizElemenata[i]!=nullptr)
            {
                while(nizElemenata[i]!=nullptr)
                {
                    auto trenutni=nizElemenata[i];
                    nizElemenata[i]=nizElemenata[i]->next;
                    delete trenutni;
                }
                nizElemenata[i]=nullptr;
            }
        }
        delete [] nizElemenata;
        brojEl=0;
        KapacitetMape=50;
        nizElemenata=nullptr;
    }
    void obrisi(const TipKljuca &k)
    {
        if(brojEl==0)
        {
            throw std::domain_error("Nema elemenata u mapi");
        }
        unsigned int adresa=f(k,KapacitetMape);
        bool ima=false;
        if(nizElemenata[adresa]!=nullptr)
        {
            if(nizElemenata[adresa]->kljuc==k)
            {
                ima=true;
                nizElemenata[adresa]->vrijednost=TipVrijednosti();
                brojEl=brojEl+(-1);
                return;
            }
            auto pok=nizElemenata[adresa];
            while(pok!=nullptr)
            {
                if(pok->next!=nullptr)
                {
                    auto p=pok->next;
                    if(p->kljuc==k)
                    {
                        ima=true;
                        p->vrijednost=TipVrijednosti();
                        break;
                    }
                }
                pok=pok->next;
            }
            brojEl=brojEl+(-1);
        }
        if(ima==false)
        {
            throw std::domain_error("Nema elemenata u mapi");
        }
    }
    int brojElemenata() const
    {
        return brojEl;
    }
    void definisiHashFunkciju(unsigned int(*fun)(TipKljuca,unsigned int))
    {
        if(fun==nullptr)
        {
            throw std::domain_error("Nije definisana hash funkcija");
        }
        f=fun;
    }
};

template <typename TipOznake>
class UsmjereniGraf
{
public:
    UsmjereniGraf()=default;
    virtual int dajBrojCvorova()=0;
    virtual ~UsmjereniGraf() {};
    virtual void postaviBrojCvorova(int broj)=0;
    virtual void dodajGranu(int polazni, int dolazni, float tezina)=0;
    virtual void obrisiGranu(int polazni, int dolazni)=0;
    virtual void postaviTezinuGrane(int polazni, int dolazni,float tezina=0)=0;
    virtual float dajTezinuGrane(int polazni, int dolazni)=0;
    virtual bool postojiGrana(int polazni, int dolazni)=0;
    virtual void postaviOznakuCvora(int broj, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuCvora(int broj)=0;
    virtual void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka)=0;
    virtual TipOznake dajOznakuGrane(int polazni, int dolazni)=0;
    virtual Cvor<TipOznake> &dajCvor(int broj)=0;
    virtual Grana<TipOznake> &dajGranu(int polozeni, int dolazni)=0;
    virtual GranaIterator<TipOznake> dajGranePocetak()=0;
    virtual GranaIterator<TipOznake> dajGraneKraj()=0;
};

template <typename TipOznake>
class Cvor
{
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *pokGraf;
    int redniBroj;
public:
    Cvor(int rednibroj, UsmjereniGraf<TipOznake> &graf)
    {
        pokGraf= &graf;
        redniBroj=rednibroj;
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }
    void postaviOznaku(TipOznake novaoznaka)
    {
        oznaka=novaoznaka;
    }
    int dajRedniBroj()
    {
        return redniBroj;
    }
};

template <typename TipOznake>
class Grana
{
    int polazniCvor;
    int dolazniCvor;
    float tezina;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *pokGraf;
public:
    Grana(int a, int b, UsmjereniGraf<TipOznake> &graf, float tez=0)
    {
        polazniCvor=a;
        dolazniCvor=b;
        tezina=tez;
        pokGraf=&graf;
    }
    Grana(const Grana<TipOznake> &grana)
    {
        polazniCvor=grana.polazniCvor;
        dolazniCvor=grana.dolazniCvor;
        tezina=grana.tezina;
        pokGraf=grana.pokGraf;
        oznaka=grana.oznaka;
    }
    float dajTezinu()
    {
        return tezina;
    }
    void postaviTezinu(float tez=0)
    {
        pokGraf->dajGranu(polazniCvor,dolazniCvor).tezina=tez;
        tezina=tez;
    }
    TipOznake dajOznaku()
    {
        return oznaka;
    }
    void postaviOznaku(TipOznake ozn)
    {
        pokGraf->dajGranu(polazniCvor,dolazniCvor).oznaka=ozn;
        oznaka=ozn;
    }
    bool operator == (Grana<TipOznake> grana)
    {
        if(grana.polazniCvor==polazniCvor && grana.dolazniCvor==dolazniCvor)
        {
            return true;
        }
        return false;
    }
    bool operator != (Grana<TipOznake> grana)
    {
        if(grana.polazniCvor==polazniCvor && grana.dolazniCvor==dolazniCvor)
        {
            return false;
        }
        return true;
    }
    Cvor<TipOznake> dajPolazniCvor()
    {
        return pokGraf->dajCvor(polazniCvor);
    }
        Cvor<TipOznake> dajDolazniCvor()
    {
        return pokGraf->dajCvor(dolazniCvor);
    }
};

template <typename TipOznake>
class GranaIterator
{
    int pozicijaMatrica;
    int dolazniCvor;
    int polazniCvor;
    std::vector<Grana<TipOznake>> grane;
    UsmjereniGraf<TipOznake> *graf=nullptr;
public:
    GranaIterator(UsmjereniGraf<TipOznake> &g, std::vector<Grana<TipOznake>> gr, int p, int d, int poz)
    {
        polazniCvor=p;
        dolazniCvor=d;
        grane=gr;
        pozicijaMatrica=poz;
        graf=&g;
    }
    Grana<TipOznake> operator*()
    {
        int brojac=0;
        for(int i=0;i<graf->dajBrojCvorova();i++)
        {
            for(int j=0;j<graf->dajBrojCvorova();j++)
            {
                if(graf->postojiGrana(i,j) && brojac<pozicijaMatrica)
                {
                    brojac=brojac+1;
                }
                else if(brojac>=pozicijaMatrica && graf->postojiGrana(i,j))
                {
                    return graf->dajGranu(i,j);
                }
            }
        }
        throw std::domain_error("Nema grane");
    }
    GranaIterator(const GranaIterator<TipOznake> &grana)
    {
        polazniCvor=grana.polazniCvor;
        dolazniCvor=grana.dolazniCvor;
        pozicijaMatrica=grana.pozicijaMatrica;
        grane=grana.grane;
    }
    GranaIterator<TipOznake> &operator = (const GranaIterator<TipOznake> &grana)
    {
        polazniCvor=grana.polazniCvor;
        dolazniCvor=grana.dolazniCvor;
        pozicijaMatrica=grana.pozicijaMatrica;
        grane=grana.grane;
    }
    bool operator == (const GranaIterator &granait) const
    {
        if(polazniCvor==granait.polazniCvor && dolazniCvor==granait.dolazniCvor)
        {
            return true;
        }
        return false;
    }
    bool operator != (const GranaIterator &granait) const
    {
        if(polazniCvor==granait.polazniCvor && dolazniCvor==granait.dolazniCvor)
        {
            return false;
        }
        return true;
    }
    GranaIterator &operator++()
    {
        pozicijaMatrica=pozicijaMatrica+1;
        return *this;
    }
    GranaIterator operator++(int)
    {
        pozicijaMatrica=pozicijaMatrica+1;
        return *this;
    }
};

template <typename TipOznake>
class ListaGraf: public UsmjereniGraf<TipOznake>
{
    std::vector<std::vector<int>> lista;
    std::vector<Cvor<TipOznake>> cvorovi;
    std::vector<Grana<TipOznake>> grane;
public:
    ListaGraf(int broj)
    {
        if(broj<=0)
        {
            throw std::domain_error("Greska");
        }
        for(int i=0;i<broj;i++)
        {
            cvorovi.push_back(Cvor<TipOznake>(i,*this));
            lista.push_back(std::vector<int>());
        }
    }
    ~ListaGraf() {}
    ListaGraf(const ListaGraf<TipOznake> &m)
    {
        grane=m.grane;
        cvorovi=m.cvorovi;
        lista=m.lista;
    }
    ListaGraf<TipOznake> &operator = (const ListaGraf<TipOznake> &m)
    {
        if(&m==this)
        {
            return *this;
        }
        cvorovi=m.cvorovi;
        lista=m.lista;
        grane=m.grane;
        return *this;
    }
    int dajBrojCvorova()
    {
        return cvorovi.size();
    }
    void postaviBrojCvorova(int broj)
    {
        if(broj<lista.size())
        {
            throw std::domain_error("Greska");
        }
        int stara(lista.size());
        for(int i=stara;i<broj;i++)
        {
            cvorovi.push_back(Cvor<TipOznake>(i,*this));
            lista.push_back(std::vector<int>());
        }
    }
    void dodajGranu(int polazni, int dolazni, float tezina=0)
    {
        if(polazni>=cvorovi.size() || dolazni>=cvorovi.size() || polazni<0 || dolazni<0)
        {
            throw std::domain_error("Cvor je prekoracio opseg");
        }
        for(int j=0;j<lista.at(polazni).size();j++)
        {
            if(lista.at(polazni).at(j)==dolazni)
            {
                throw std::domain_error("Grana vec postoji");
            }
        }
        lista.at(polazni).push_back(dolazni);
        grane.push_back(Grana<TipOznake>(polazni,dolazni,*this,tezina));
    }
    void obrisiGranu(int polazni, int dolazni)
    {
        for(int i=0;i<lista.at(polazni).size();i++)
        {
            if(lista.at(polazni).at(i)==dolazni)
            {
                lista.at(polazni).erase(lista.at(polazni).begin()+i);
                break;
            }
        }
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,0))
            {
                grane.erase(grane.begin()+i);
                return;
            }
        }
    }
    void postaviTezinuGrane(int polazni, int dolazni, float tezina=0)
    {
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,tezina))
            {
                grane.at(i).postaviTezinu(tezina);
                return;
            }
        }
        throw std::domain_error("Grana nije pronadjena");
    }
    float dajTezinuGrane(int polazni, int dolazni)
    {
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,0))
            {
                return grane.at(i).dajTezinu();
            }
        }
        throw std::domain_error("Grana nije pronadjena");
    }
    bool postojiGrana(int polazni, int dolazni)
    {
        for(int j=0;j<lista.at(polazni).size();j++)
        {
            if(lista.at(polazni).at(j)==dolazni)
            {
                return true;
            }
        }
        return false;
    }
    void postaviOznakuCvora(int broj, TipOznake oznaka)
    {
        if(broj>=cvorovi.size() || broj<0)
        {
            throw std::domain_error("Cvor ne postoji");
        }
        cvorovi.at(broj).postaviOznaku(oznaka);
    }
    TipOznake dajOznakuCvora(int broj)
    {
        if(broj>=cvorovi.size() ||broj<0)
        {
            throw std::domain_error("Cvor ne postoji");
        }
        return cvorovi.at(broj).dajOznaku();
    }
    void postaviOznakuGrane(int polazni, int dolazni, TipOznake oznaka)
    {
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,0))
            {
                grane.at(i).postaviOznaku(oznaka);
                return;
            }
        }
        throw std::domain_error("Grana nije pronadjena");
    }
    TipOznake dajOznakuGrane(int polazni, int dolazni)
    {
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,0))
            {
                return grane.at(i).dajOznaku();
            }
        }
        throw std::domain_error("Grana nije pronadjena");
    }
    Cvor<TipOznake> &dajCvor(int broj)
    {
        return cvorovi.at(broj);
    }
    Grana<TipOznake> &dajGranu(int polazni, int dolazni)
    {
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,0))
            {
                return grane.at(i);
            }
        }
        throw std::domain_error("Grana nije pronadjena");
    }
    GranaIterator<TipOznake> dajGranePocetak()
    {
        return GranaIterator<TipOznake>(*this,grane,grane.at(0).dajPolazniCvor().dajRedniBroj(),
        grane.at(0).dajDolazniCvor().dajRedniBroj(),0);
    }
    GranaIterator<TipOznake> dajGraneKraj()
    {
        return GranaIterator<TipOznake>(*this,grane,grane.at(grane.size()-1).dajPolazniCvor().dajRedniBroj(),
        grane.at(grane.size()-1).dajDolazniCvor().dajRedniBroj(),grane.size()-1);
    }
};

template<typename TipOznake>
void bfs(UsmjereniGraf<TipOznake> *graf, std::vector<Cvor<TipOznake>> &bfs_obilazak,Cvor<TipOznake> pocetni_cvor)
{
    std::queue<Cvor<TipOznake>> red;
    red.push(pocetni_cvor);
    bfs_obilazak.push_back(pocetni_cvor);
    while(!red.empty() && bfs_obilazak.size()!=graf->dajBrojCvorova())
    {
        Cvor<TipOznake> cvor=red.front();
        red.pop();
        bool pamti=false;
        std::vector<Cvor<TipOznake>> izlazeci;
        for(int i=0;i<graf->dajBrojCvorova();i++)
        {
            if(graf->postojiGrana(cvor.dajRedniBroj(),i))
            {
                izlazeci.push_back(graf->dajCvor(i));
                red.push(graf->dajCvor(i));
            }
        }
        for(int i=0;i<izlazeci.size();i++)
        {
            pamti=false;
            for(int j=0;j<bfs_obilazak.size();j++)
            {
                if(izlazeci[i].dajRedniBroj()==bfs_obilazak[j].dajRedniBroj())
                {
                    pamti=true;
                }
            }
            if(!pamti)
            {
                bfs_obilazak.push_back(izlazeci[i]);
            }
        }
    }
}

template <typename TipOznake>
void dfs(UsmjereniGraf<TipOznake>* &graf, std::vector<Cvor<TipOznake>> &dfs_obilazak,Cvor<TipOznake> &cvor)
{
    bool pamti=false;
    for(int i=0;i<dfs_obilazak.size();i++)
    {
        if(dfs_obilazak[i].dajRedniBroj()==cvor.dajRedniBroj())
        {
            pamti=true;
            break;
        }
    }
    if(!pamti)
    {
        dfs_obilazak.push_back(cvor);
    }
    for(int i=0;i<graf->dajBrojCvorova();i++)
    {
        if(graf->postojiGrana(cvor.dajRedniBroj(),i) && !pamti)
        {
            dfs(graf,dfs_obilazak,graf->dajCvor(i));
        }
    }
}

int main()
{
    return 0;
}