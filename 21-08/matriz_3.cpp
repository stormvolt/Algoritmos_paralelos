#include <iostream>
#include <chrono>

using namespace std;

const int MAX = 800;

int main()
{
    int m_a[MAX][MAX];
    int m_b[MAX][MAX];
    int m_c[MAX][MAX];

    //iniciando matrices
    for(int i=0; i<MAX; i++)
    {
        for(int j=0; j<MAX; j++)
        {
            m_a[i][j] = j;
            m_a[i][j] = j+1;
            m_c[i][j] = 0;
        }
    }

    //Tiempo de inicio
    auto start = std::chrono::high_resolution_clock::now();

    ///Algoritmo tradicional
    for(int i=0; i<MAX; i++)
    {
        for(int j=0; j<MAX; j++)
        {
            for(int k=0; k<MAX; k++)
            {
                m_c[i][j] += m_a[i][k] * m_b[k][j];
            }
        }
    }

    //Tiempo de fin
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    cout << "Tiempo de ejecucion: " << elapsed.count() << " segundos" << endl;

    return 0;
}
