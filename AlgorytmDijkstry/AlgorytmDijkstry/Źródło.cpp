/********************************************************************************
*                                                                           *
*   ALGORYTM DIJKSTRY wyznaczania najkrotszej sciezki od            *
*   danego wierzcholka do wszystkich pozostalych w grafie skierowanym   *
*   o nieujemnych wagach                                                    *
*                  algorytm.org                                                 *
*                                                                               *
*                                                                               *
*                                                                               *
*********************************************************************************/

/*Uwaga:
Graf znajduje sie w pliku graf.txt


W ponizszej implementacji posluzono sie biblioteka  STL, ktora
zawiera szablon wektora oraz algorytm sortowania kolejki.
Jezeli tw¥o kompilator nie posiada tej biblioteki mozesz zastapic
wektor tablica i algorytmem QuickSort (ze strony).

*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <stack>

//!Wartosc, od ktorej przyjmujemy nieskonczonosc
#define nieskonczonosc 1000

using namespace std;

/*!Wektor odleglosci od pierwszego wierzcholka do pozostalych,
poczatkowo zawiera pierwszy wiersz macierzy A
*/
int     D[100];

class   cGraf {

public:

	/*!Macierz przyleglosci, w indeksie (i,j) zawiera wartosci:
	0           dla i=j
	nieskonczonosc      jezeli nie istnieje krawedz (i,j)
	K>0          waga krawedzi (i,j)
	*/
	int     A[100][100];



	//!Liczba wierzcholkow grafu
	int n;

	//!Metoda wyznacza lista nastepnikow wierzcholka x
	vector<int> Nastepniki(int x) {
		vector<int> wynik;

		for (int i = 0; i<n; i++)
			if ((A[x][i] != nieskonczonosc) && (A[x][i] != 0))
				wynik.push_back(i);

		return(wynik);
	}

};

class cWierzcholek {

public:

	//!Wierzcholek
	int V;

	//!Odleglosc wierzcholka pierwszego do wierzcholka V
	int* Odl;     //bedzie wskazywac na odpowiednia komorke D[i]

				  //!Przeciazamy operator mniejszosci dla klasy cWierzcholek
	friend bool operator < (const cWierzcholek &p1, const cWierzcholek &p2) {
		return(*(p1.Odl)>*(p2.Odl));
	}

};


void main(void)
{
	FILE                *plik;
	char                s[5];
	int                 i, j, k;
	cGraf               Graf;

	/*!Kolejka priorytetowa wierzcholkow, uporzadkowana wg klucza,
	ktorym jest atrybut Odl klasy cWierzcholek
	*/
	vector<cWierzcholek>      Q;
	cWierzcholek            wierzcholek;

	//!Lista nastepnikow
	vector<int>           nastepniki;
	//--------------------------------------------

	if ((plik = fopen("graf.txt", "r")) == NULL)
		printf("Brak pliku graf.txt!\n"); else
	{
		//!Wczytujemy liczbe wierzcholkow grafu
		fscanf(plik, "%d", &Graf.n);

		//!Wczytujemy dane do macierzy przylegsosci
		for (j = 0; j<Graf.n; j++)
			for (i = 0; i<Graf.n; i++)
			{
				fscanf(plik, "%s", s);
				if (strcmp(s, "*") != 0)
					Graf.A[j][i] = atoi(s); else Graf.A[j][i] = nieskonczonosc;
			}
		fclose(plik);


		//!przyjmujemy, ze wyznaczamy odleglosci od pierwszego wierzcholka w macierzy A


		for (i = 0; i<Graf.n; i++) {

			//!Poczatkowo wektor D zawiera pierwszy wiersz macierzy A
			D[i] = Graf.A[0][i];

			//!Dodaj dane z macierzy do kolejki Q
			wierzcholek.V = i;
			wierzcholek.Odl = &D[i];
			Q.push_back(wierzcholek);
		}

		vector<cWierzcholek>::iterator it;
		//tworzymy kopiec
		make_heap(Q.begin(), Q.end(), less<cWierzcholek>());


		/*!Usun pierwszy wierzcholek (odleglosc od pierwszego wierzcholka do pierwszego
		wierzcholka wynosi 0)
		*/
		pop_heap(Q.begin(), Q.end());
		Q.pop_back();

		while (Q.empty() != true) {
			//przywrocenie wlasnosci kopca
			make_heap(Q.begin(), Q.end(), less<cWierzcholek>());

			//!Pobierz pierwszy wierzcholek z kolejki Q (ma on najmniejsza wartosc klucza)
			wierzcholek = Q[0];

			//!Usun go z kolejki
			pop_heap(Q.begin(), Q.end());
			Q.pop_back();

			//!Wyznacz liste jego nastepnikow
			nastepniki = Graf.Nastepniki(wierzcholek.V);

			//!Dokonaj relaksacji odleglosci od wierzcholka pierwszego do kazdego nastepnika z tej listy
			for (i = 0; i<nastepniki.size(); i++)
				D[nastepniki[i]] = min(D[nastepniki[i]], D[wierzcholek.V] + Graf.A[wierzcholek.V][nastepniki[i]]);
		}


		//!Wypisz wektor D zastepujac wartosc stalej nieskonczonosc znakiem *
		k = nieskonczonosc;
		for (i = 0; i<Graf.n; i++)
			if (D[i]<k)
				printf("D(%i)=%d\n", i, D[i]);
			else
				printf("D(%i)=%s\n", i, "*");
	}



	printf("\nDowolny klawisz...\n");
	_getch();
	return;
}