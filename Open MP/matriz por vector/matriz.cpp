#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;


int main(int argc, char* argv[])
{
   int thread_count = strtol(argv[1], NULL, 10);
   long i, j;

   int m = 80000; //matrix m X n
   int n = 8;

   int A[m][n]; //matrix
   int x[n]; //vector
   int y[m]; //Ax

    //Iniciando A y x
    for(i=0; i<m; i++)
    {
        for(j=0; j<n; j++)
        {
            A[i][j] = i + j;
            x[j] = i - j;
        }
    }

	//Tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

   //Multiplicacion
   #pragma omp parallel for num_threads(thread_count) \
        default(none) private(i, j) shared(A, x, y, m, n)
   for (i = 0; i < m; i++)
   {
		y[i] = 0.0;
		for (j = 0; j < n; j++)
		{
			y[i] += A[i][j]*x[j];
		}
	}

	//Tiempo de fin
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    cout << "Tiempo de ejecucion: " << elapsed.count() << " segundos" << endl << endl;

    return 0;
}
