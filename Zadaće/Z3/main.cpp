#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int DajMax(std::vector<int> Vektor)
{
    int Max=Vektor.at(0);
    for (int i=0;i<Vektor.size();i++)
    {
        if (Vektor.at(i)>Max)
        {
            Max=Vektor.at(i);
        }
    }
    return Max;
}

void countSort(std::vector<int> &a, int &exp)
{
        std::vector<int> count(10,0);
        std::vector<int> Vracaj(a.size());
        for(int i=0;i<a.size();i++)
        {
            count.at((a.at(i)/exp)%10)++;
        }
        for(int i=1;i<10;i++)
        {
            count.at(i)+=count.at(i-1);
        }
        for(int i=a.size()-1;i>=0;i--)
        {
            Vracaj.at(count.at((a.at(i)/exp)%10)-1)=a.at(i);
            count.at((a.at(i)/exp)%10)--;
        }
        for(int i=0;i<a.size();i++)
        {
            a.at(i)=Vracaj.at(i);
        }
}

void radixSort(vector<int> &a)
{
    int Max=DajMax(a);
    for (int exp=1;Max/exp>0;exp=exp*10)
    {
        countSort(a,exp);
    }
}

void stvoriGomilu(std::vector<int> &a)
{
    int velicina=a.size();
    for (int i=velicina/2;i>=0;i--)
    {
        while (!((i>=velicina/2)&&(i<velicina)))
        {
            int veci=2*i+1;
            int desni=2*i+2;
            if ((desni<velicina) && (a.at(desni)>a.at(veci)))
            {
                veci=desni;
            }
            if (a.at(i)>a.at(veci))
            {
                break;
            }
            int pomocni=a.at(i);
            a.at(i)=a.at(veci);
            a.at(veci)=pomocni;
            i=veci;
        }
    }
}

void umetniUGomilu(std::vector<int> &a, int umetnuti, int &velicina)
{
    if (a.size()==velicina)
    {
        a.resize(a.size()+1);
    }
    a.at(velicina++)=umetnuti;
    int i=velicina-1;
    while ((i!=0) && (a.at(i)>a.at((i-1)/2)))
    {
        int j=(i-1)/2;
        int pomocni=a.at(i);
        a.at(i)=a.at(j);
        a.at(j)=pomocni;
        i=j;
    }
}

int izbaciPrvi(vector<int> &a, int &velicina)
{
    if (velicina==0)
    {
        throw std::domain_error("Gomila je prazna");
    }
    int j=velicina-1;
    int pomocni=a.at(0);
    a.at(0)=a.at(j);
    a.at(j)=pomocni;
    velicina=velicina-1;
    if (velicina!=0)
    {
        int i=0;
        while (!((i>=velicina/2)&&(i<velicina)))
        {
            int veci=2*i+1;
            int desni=2*i+2;
            if ((desni<velicina) && (a.at(desni)>a.at(veci)))
            {
                veci=desni;
            }
            if (a.at(i)>a.at(veci))
            {
                break;
            }
            int pomocni=a.at(i);
            a.at(i)=a.at(veci);
            a.at(veci)=pomocni;
            i=veci;
        }
    }
    return a.at(velicina);
}

void gomilaSort(std::vector<int> &a)
{
    stvoriGomilu(a);
    int velicina = a.size();
    for (int i = a.size()-1;i>=1;i--)
    {
        int pomocni = a.at(0);
        a.at(0) = a.at(i);
        a.at(i) = pomocni;
        velicina=velicina-1;
        auto pamti=i;
        i=0;
        while (!((i>=velicina/2)&&(i<velicina)))
        {
            int veci=2*i+1;
            int desni=2*i+2;
            if ((desni<velicina) && (a.at(desni)>a.at(veci)))
            {
                veci=desni;
            }
            if (a.at(i)>a.at(veci))
            {
                break;
            }
            int pomocni=a.at(i);
            a.at(i)=a.at(veci);
            a.at(veci)=pomocni;
            i=veci;
        }
        i=pamti;
    }
}
int main()
{
    return 0;
}