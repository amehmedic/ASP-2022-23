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

template <typename TipOznake>
class UsmjereniGraf
{
public:
    UsmjereniGraf()=default;
    virtual ~UsmjereniGraf() {};
    virtual int dajBrojCvorova()=0;
    virtual void postaviBrojCvorova(int broj)=0;
    virtual void dodajGranu(int polazni, int dolazni, double tezina)=0;
    virtual void obrisiGranu(int polazni, int dolazni)=0;
    virtual void postaviTezinuGrane(int polazni, int dolazni,double tezina=0)=0;
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
    double tezina;
    TipOznake oznaka;
    UsmjereniGraf<TipOznake> *pokGraf;
public:
    Grana(int a, int b, UsmjereniGraf<TipOznake> &graf, double tez=0)
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
    double dajTezinu()
    {
        return tezina;
    }
    void postaviTezinu(double tez=0)
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
    std::vector<std::vector<bool>> matricaVrijednosti;
public:
    Grana<TipOznake> operator*() const
    {
        int brojac=0;
        int red=-1;
        int kolona=-1;
        bool da=false;
        for(int i=0;i<matricaVrijednosti.size();i++)
        {
            for(int j=0;j<matricaVrijednosti.at(0).size();j++)
            {
                if(matricaVrijednosti.at(i).at(j) && brojac<=pozicijaMatrica)
                {
                    brojac=brojac+1;
                    red=i;
                    kolona=j;
                }
                else if(brojac>pozicijaMatrica)
                {
                    da=true;
                    break;
                }
            }
            if(da==true)
            {
                break;
            }
        }
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i).dajPolazniCvor().dajRedniBroj()==red && grane.at(i).dajDolazniCvor().dajRedniBroj()==kolona)
            {
                return grane.at(i);
            }
        }
        throw std::domain_error("Grana nije pronadjena");
    }
    Grana<TipOznake> operator *()
    {
        int brojac=0;
        int red=-1;
        int kolona=-1;
        bool da=false;
        for(int i=0;i<matricaVrijednosti.size();i++)
        {
            for(int j=0;j<matricaVrijednosti.at(0).size();j++)
            {
                if(matricaVrijednosti.at(i).at(j) && brojac<=pozicijaMatrica)
                {
                    brojac=brojac+1;
                    red=i;
                    kolona=j;
                }
                else if(brojac>pozicijaMatrica)
                {
                    da=true;
                    break;
                }
            }
            if(da==true)
            {
                break;
            }
        }
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i).dajPolazniCvor().dajRedniBroj()==red && grane.at(i).dajDolazniCvor().dajRedniBroj()==kolona)
            {
                return grane.at(i);
            }
        }
        throw std::domain_error("Grana nije pronadjena");
    }
    GranaIterator(const GranaIterator<TipOznake> &granait)
    {
        pozicijaMatrica=granait.pozicijaMatrica;
        grane=granait.grane;
        polazniCvor=granait.dolazniCvor;
        if(1==1)
        {
            dolazniCvor=granait.dolazniCvor;
        }
    }
    GranaIterator<TipOznake> &operator = (const GranaIterator<TipOznake> &granait)
    {
        pozicijaMatrica=granait.pozicijaMatrica;
        grane=granait.grane;
        polazniCvor=granait.dolazniCvor;
        if(1==1)
        {
            dolazniCvor=granait.dolazniCvor;
        }
    }
    bool operator ==(const GranaIterator &granait) const
    {
        int brojac=0;
        int red=-1;
        int kolona=-1;
        bool da=false;
        for(int i=0;i<matricaVrijednosti.size();i++)
        {
            for(int j=0;j<matricaVrijednosti.at(0).size();j++)
            {
                if(matricaVrijednosti.at(i).at(j) && brojac<=pozicijaMatrica)
                {
                    brojac=brojac+1;
                    red=i;
                    kolona=j;
                }
                else if(brojac>pozicijaMatrica)
                {
                    da=true;
                    break;
                }
            }
            if(da==true)
            {
                break;
            }
        }
        if(matricaVrijednosti.at(red).at(kolona)==granait.matricaVrijednosti.at(red).at(kolona))
        {
            return true;
        }
        return false;
    }
    bool operator !=(const GranaIterator &granait)const
    {
        int brojac=0;
        int red=-1;
        int kolona=-1;
        bool da=false;
        for(int i=0;i<matricaVrijednosti.size();i++)
        {
            for(int j=0;j<matricaVrijednosti.at(0).size();j++)
            {
                if(matricaVrijednosti.at(i).at(j) && brojac<=pozicijaMatrica)
                {
                    brojac=brojac+1;
                    red=i;
                    kolona=j;
                }
                else if(brojac>pozicijaMatrica)
                {
                    da=true;
                    break;
                }
            }
            if(da==true)
            {
                break;
            }
        }
        if(matricaVrijednosti.at(red).at(kolona)!=granait.matricaVrijednosti.at(red).at(kolona))
        {
            return false;
        }
        if(pozicijaMatrica==grane.size())
        {
            return false;
        }
        return true;
    }
    GranaIterator &operator ++()
    {
        int brojac=0;
        int red=-1;
        int kolona=-1;
        bool da=false;
        for(int i=0;i<matricaVrijednosti.size();i++)
        {
            for(int j=0;j<matricaVrijednosti.at(0).size();j++)
            {
                if(matricaVrijednosti.at(i).at(j) && brojac<=pozicijaMatrica)
                {
                    brojac=brojac+1;
                    red=i;
                    kolona=j;
                }
                else if(brojac>pozicijaMatrica)
                {
                    da=true;
                    break;
                }
            }
            if(da==true)
            {
                break;
            }
        }
        if(red==matricaVrijednosti.size()-1 && kolona==matricaVrijednosti.at(0).size()-1)
        {
            throw std::domain_error("Greska");
        }
        pozicijaMatrica=pozicijaMatrica+1;
        return *this;
    }
    GranaIterator &operator ++(int x)
    {
        int brojac=0;
        int red=-1;
        int kolona=-1;
        bool da=false;
        for(int i=0;i<matricaVrijednosti.size();i++)
        {
            for(int j=0;j<matricaVrijednosti.at(0).size();j++)
            {
                if(matricaVrijednosti.at(i).at(j) && brojac<=pozicijaMatrica)
                {
                    brojac=brojac+1;
                    red=i;
                    kolona=j;
                }
                else if(brojac>pozicijaMatrica)
                {
                    da=true;
                    break;
                }
            }
            if(da==true)
            {
                break;
            }
        }
        if(red+x>=matricaVrijednosti.size()-1 && kolona+x>=matricaVrijednosti.at(0).size()-1)
        {
            throw std::domain_error("Greska");
        }
        pozicijaMatrica=pozicijaMatrica+x;
        return *this;
    }
    GranaIterator(std::vector<std::vector<bool>> m, std::vector<Grana<TipOznake>> gr, int polazni, int dolazni, int poz)
    {
        dolazniCvor=dolazni;
        pozicijaMatrica=poz;
        matricaVrijednosti=m;
        polazniCvor=polazni;
        grane=gr;
    }
};

template <typename TipOznake>
class MatricaGraf: public UsmjereniGraf<TipOznake>
{
    std::vector<std::vector<bool>> matricaVrijednosti;
    std::vector<Cvor<TipOznake>> cvorovi;
    std::vector<Grana<TipOznake>> grane;
public:
    MatricaGraf(int broj)
    {
        if(broj<=0)
        {
            throw std::domain_error("Greska");
        }
        matricaVrijednosti.resize(broj);
        for(int i=0;i<broj;i++)
        {
            matricaVrijednosti.at(i).resize(broj);
        }
        for(int i=0;i<broj;i++)
        {
            for(int j=0;j<broj;j++)
            {
                matricaVrijednosti.at(i).at(j)=false;
            }
            cvorovi.push_back(Cvor<TipOznake>(i,*this));
        }
    }
    ~MatricaGraf() {};
    MatricaGraf(const MatricaGraf<TipOznake> &matrica)
    {
        grane=matrica.grane;
        cvorovi=matrica.cvorovi;
        matricaVrijednosti=matrica.matricaVrijednosti;
    }
    MatricaGraf<TipOznake> &operator =(const MatricaGraf<TipOznake> &matrica)
    {
        if(matricaVrijednosti==this->matricaVrijednosti)
        {
            return *this;
        }
        grane=matrica.grane;
        cvorovi=matrica.cvorovi;
        matricaVrijednosti=matrica.matricaVrijednosti;
        return *this;
    }
    int dajBrojCvorova()
    {
        return cvorovi.size();
    }
    void postaviBrojCvorova(int broj)
    {
        if(broj<matricaVrijednosti.size())
        {
            throw std::domain_error("Greska");
        }
        int prijasnja(matricaVrijednosti.size());
        for(int i=prijasnja;i<broj;i++)
        {
            cvorovi.push_back(Cvor<TipOznake>(i,*this));
        }
        if(prijasnja<broj)
        {
            matricaVrijednosti.resize(broj);
            for(int i=0;i<broj;i++)
            {
                matricaVrijednosti.at(i).resize(broj);
            }
            for(int i=0;i<broj;i++)
            {
                for(int j=0;j<broj;j++)
                {
                    if(i>prijasnja || j>prijasnja)
                    {
                        matricaVrijednosti.at(i).at(j)=false;
                    }
                }
            }
        }
    }
    void dodajGranu(int polazni, int dolazni, double tezina=0)
    {
        if(polazni>=cvorovi.size() || dolazni>=cvorovi.size() || polazni<0 || dolazni<0)
        {
            throw std::domain_error("Cvor je izvan opsega");
        }
        if(matricaVrijednosti.at(polazni).at(dolazni)==false)
        {
            matricaVrijednosti.at(polazni).at(dolazni)=true;
            grane.push_back(Grana<TipOznake>(polazni,dolazni,*this,tezina));
        }
        else
        {
            throw std::domain_error("Grana vec postoji");
        }
    }
    void obrisiGranu(int polazni, int dolazni)
    {
        matricaVrijednosti.at(polazni).at(dolazni)=false;
        for(int i=0;i<grane.size();i++)
        {
            if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,0))
            {
                grane.erase(grane.begin()+i);
                return;
            }
        }
    }
    void postaviTezinuGrane(int polazni, int dolazni, double tezina=0)
    {
        if(matricaVrijednosti.at(polazni).at(dolazni)==true)
        {
            for(int i=0;i<grane.size();i++)
            {
                if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,tezina))
                {
                    grane.at(i).postaviTezinu(tezina);
                    return;
                }
            }
        }
        else
        {
            throw std::domain_error("Grana nije pronadjena");
        }
    }
    float dajTezinuGrane(int polazni, int dolazni)
    {
        if(matricaVrijednosti.at(polazni).at(dolazni)==true)
        {
            for(int i=0;i<grane.size();i++)
            {
                if(grane.at(i)==Grana<TipOznake>(polazni,dolazni,*this,0))
                {
                    return grane.at(i).dajTezinu();
                }
            }
        }
        throw std::domain_error("Grana nije pronadjena");
    }
    bool postojiGrana(int polazni,int dolazni)
    {
        return matricaVrijednosti.at(polazni).at(dolazni);
    }
    void postaviOznakuCvora(int broj,TipOznake oznaka)
    {
        if(broj>=cvorovi.size() || broj<0)
        {
            throw std::domain_error("Cvor ne postoji");
        }
        cvorovi.at(broj).postaviOznaku(oznaka);
    }
    TipOznake dajOznakuCvora(int broj)
    {
        if(broj>=cvorovi.size() || broj<0)
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
        throw std::domain_error("Nije pronadjena");
    }
    GranaIterator<TipOznake> dajGranePocetak()
    {
        return GranaIterator<TipOznake>(matricaVrijednosti,grane,grane.at(0).dajPolazniCvor().dajRedniBroj(),grane.at(0).dajDolazniCvor().dajRedniBroj(),0);
    }
    GranaIterator<TipOznake> dajGraneKraj()
    {
        return GranaIterator<TipOznake>(matricaVrijednosti,grane,grane.at(grane.size()-1).dajPolazniCvor().dajRedniBroj(),grane.at(grane.size()-1).dajDolazniCvor().dajRedniBroj(),grane.size()-1);
    }
};

int main()
{
    return 0;
}