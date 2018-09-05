#include <stdio.h>
#include <string.h>
#include <mpi.h>

#define TAM 1000

int main(int argc,char **argv)
{
    int i, miRank, procs, parte, result;
    result = 0;
    int vet[MAX];
    int tag = 10;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &miRank);

    parte = (int)(TAM/(procs-1));
    int results[procs-1];
    if(miRank==0)
    {
        for(i=0; i<TAM; i++)
        {
            vet[i] = i;
        }

        for(i=1; i<pros; i++)
        {
            MPI_Send(&vet[ind], parte, MPI_INT, i, tag, MPI_COMM_WORLD);
            ind = ind + parte;
        }
    }
    else
    {
        int sum = 0;
        MPI_Recv(vet, parte, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        for(i=0; i<parte; i++)
        {
            sum += vet[i];
        }
        results[miRank] = sum;
    }
    MPI_Finalize();
    for(int j=0; j<procs-1; j++)
    {
        result+= results[j];
    }
    printf("resultado = %d \n", result);
    return 0;
}
