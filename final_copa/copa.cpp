#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <math>

using namespace std;

//cantidad de elementos
int n = 10000;

//capacidad de la bolsa
int c = 10000;

//Array con los pesos de los objetos
int pesos[n];
for(int i=0; i<n; i++)
{
	pesos[i] = i*2;
}


//Array con los valores de los objetos
int valores[n];
for(int i=0; i<n; i++)
{
	valores[i] = i*2;
}



///1. Generacion

class Conjunto
{
public:
	string elementos; //cadena de 0's(no esta) y 1's(no esta)
	int profit;


	Triple(string e, int w, int p)
	{
		elementos = e;
		profit = p;
	}
};



//Auxiliares para los mapas
class compara
{
	bool is_A;
	compara(bool param=true)
	{
		is_A = param;
	}
	bool operator() (int left, int right)
	{
		if(is_A)
		{
			return (left < right);
		}
		else
		{
			return (left > right);
		}

	}
};

typedef multimap<int, Conjunto>::iterator iterador;

///MAPAS (seudo colas de prioridad)
//Lista de tripletas: primera mitad
multimap<int, Conjunto, comparar> lista_A (compara(true));

//Lista de tripletas: primera mitad
multimap<int, Conjunto, comparar> lista_B (compara(false));


//calcula el peso total de un conjunto de elementos
int peso_total(string conjunto)
{
	int resultado = 0;
	for(int i=0; i<n; i++)
	{
		if(conjunto[i]=="1")
		{
			resultado += pesos[i];
		}
	}
	return resultado;
}

//calcula el valor total de un conjunto de elementos
int valor_total(string conjunto)
{
	int resultado = 0;
	for(int i=0; i<n; i++)
	{
		if(conjunto[i]=="1")
		{
			resultado += valores[i];
		}
	}
	return resultado;
}


//FUNCIONES DE COMBINACION
void combinar_step_A(string prev, int current, int limite, int extra)
{
    current++;
    if(current <= limite)
    {
       combinar_step_A(prev+"0", current, limite, extra);
       combinar_step_A(prev+"1", current, limite, extra);
    }
    else
    {
        for(int i=0; i<extra; i++)
        {
            prev += "0";
        }
		lista_A.insert( pair<int,Conjunto>(peso_total(prev),Conjunto(prev,valor_total(prev))) ); //Insertamos la tripleta en A
    }
}

void combinar_step_B(string prev, int current, int limite)
{
    current++;
    if(current <= limite)
    {
       combinar_step_B(prev+"0", current, limite);
       combinar_step_B(prev+"1", current, limite);
    }
    else
    {
        lista_B.insert( pair<int,Conjunto>(peso_total(prev),Conjunto(prev,valor_total(prev))) );; //Insertamos la tripleta en B
    }
}

//primera mitad
void combinar_A(int n)
{
    int my_n = n/2;
    int extra = n - my_n;

    combinar_step_A("0", 1, my_n, extra);
    combinar_step_A("1", 1, my_n, extra);
}

//segunda mitad
void combinar_B(int n)
{
    int my_n = n - (n/2);
    string prev = "";
    for(int i=0; i<n/2; i++)
    {
        prev += "0";
    }
    combinar_step_B(prev+"0", 1, my_n);
    combinar_step_B(prev+"1", 1, my_n);
}




//Algorithm 3
///2. Escenario de guardado de maximo valor
pair<int,Conjunto> local_max_A()
{
	iterador it;
    int my_rank = omp_get_thread_num();
    int thread_count_k = omp_get_num_threads();
	int e = lista_A.size()/k;
	int local_ini = my_rank * e;
	int local_fin = local_a + e;
	pair<int,Conjunto> max;
	max.first = (lista_A.begin()+local_ini)->first;
	max.second = lista_A.begin()+local_ini)->second;
    for (it = lista_A.begin()+local_ini; i!=lista_A.begin()+local_fin; ++it)
    {
        if((*it).second.profit>max)
		{
			max.first = (*it).first;
			max.second = (*it).second;
		}
    }
    return max;
}

pair<int,Conjunto> local_max_B()
{
	iterador it;
    int my_rank = omp_get_thread_num();
    int thread_count_k = omp_get_num_threads();
	int e = lista_B.size()/k;
	int local_ini = my_rank * e;
	int local_fin = local_a + e;
	pair<int,Conjunto> max;
	max.first = (lista_B.begin()+local_ini)->first;
	max.second = lista_B.begin()+local_ini)->second;
    for (it = lista_B.begin()+local_ini; i!=lista_B.begin()+local_fin; ++it)
    {
        if((*it).second.profit>max)
		{
			max.first = (*it).first;
			max.second = (*it).second;
		}
    }
    return max;
}



int main(int argc, char* argv[])
{
	double global_result = 0.0;
	int thread_count;

	vector<pair<int,Conjunto>> maxA;
	vector<pair<int,Conjunto>> maxB;

	thread_count = strtol(argv[1], NULL, 10);
# pragma omp parallel num_threads(thread_count)
	{
		Conjunto m_a_i = local_max_A();
		Conjunto m_b_i = local_max_B();
#pragma omp critical
		maxA.push_back(m_a_i);
		maxB.push_back(m_b_i);

	}

	return 0;
}
