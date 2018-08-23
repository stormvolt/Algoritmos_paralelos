#include <iostream>
#include <algorithm>
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

    ///Algoritmo de 6 bucles
    int block = MAX/10;
    for(int ii=0; ii<MAX; ii+=block)
    {
        for(int jj=0; jj<MAX; jj+=block)
        {
            for(int kk=0; kk<MAX; kk+=block)
            {
                for(int i=ii; i<min(ii+block-1,MAX); i++)
                {
                    for(int j=jj; j<min(jj+block-1,MAX); j++)
                    {
                        for(int k=kk; i<min(kk+block-1,MAX); i++)
                        {
                            m_c[i][j] += m_a[i][k] * m_b[k][j];
                        }
                    }
                }
            }
        }
    }

    //Tiempo de fin
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    cout << "Tiempo de ejecucion: " << elapsed.count() << " segundos" << endl;

    return 0;
}
