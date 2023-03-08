#include <iostream>
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
class AVLStabloMapa: public Mapa<TipKljuca,TipVrijednosti>
{
    Cvor<TipKljuca, TipVrijednosti>* korijen;
    int VelicinaMape;
public:
    AVLStabloMapa()
    {
        korijen=nullptr;
        VelicinaMape=0;
    }
    ~AVLStabloMapa()
    {
        brisi(korijen);
        VelicinaMape=0;
        korijen=nullptr;
    }
    void brisi(Cvor<TipKljuca,TipVrijednosti>* korijen)
    {
        if(korijen)
        {
            brisi(korijen->lijevo);
            brisi(korijen->desno);
            delete korijen;
        }
    }
    void umetniIterativno(Cvor<TipKljuca,TipVrijednosti>* &r, Cvor<TipKljuca,TipVrijednosti>* &z)
    {
        Cvor<TipKljuca,TipVrijednosti>* y=0;
        Cvor<TipKljuca,TipVrijednosti>* x=r;
        while(x!=0)
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
    void Inorder(Cvor<TipKljuca,TipVrijednosti>*korijen1, Cvor<TipKljuca,TipVrijednosti>* &korijen2)
    {
        if(korijen1!=0)
        {
            Inorder(korijen1->lijevo,korijen2);
            Cvor<TipKljuca,TipVrijednosti>* novi=new Cvor<TipKljuca,TipVrijednosti>(korijen1->kljuc,korijen1->vrijednost,0,0,0);
            umetniIterativno(korijen2,novi);
            Inorder(korijen1->desno,korijen2);
        }
    }
    AVLStabloMapa(const AVLStabloMapa<TipKljuca,TipVrijednosti> &mapa)
    {
        korijen=nullptr;
        Inorder(mapa.korijen,korijen);
        VelicinaMape=mapa.VelicinaMape;
    }
    AVLStabloMapa<TipKljuca,TipVrijednosti> &operator =(const AVLStabloMapa<TipKljuca,TipVrijednosti> &mapa)
    {
        if(&mapa==this)
        {
            return *this;
        }
        obrisi();
        Inorder(mapa.korijen,korijen);
        VelicinaMape=mapa.VelicinaMape;
        return *this;
    }
    Cvor<TipKljuca,TipVrijednosti>* trazi(Cvor<TipKljuca,TipVrijednosti>* cvor, const TipKljuca& kljuc) const
    {
        while(cvor!=0 && kljuc!=cvor->kljuc)
        {
            if(kljuc<cvor->kljuc)
            {
                cvor=cvor->lijevo;
            }
            else
            {
                cvor=cvor->desno;
            }
        }
        return cvor;
    }
    Cvor<TipKljuca,TipVrijednosti>* &umetni(Cvor<TipKljuca,TipVrijednosti>* &korijen, const TipKljuca& x)
    {
        if(korijen==nullptr)
        {
            korijen=new Cvor<TipKljuca,TipVrijednosti>(x,TipVrijednosti(),0,0,0);
            return korijen;
        }
        else if(x<korijen->kljuc)
        {
            return umetni(korijen->lijevo,x);
        }
        return umetni(korijen->desno,x);
    }
    TipVrijednosti operator[](const TipKljuca &kljuc) const
    {
        if(VelicinaMape==0)
        {
            return TipVrijednosti();
        }
        auto vr=trazi(korijen,kljuc);
        if(vr==nullptr)
        {
            return TipVrijednosti();
        }
        return vr->vrijednost;
    }
    TipVrijednosti& operator [](const TipKljuca &kljuc)
    {
        if(VelicinaMape==0)
        {
            korijen=new Cvor<TipKljuca,TipVrijednosti>(kljuc,TipVrijednosti(),0,0,0);
            VelicinaMape=VelicinaMape+1;
            return korijen->vrijednost;
        }
        auto vr=trazi(korijen,kljuc);
        if(vr!=nullptr)
        {
            return vr->vrijednost;
        }
        VelicinaMape=VelicinaMape+1;
        auto pok=umetni(korijen,kljuc);
        return pok->vrijednost;
    }
    int brojElemenata() const
    {
        return VelicinaMape;
    }
    void obrisiCvor(Cvor<TipKljuca,TipVrijednosti>* &cvor, const TipKljuca& kljuc)
    {
        Cvor<TipKljuca,TipVrijednosti> *p=cvor;
        Cvor<TipKljuca,TipVrijednosti> *q=0;
        Cvor<TipKljuca,TipVrijednosti> *priv=0;
        Cvor<TipKljuca,TipVrijednosti> *pp=0;
        Cvor<TipKljuca,TipVrijednosti> *rp=0;
        while(p!=0 && kljuc!=p->kljuc)
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
        if(p==0)
        {
            return;
        }
        if(p->lijevo==0)
        {
            rp=p->desno;
        }
        else if(p->desno==0)
        {
            rp=p->lijevo;
        }
        else
        {
            pp=p;
            rp=p->lijevo;
            priv=rp->desno;
            while(priv!=0)
            {
                pp=rp;
                rp=priv;
                priv=rp->desno;
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
        if(q==0)
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
    void obrisi()
    {
        brisi(korijen);
        VelicinaMape=0;
        korijen=nullptr;
    }
    void obrisi(const TipKljuca& kljuc)
    {
        obrisiCvor(korijen,kljuc);
        VelicinaMape=VelicinaMape-1;
    }
};

int main ()
{
    return 0;
}