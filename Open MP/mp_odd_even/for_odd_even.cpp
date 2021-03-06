#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <chrono>

using namespace std;

///Two for directives

int main(int argc, char* argv[])
{
	int thread_count = strtol(argv[1], NULL, 10);
	int i, phase, n, tmp;
	cout<<"Enter size of the array: ";
	cin>>n;
	int a[n];
	for(i=0; i<n; i++)
	{
		a[i] = n - i;
	}

	//Tiempo de inicio
	auto start = std::chrono::high_resolution_clock::now();


	#pragma omp parallel num_threads(thread_count) \
		default(none) shared(a, n) private(i, tmp, phase)	
	for (phase = 0; phase < n; phase++)
	{
		if (phase % 2 == 0)
		{
			#pragma omp for
			for (i = 1; i < n; i += 2)
			{
				if (a[i-1] > a[i])
				{
					tmp = a[i-1];
					a[i-1] = a[i];
					a[i] = tmp;
				}
			}
		}
		else
		{
			#pragma omp for
			for (i = 1; i < n-1; i += 2)
			{
				if (a[i] > a[i+1])
				{
					tmp = a[i+1];
					a[i+1] = a[i];
					a[i] = tmp;
				}
			}
		}
	}


	//Tiempo de fin
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	cout << "Tiempo de ejecucion: " << elapsed.count() << " segundos" << endl << endl;
	return 0;
}
