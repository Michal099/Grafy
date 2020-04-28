#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

struct list
{
    int s,w;    // s - sasiadzi, w - waga
    list* nastepny;
};

void UtworzDane(fstream& strumien);
void WyswietlLista(list** tab_list, int w);
void WyswietlMacierz(int** mac, int w);
void DrogaLista(int w, int poczatek, list** tab_list);  //algorytm Bellmana-Forda
void DrogaMacierz(int** mac, int w, int poczatek);      //algorytm Bellmana-Forda
void GrafList(int& w, int& kr, int** graf, list** tab_list, list* lista);
void** GrafMac(int& w, int& kr, int** graf, int** graf_m);
void** Wczytaj(int** graf,int& kr, ifstream& wejscie);
void SzukajDrogiMac(int** graf, int& w, int& kr, int& poczatek, fstream& wyjscie);
void SzukajDrogiMac(int** graf, int& w, int& kr, int& poczatek, fstream& wyjscie);
void ZnajdzDroge(ifstream& wejscie);

int main()
{
    int wybor;
    int STOP=0;

    fstream dane;
    ifstream wejscie;
    srand(time(NULL)); //czas na zero

    do
    {
        cout << "1. wygeneruj dane do pliku "<<endl;
        cout << "2. pomiar "<<endl;
        cout << "3. koniec "<<endl;
        cin >> wybor;

     switch( wybor )
     {
        case 1:
        UtworzDane(dane);
        break;
        case 2:
        wejscie.open("dane.txt", ios_base::in); 
        ZnajdzDroge(wejscie);
        wejscie.close();  
        break;
        case 3: 
        STOP=1;
        break;
     }
    } while(STOP!=1);
return 0;


}

void UtworzDane(fstream& strumien)
{
    int wierzcholki[5] = {10, 50, 100, 250, 500};  
    int krawedzie[5][4];                            //ilość krawedzi
    int max_kra[5]; 
    int wypelnienie[4] = {25, 50, 75, 100};         // % wypelnienia

    strumien.open("dane.txt", ios_base::out);       //utworzenie pliku/czyszczenie pliku z ewentualnych wartośći
    strumien.close();                               

    for (int i = 0; i < 5; i++) 
    {
        max_kra [i] = wierzcholki[i] * (wierzcholki[i] - 1) /2;
        for (int j = 0; j < 4; j++)
        {
            krawedzie[i][j] = wypelnienie[j] * max_kra[i] /100;
            for(int k = 0; k < 100; k++) 
            {
                int reszta = krawedzie[i][j] - wierzcholki[i] + 1;  //krawedzie bez wierzchołków
                int cos = 0;
                int wierzcholek = wierzcholki[i];
                int licznik = 0;

                strumien.open("dane.txt", ios_base::app);  

                strumien << wierzcholki[i]<<" "<<krawedzie[i][j]<<" "<<rand()%wierzcholki[i]<<endl;

                for(int z = 0; z < wierzcholki[i] - 1 && z < krawedzie[i][j] -1 ; z++)

                {
                    strumien << z<< " "<<z+1<<" "<<rand()%100+1<<endl;
                }


                while(licznik < reszta) 
                {
                    cos = 0;

                    while(licznik < reszta && cos < wierzcholek - 2)
                    {
                        strumien << wierzcholek - 1<<" "<< cos <<" "<<rand()%10+1<<endl;
                        licznik++;
                        cos++;
                    }
                    wierzcholek--;
                }
                strumien.close();
            }
        }
    }

}

void WyswietlLista(list** tab_list, int w)
{
    list* list_w;
    cout<< "lista:\n";
    for(int i = 0; i < w; i++)
      {
        cout << "tab[" << i << "] =";
        list_w = tab_list[i];
        while(list_w)
        {
          cout << list_w->s <<" ";
          list_w = list_w->nastepny;
        }
        cout << endl;
    }
    delete []list_w;
}

void WyswietlMacierz(int** mac, int w)
{

    for(int i = 0; i < w; i++)
    {
        for(int j = 0; j < w; j++)
        {
            cout << mac[i][j] <<" ";
        }
        cout<<endl;
    }
}

void DrogaLista(int w, int poczatek, list** tab_list) //algorytm Bellmana-Forda
{
    list* sasiedzi;

    int max_int =2147483647;            //max w 32bit

    int* koszt_dojscia;                 ///wskaznik na tablice dynamiczna kosztow dojscia
    koszt_dojscia = new int [w];

    int* poprzednik;                    ///wskaznik na tablice dynamiczna poprzednikow
    poprzednik = new int [w];

    for(int i = 0; i < w; i++)
    {
        poprzednik[i] = -1;             
        koszt_dojscia[i] = max_int;     
    }

    koszt_dojscia[poczatek] = 0;     
    for(int j = 1; j < w; j++)
    {
        for(int k = 0; k < w; k++)    //szukamy krawędzi
        {
            for(sasiedzi = tab_list[k]; sasiedzi; sasiedzi = sasiedzi -> nastepny)
            {
                if(koszt_dojscia[k] != max_int && koszt_dojscia[sasiedzi -> s] > koszt_dojscia[k] + sasiedzi -> w)
                {
                    koszt_dojscia[sasiedzi -> s] = koszt_dojscia [k] + sasiedzi -> w;
                    poprzednik[sasiedzi -> s] = k;
                }
            }
        }
    }
    
}

void DrogaMacierz(int** mac, int w, int poczatek)   //algorytm Bellmana-Forda
{

    int max_int =2147483647;            //max w 32bit

    int* koszt_dojscia;                 ///wskaznik na tablice dynamiczna kosztow dojscia
    koszt_dojscia = new int [w];

    int* poprzednik;                    ///wskaznik na tablice dynamiczna poprzednikow
    poprzednik = new int [w];

    for(int i = 0; i < w; i++)
    {
        poprzednik[i] = -1;             
        koszt_dojscia[i] = max_int;     
    }

    koszt_dojscia[poczatek] = 0;           
    for(int j = 1; j < w; j++)
    {
        for(int k = 0; k < w; k++)
        {
            for(int v = 0; v < w; v++)    //szukamy krawędzi
            {
                if(koszt_dojscia[k] != max_int && mac[v][k] != 0 && koszt_dojscia[v] > koszt_dojscia[k] + mac[v][k])
                {
                    koszt_dojscia[v] = koszt_dojscia [k] +  mac[v][k];
                    poprzednik[v] = k;
                }
            }
        }
    }
}

void GrafList(int& w, int& kr, int** graf, list** tab_list, list* lista)
{
    int waga, s1, s2;  // waga, 1-sąsiad, 2-sąsiad

    for(int i = 0; i < w; i++)   // tablic NULL'ów
    {
        tab_list[i] = NULL;

    }

    for(int j = 0; j < kr; j++)   //listy dla grafu nieskierowanego
    {
        s2 = graf[j][0];     //wczytujemy dane z pliku
        s1 = graf[j][1];
        waga = graf[j][2];

        lista = new list;

        lista->s = s2;
        lista->w = waga;
        lista->nastepny = tab_list[s1];
        tab_list[s1] = lista;
    }                           

    for(int j = 0; j < kr; j++)   //listy dla grafu skierowanego
    {
        s1 = graf[j][0];     //wczytujemy dane z pliku
        s2 = graf[j][1];
        waga = graf[j][2];

        lista = new list;

        lista->s = s2;
        lista->w = waga;
        lista->nastepny = tab_list[s1];
        tab_list[s1] = lista;

    }
    
}

void** GrafMac(int& w, int& kr, int** graf, int** graf_m)
{
    int waga, s1, s2;   // waga, 1-sąsiad, 2-sąsiad
    int k = 0;

    for (int i = 0; i < w; ++i)   // 0 = brak krawedzi
    {
        for(int j = k; j < w; ++j)
        {
            graf_m[i][j] = 0;
            graf_m[j][i] = 0;
        }
        k++;
    }

    for (int i = 0; i < kr; ++i)    //dla grafu nieskierowaniego
    {
        s1 = graf[i][0];        //wczytujemy dane z pliku
        s2 = graf[i][1];
        waga = graf[i][2];

        graf_m[s2][s1] = waga;  
    }

     for (int i = 0; i < kr; ++i)    //dla grafu skierowaniego
    {
        s1 = graf[i][0];        //wczytujemy dane z pliku
        s2 = graf[i][1];
        waga = graf[i][2];

        graf_m[s1][s2] = waga;  
    }
}

void** Wczytaj(int** graf,int& kr, ifstream& wejscie)
{
    for(int i = 0; i < kr; i++)    ///wczytujemy graf z pliku do tablicy dynamicznej 2-wymiarowej
    {
        wejscie >> graf[i][0];
        wejscie >> graf[i][1];
        wejscie >> graf[i][2];
    }
}

void SzukajDrogiList(int** graf, int& w, int& kr, int& poczatek, fstream& wyjscie)
{
    double czas;
    clock_t czas1, czas2;         

    list** tab_list;
    list* lista, *usun;      

    tab_list = new list * [w]; //alokujemy pamiec dla tablicy

    GrafList(w, kr, graf, tab_list, lista);    

    czas1 = clock();

    DrogaLista(w, poczatek, tab_list);   

    czas2 = clock() - czas1;   


      for(int i = 0; i < w; i++) //usuwanie list
      {
        lista = tab_list[i];
        while(lista)
        {
          usun = lista;
          lista = lista->nastepny;
          delete usun;
        }
      }
        delete [] tab_list;   //usuwamy tablice list

    czas = (double) (czas2) / CLOCKS_PER_SEC; 

    wyjscie << "Czas trwania dla listy przy: "<<w<<" i: "<<kr*100/(w * (w - 1) /2)<<"% wypelnienia: "<<czas;
}

void SzukajDrogiMac(int** graf, int& w, int& kr, int& poczatek, fstream& wyjscie)
{
    double czas;
    clock_t czas1, czas2;    

    int** graf_m;                     //tworzymy macierz i nastepnie alokujemy dla niej pamiec

    graf_m = new int *[w];
    for(int i = 0; i < w; ++i)
    {
        graf_m[i] = new int [w];
    }

    GrafMac(w, kr, graf, graf_m); 

    czas1 = clock();                  

    DrogaMacierz(graf_m, w, poczatek); 

    czas2 = clock() - czas1;     

    for(int i = 0; i < w; ++i) //usuwanie macierzy
    {
        delete [] graf_m[i];
    }
    delete [] graf_m;

    czas = (double) (czas2) / CLOCKS_PER_SEC;       

    wyjscie << " Czas dla macierzy: "<<czas<<endl;
}

void ZnajdzDroge(ifstream& wejscie)
{
    fstream wyjscie;
    int w, kr, poczatek;
    int** graf;
     
    wyjscie.open("pomiar.txt", ios_base::app);  ///otwarcie strumienia wyjscioweg 

    for(int i = 0; i < 2000; i++)
    {
        wejscie >> w;            
        wejscie >> kr;
        wejscie >> poczatek;

        graf = new int* [kr];    //alokowanie pamieci

        for(int j = 0; j < kr; j++)
        {
            graf[j] = new int [3];
        }

        Wczytaj(graf, kr, wejscie);      
        SzukajDrogiList(graf, w, kr, poczatek, wyjscie);
        SzukajDrogiMac(graf, w, kr, poczatek, wyjscie);  

        for(int i = 0; i < kr; ++i) //usuwanie tablic
        {
            delete [] graf[i];
        }
        delete [] graf;
    }
    wyjscie.close();    //zamknięcie strumienia wyjsciowego
}




# Grafy
