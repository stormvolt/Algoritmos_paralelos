#include <iostream>
#include <chrono>

using namespace std;

const int MAX = 1000;

int main()
{
    double A[MAX][MAX];
    double x[MAX];
    double y[MAX];

    for(int i=0; i<MAX; i++)
    {
        x[i] = i;
        y[i] = 0;
        for(int j=0; j<MAX; j++)
        {
            A[i][j] = j;
        }
    }

    //Tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

    ///Primer par de bucles
    for(int i=0; i<MAX; i++)
    {
        for(int j=0; j<MAX; j++)
        {
            y[i] += A[i][j] * x[j];
        }
    }

    //Tiempo de fin
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    cout << "Tiempo de ejecucion: " << elapsed.count() << " segundos" << endl;

    return 0;
}
