#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[])
{
	int thread_count = strtol(argv[1], NULL, 10);
	double pi_approx;
	int n;
	int k;
	double factor = 1.0;
	double sum = 0.0;
	cout<<"Enter n: ";
	cin>>n;

	#pragma omp parallel for num_threads(thread_count) \
		reduction(+:sum) private(factor)
	for(k=0; k < n; k++)
	{
		if(k%2==0)
		{
			factor = 1.0;
		}
		else
		{
			factor = -1.0;
		}
		sum += factor/(2*k+1);
	}
	pi_approx = 4.0*sum;
	//cout<<pi_approx<<endl;
	cout<<"Pi calculado: "<<  fixed << setprecision(10) << pi_approx <<endl;
}
