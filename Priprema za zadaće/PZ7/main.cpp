#include <bits/types/clockid_t.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <limits>
using namespace std;

template<typename Tip>
void bubble_sort(Tip* niz, int vel)
{
    for(int i=0;i<vel-1;i++)
    {
        for(int j=0;j<vel-1-i;j++)
        {
            if(niz[j]>niz[j+1])
            {
                auto priv=niz[j];
                niz[j]=niz[j+1];
                niz[j+1]=priv;
            }
        }
    }
}

template<typename Tip>
void selection_sort(Tip* niz, int vel)
{
    int minindex;
    for(int i=0;i<vel-1;i++)
    {
        minindex=i;
        for(int j=i+1;j<vel;j++)
        {
            if(niz[j]<niz[minindex])
            {
                minindex=j;
            }
        }
        if(minindex!=i)
        {
            auto priv=niz[minindex];
            niz[minindex]=niz[i];
            niz[i]=priv;
        }
    }
}

template<typename Tip>
int partition(Tip* niz, int prvi, int zadnji)
{
    auto pivot=niz[zadnji];
    int i=(prvi-1);
    for(int j=prvi;j<=zadnji-1;j++)
    {
        if(niz[j]<pivot)
        {
            i++;
            auto priv=niz[i];
            niz[i]=niz[j];
            niz[j]=priv;
        }
    }
    auto priv=niz[i+1];
    niz[i+1]=niz[zadnji];
    niz[zadnji]=priv;
    return(i+1);
}

template<typename Tip>
void quickpomocna(Tip* niz, int prvi, int zadnji)
{
    if(prvi<zadnji)
    {
        int partitionindex=partition(niz,prvi,zadnji);
        quickpomocna(niz,prvi,partitionindex-1);
        quickpomocna(niz,partitionindex+1,zadnji);
    }
}

template<typename Tip>
void quick_sort(Tip* niz, int vel)
{
    quickpomocna(niz,0,vel-1);
}

template<typename Tip>
void merge(Tip* niz, int lijevi, int srednji, int desni)
{
    auto podniz1=srednji-lijevi+1;
    auto podniz2=desni-srednji;
    auto *lijeviniz=new Tip[podniz1];
    auto *desniniz=new Tip[podniz2];
    for(int i=0;i<podniz1;i++)
    {
        lijeviniz[i]=niz[lijevi+i];
    }
        for(int j=0;j<podniz2;j++)
    {
        desniniz[j]=niz[srednji+j+1];
    }
    auto indexpodniza1=0;
    auto indexpodniza2=0;
    int indexmergeniza=lijevi;
    while(indexpodniza1<podniz1 && indexpodniza2<podniz2)
    {
        if(lijeviniz[indexpodniza1]<=desniniz[indexpodniza2])
        {
            niz[indexmergeniza]=lijeviniz[indexpodniza1];
            indexpodniza1=indexpodniza1+1;
        }
        else
        {
            niz[indexmergeniza]=desniniz[indexpodniza2];
            indexpodniza2=indexpodniza2+1;
        }
        indexmergeniza=indexmergeniza+1;
    }
    while(indexpodniza1<podniz1)
    {
        niz[indexmergeniza]=lijeviniz[indexpodniza1];
        indexpodniza1=indexpodniza1+1;
        indexmergeniza=indexmergeniza+1;
    }
    while(indexpodniza2<podniz2)
    {
        niz[indexmergeniza]=desniniz[indexpodniza2];
        indexpodniza2=indexpodniza2+1;
        indexmergeniza=indexmergeniza+1;
    }
    delete[] desniniz;
    delete[] lijeviniz;
}

template<typename Tip>
void mergepomocna(Tip* niz, int pocetak, int kraj)
{
    if(pocetak>=kraj)
    {
        return;
    }
    auto sredina=pocetak+(kraj-pocetak)/2;
    mergepomocna(niz,pocetak,sredina);
    mergepomocna(niz,sredina+1,kraj);
    merge(niz,pocetak,sredina,kraj);
}

template<typename Tip>
void merge_sort(Tip* niz, int vel)
{
    mergepomocna(niz,0,vel-1);
}

void ucitaj(std::string filename, int *& niz, int &vel)
{
    int brojac=0;
    std::ifstream ulazni_tok(filename);
    if(!ulazni_tok)
    {
        std::cout <<"Datoteka ne postoji";
    }
    else
    {
        while(!ulazni_tok==false)
        {
            if(!ulazni_tok==true || ulazni_tok.eof()==true)
            {
                break;
            }
            int broj;
            ulazni_tok >> broj;
            brojac=brojac+1;
        }
        ulazni_tok.clear();
        ulazni_tok.close();
        std::ifstream ulazni_tok(filename);
        niz = new int[brojac];
        for(int i=0;i<brojac;i++)
        {
            if(!ulazni_tok==true || ulazni_tok.eof()==true)
            {
                break;
            }
            int broj;
            ulazni_tok >> broj;
            if(!ulazni_tok==false)
            {
                niz[i]=broj;
            }
        }
        vel=brojac;
    }
}

void generisi(std::string filename, int vel)
{
    std::ofstream izlazni_tok(filename);
    for(int i=0;i<vel;i++)
    {
        int broj=std::rand()%1000;
        izlazni_tok << broj;
        if(i!=vel-1)
        {
            izlazni_tok << " ";
        }
    }
}

void Funkcija(std::string filename)
{
    int vel=0;
    int *niz=nullptr;
    ucitaj(filename,niz,vel);
    int x;
    clock_t prije;
    clock_t poslije;
    while(x!=1 && x!=2 && x!=3 && x!=4)
    {
        std::cout << "Izbor algoritma pretrage:\nUnesite \"1\" za \"Bubble Sort\"\nUnesite \"2\" za \"Selection Sort\"\nUnesite \"3\" za \"Quick Sort\"\nUnesite \"4\" za \"Merge Sort\"\n";
        std::cin >> x;
        prije=clock();
        if(x==1)
        {
            std::cout << "Odabrali ste \"Bubble Sort\"";
            bubble_sort(niz,vel);
        }
        if(x==2)
        {
            std::cout << "Odabrali ste \"Selection Sort\"";
            selection_sort(niz,vel);
        }
        if(x==3)
        {
            std::cout << "Odabrali ste \"Quick Sort\"";
            quick_sort(niz,vel);
        }
        if(x==4)
        {
            std::cout << "Odabrali ste \"Merge Sort\"";
            merge_sort(niz,vel);
        }
        std::cout << "\n";
        poslije=clock();
        cin.seekg(0,ios::end);
        cin.clear();
        if(x!=1 && x!=2 && x!=3 && x!=4)
        {
            std::cout << "Greska! Ponovite unos!\n";
        }
    }
    bool DaLiJeSortiran=true;
    for(int i=0;i<vel-1;i++)
    {
        if(niz[i]>niz[i+1])
        {
            DaLiJeSortiran=false;
        }
    }
    if(DaLiJeSortiran==false)
    {
        std::cout << "Nije sortiran\n";
    }
    else
    {
        std::cout << "Sortiran je\n";
    }
    int vrijeme=poslije-prije;
    std::cout<< "Sortiranje je trajalo " << vrijeme << "ms" << "\n";
    std::ofstream izlazni_tok("izlaz.txt");
    for(int i=0;i<vel;i++)
    {
        izlazni_tok << niz[i];
        if(i<vel-1)
        {
            izlazni_tok << " ";
        }
    }
}

int main()
{
    std::srand(std::time(0));
    std::cout << "Unesite koliko brojeva zelite: ";
    int broj;
    std::cin >> broj;
    generisi("ulaz.txt",broj);
    auto niz=new int[broj];
    ucitaj("ulaz.txt",niz,broj);
    std::cout << "Prije sortiranja: \n";
    for(int i=0;i<broj;i++)
    {
        std::cout << niz[i] << " ";
    }
    std::cout << "\n";
    Funkcija("ulaz.txt");
    ucitaj("izlaz.txt",niz,broj);
    std::cout << "Poslije sortiranja: \n";
    for(int i=0;i<broj;i++)
    {
        std::cout << niz[i] << " ";
    }
    delete[] niz;
    return 0;
}